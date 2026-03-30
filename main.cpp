#include <iostream>
#include <vector>
#include <string>
#include "Parser.h"
#include "models.h"
#include "EdmondsKarp.h"
#include "data_structures/Graph.h"

using namespace std;

int main(int argc, char* argv[]) {
    string inputFile = "";
    string outputFile = "";
    bool isBatchMode = false;

    // Verificar se arrancámos em modo Batch: myProg -b input.csv output.csv
    if (argc >= 4 && string(argv[1]) == "-b") {
        isBatchMode = true;
        inputFile = argv[2];
        outputFile = argv[3];
    } else {
        cout << "Arrancar em Modo Normal (Terminal)...\n";
        inputFile = "../Project1SampleDatasets/input/dataset1.csv"; // Ajustar para o default local
    }

    vector<Submission> submissoes;
    vector<Reviewer> revisores;
    Config config;

    cout << "A ler o ficheiro: " << inputFile << " ..." << endl;
    lerFicheiro(inputFile, submissoes, revisores, config);

    if (isBatchMode) {
        config.outputFileName = outputFile;
    }

    cout << "Leitura concluida com sucesso!" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Submissoes lidas: " << submissoes.size() << endl;
    cout << "Revisores lidos: " << revisores.size() << endl;
    cout << "Reviews Minimas p/ Artigo: " << config.minReviewsPerSubmission << endl;
    cout << "Ficheiro de Destino configurado para: " << config.outputFileName << endl;
    cout << "-----------------------------------" << endl;

    if (!isBatchMode) {
        int opcao = -1;
        while (opcao != 0) {
            cout << "\n=== MENU DA CONFERENCIA ===" << endl;
            cout << "1. Gerar Atribuicoes (Max-Flow)" << endl;
            cout << "2. Analise de Risco (K=1)" << endl;
            cout << "0. Sair" << endl;
            cout << "Escolha uma opcao: ";
            cin >> opcao;

            if (opcao == 1) {
                cout << "\n--- INICIAR ATRIBUICAO DE REVISORES ---" << endl;

                Graph<int> conferenciaGraph;
                int SOURCE_ID = 0;
                int SINK_ID = 999999;

                cout << "A construir a Rede de Fluxo..." << endl;
                conferenciaGraph.addVertex(SOURCE_ID);
                conferenciaGraph.addVertex(SINK_ID);

                for (const auto& r : revisores) {
                    conferenciaGraph.addVertex(r.id);
                    addFlowEdge(&conferenciaGraph, SOURCE_ID, r.id, config.maxReviewsPerReviewer);
                }

                for (const auto& s : submissoes) {
                    conferenciaGraph.addVertex(s.id);
                    addFlowEdge(&conferenciaGraph, s.id, SINK_ID, config.minReviewsPerSubmission);
                }

                int matchesPossiveis = 0;
                for (const auto& r : revisores) {
                    for (const auto& s : submissoes) {
                        if (r.primaryExpertise == s.primaryDomain) {
                            addFlowEdge(&conferenciaGraph, r.id, s.id, 1);
                            matchesPossiveis++;
                        }
                    }
                }
                cout << "-> Arestas de Match criadas: " << matchesPossiveis << endl;

                cout << "A calcular o fluxo maximo (Edmonds-Karp)..." << endl;
                edmondsKarp(&conferenciaGraph, SOURCE_ID, SINK_ID);

                cout << "A extrair as atribuicoes geradas..." << endl;
                vector<Assignment> atribuicoes = extrairAtribuicoes(&conferenciaGraph, revisores);

                //  Guardamos o resultado na variável 'sucesso'
                bool sucesso = verificarSucesso(&conferenciaGraph, SINK_ID, submissoes.size(), config.minReviewsPerSubmission);

                if (!atribuicoes.empty()) {
                    escreverOutput(config.outputFileName, atribuicoes);
                } else {
                    cout << "Aviso: Nenhuma atribuicao foi gerada (O grafo pode estar vazio)." << endl;
                }

                if (!sucesso) {
                    cout << "A gerar registo de Missing Reviews..." << endl;
                    vector<MissingReview> missing;
                    for (const auto& s : submissoes) {
                        Vertex<int>* vSub = conferenciaGraph.findVertex(s.id);
                        double fluxoDesteArtigo = 0;
                        for (auto e : vSub->getAdj()) {
                            if (e->getDest()->getInfo() == SINK_ID) {
                                fluxoDesteArtigo = e->getFlow();
                            }
                        }
                        if (fluxoDesteArtigo < config.minReviewsPerSubmission) {
                            missing.push_back({s.id, s.primaryDomain, (int)(config.minReviewsPerSubmission - fluxoDesteArtigo)});
                        }
                    }
                    escreverMissingReviews(config.outputFileName, missing);
                }
            }
            else if (opcao == 2) {
                if (config.riskAnalysis == 1) {
                    cout << "\n--- INICIAR ANALISE DE RISCO (K=1) ---" << endl;

                    Graph<int> riskGraph;
                    int SOURCE_ID = 0;
                    int SINK_ID = 999999;

                    riskGraph.addVertex(SOURCE_ID);
                    riskGraph.addVertex(SINK_ID);

                    for (const auto& r : revisores) {
                        riskGraph.addVertex(r.id);
                        addFlowEdge(&riskGraph, SOURCE_ID, r.id, config.maxReviewsPerReviewer);
                    }
                    for (const auto& s : submissoes) {
                        riskGraph.addVertex(s.id);
                        addFlowEdge(&riskGraph, s.id, SINK_ID, config.minReviewsPerSubmission);
                    }
                    for (const auto& r : revisores) {
                        for (const auto& s : submissoes) {
                            if (r.primaryExpertise == s.primaryDomain) {
                                addFlowEdge(&riskGraph, r.id, s.id, 1);
                            }
                        }
                    }

                    int revisoesNecessarias = submissoes.size() * config.minReviewsPerSubmission;
                    vector<int> riskyReviewers;

                    cout << "A simular a falha de cada revisor individualmente..." << endl;

                    for (const auto& r : revisores) {
                        riskGraph.removeEdge(SOURCE_ID, r.id);
                        addFlowEdge(&riskGraph, SOURCE_ID, r.id, 0);

                        edmondsKarp(&riskGraph, SOURCE_ID, SINK_ID);

                        double fluxoTotal = 0;
                        Vertex<int>* sink = riskGraph.findVertex(SINK_ID);
                        for (auto e : sink->getIncoming()) {
                            fluxoTotal += e->getFlow();
                        }

                        if (fluxoTotal < revisoesNecessarias) {
                            riskyReviewers.push_back(r.id);
                        }

                        riskGraph.removeEdge(SOURCE_ID, r.id);
                        addFlowEdge(&riskGraph, SOURCE_ID, r.id, config.maxReviewsPerReviewer);
                    }

                    cout << "\n=== RESULTADOS DA ANALISE DE RISCO ===" << endl;
                    if(riskyReviewers.empty()) {
                        cout << "Nenhum revisor e critico. O sistema e robusto!" << endl;
                    } else {
                        cout << "Foram encontrados " << riskyReviewers.size() << " revisores criticos!" << endl;
                        for (int id : riskyReviewers) {
                            cout << "-> Revisor ID: " << id << endl;
                        }
                        escreverRiskAnalysis(config.outputFileName, 1, riskyReviewers);
                    }
                } else {
                    cout << "A Analise de Risco esta desativada no ficheiro de configuracao (RiskAnalysis = 0)." << endl;
                }
            }
            else if (opcao != 0) {
                cout << "Opcao invalida. Tente novamente." << endl;
            }
        }
    } else {
        cout << "A executar algoritmo em modo Batch..." << endl;

        Graph<int> conferenciaGraph;
        int SOURCE_ID = 0;
        int SINK_ID = 999999;

        conferenciaGraph.addVertex(SOURCE_ID);
        conferenciaGraph.addVertex(SINK_ID);

        for (const auto& r : revisores) {
            conferenciaGraph.addVertex(r.id);
            addFlowEdge(&conferenciaGraph, SOURCE_ID, r.id, config.maxReviewsPerReviewer);
        }

        for (const auto& s : submissoes) {
            conferenciaGraph.addVertex(s.id);
            addFlowEdge(&conferenciaGraph, s.id, SINK_ID, config.minReviewsPerSubmission);
        }

        for (const auto& r : revisores) {
            for (const auto& s : submissoes) {
                if (r.primaryExpertise == s.primaryDomain) {
                    addFlowEdge(&conferenciaGraph, r.id, s.id, 1);
                }
            }
        }

        edmondsKarp(&conferenciaGraph, SOURCE_ID, SINK_ID);
        vector<Assignment> atribuicoes = extrairAtribuicoes(&conferenciaGraph, revisores);
        bool sucesso = verificarSucesso(&conferenciaGraph, SINK_ID, submissoes.size(), config.minReviewsPerSubmission);

        if (!atribuicoes.empty()) {
            escreverOutput(config.outputFileName, atribuicoes);
        }

        if (!sucesso) {
            vector<MissingReview> missing;
            for (const auto& s : submissoes) {
                Vertex<int>* vSub = conferenciaGraph.findVertex(s.id);
                double fluxoDesteArtigo = 0;
                for (auto e : vSub->getAdj()) {
                    if (e->getDest()->getInfo() == SINK_ID) {
                        fluxoDesteArtigo = e->getFlow();
                    }
                }
                if (fluxoDesteArtigo < config.minReviewsPerSubmission) {
                    missing.push_back({s.id, s.primaryDomain, (int)(config.minReviewsPerSubmission - fluxoDesteArtigo)});
                }
            }
            escreverMissingReviews(config.outputFileName, missing);
        }
    }

    return 0;
}