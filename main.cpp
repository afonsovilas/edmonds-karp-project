// Created by afons on 16/03/2026.

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
        // Modo fallback para correres no CLion sem argumentos
        cout << "Arrancar em Modo Normal (Terminal)...\n";
        inputFile = "../Project1SampleDatasets/input/dataset1.csv"; // Ajusta para o teu default local
    }

    vector<Submission> submissoes;
    vector<Reviewer> revisores;
    Config config;

    cout << "A ler o ficheiro: " << inputFile << " ..." << endl;
    lerFicheiro(inputFile, submissoes, revisores, config);

    // Se estiver em batch mode, o ficheiro de output dado no terminal tem prioridade
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
            cout << "1. Testar Escrita de Output (Dummy Data)" << endl;
            cout << "2. Gerar Atribuicoes (Max-Flow - A cargo da Pessoa 1)" << endl;
            cout << "3. Analise de Risco (A cargo da Pessoa 1)" << endl;
            cout << "0. Sair" << endl;
            cout << "Escolha uma opcao: ";
            cin >> opcao;

            if (opcao == 1) {
                // TU VAIS TESTAR A TUA FUNÇÃO AQUI!
                vector<Assignment> dummyData = {
                    {87, 2, 1},
                    {31, 1, 3}
                };
                escreverOutput(config.outputFileName, dummyData);
            }
            else if (opcao == 2) {
                cout << "\n--- INICIAR ATRIBUICAO DE REVISORES ---" << endl;

                Graph<int> conferenciaGraph;

                // Definimos IDs estáticos para a SOURCE e SINK para não se misturarem com Revisores/Artigos
                int SOURCE_ID = 0;
                int SINK_ID = 999999;

                // ==========================================================
                // CONSTRUÇÃO DO GRAFO (Ligar os dados ao Edmonds-Karp)
                // ==========================================================
                cout << "A construir a Rede de Fluxo..." << endl;

                // 1. Adicionar os nós virtuais (Source e Sink)
                conferenciaGraph.addVertex(SOURCE_ID);
                conferenciaGraph.addVertex(SINK_ID);

                // 2. Adicionar Revisores e ligar à SOURCE
                // Capacidade = maxReviewsPerReviewer
                for (const auto& r : revisores) {
                    conferenciaGraph.addVertex(r.id);
                    addFlowEdge(&conferenciaGraph, SOURCE_ID, r.id, config.maxReviewsPerReviewer);
                }

                // 3. Adicionar Submissões e ligar ao SINK
                // Capacidade = minReviewsPerSubmission
                for (const auto& s : submissoes) {
                    conferenciaGraph.addVertex(s.id);
                    addFlowEdge(&conferenciaGraph, s.id, SINK_ID, config.minReviewsPerSubmission);
                }

                // 4. Fazer os Matches (Ligar Revisores a Artigos compatíveis)
                // Capacidade = 1 (um revisor só revê o mesmo artigo 1 vez)
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
                // ==========================================================

                // A Pessoa 1 deve adicionar os nós e usar a função addFlowEdge()
                // definida no EdmondsKarp.h para ligar:
                // SOURCE -> Revisores -> Submissões -> SINK

                // ==========================================================
                // AS TUAS TAREFAS (T1 e T2) ENTRAM EM AÇÃO AQUI
                // ==========================================================

                cout << "A calcular o fluxo maximo (Edmonds-Karp)..." << endl;
                // Tarefa 1: Corre o algoritmo no grafo construído pela Pessoa 1
                edmondsKarp(&conferenciaGraph, SOURCE_ID, SINK_ID);

                cout << "A extrair as atribuicoes geradas..." << endl;
                // Tarefa 2: Retira os pares (Revisor-Artigo) onde o fluxo é > 0
                vector<Assignment> atribuicoes = extrairAtribuicoes(&conferenciaGraph, revisores);

                // Tarefa 2: Verifica se o fluxo no SINK cumpre o mínimo exigido
                verificarSucesso(&conferenciaGraph, SINK_ID, submissoes.size(), config.minReviewsPerSubmission);

                // Escreve os resultados finais no ficheiro CSV usando a função do Parser.cpp
                if (!atribuicoes.empty()) {
                    escreverOutput(config.outputFileName, atribuicoes);
                } else {
                    cout << "Aviso: Nenhuma atribuicao foi gerada (O grafo pode estar vazio)." << endl;
                }
            }
            else if (opcao == 3) {
                if (config.riskAnalysis == 1) { // Verifica a flag do config lido do ficheiro
                    cout << "\n--- INICIAR ANALISE DE RISCO ---" << endl;

                    // Nota: O teu colega tem de ter construído a 'conferenciaGraph' antes!
                    // Esta variável tem de estar acessível aqui, ou têm de a construir globalmente.
                    int SOURCE_ID = 0;
                    int SINK_ID = 999999;

                    cout << "A testar remocao individual de revisores..." << endl;

                    vector<int> emRisco = analiseDeRisco(&conferenciaGraph, SOURCE_ID, SINK_ID, revisores, submissoes.size(), config.minReviewsPerSubmission);

                    cout << "\n=== RESULTADOS DA ANALISE DE RISCO ===" << endl;
                    if (emRisco.empty()) {
                        cout << "Nenhum revisor e critico. O sistema e robusto!" << endl;
                    } else {
                        cout << "Foram encontrados " << emRisco.size() << " revisores de risco:" << endl;
                        for (int id : emRisco) {
                            cout << "-> Revisor ID: " << id << endl;
                        }
                    }
                } else {
                    cout << "A Analise de Risco esta desativada no ficheiro de configuracao (RiskAnalysis = 0)." << endl;
                }
            }
        }
    } else {
        // MODO BATCH (Para o professor testar via script)
        cout << "A executar algoritmo em modo Batch..." << endl;
    }

    // O GRAFO COMEÇA A SER CONSTRUIDO AQUI ABAIXO!

    return 0;
}