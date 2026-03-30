// Created by afons on 16/03/2026.

#include <iostream>
#include <vector>
#include <string>
#include "Parser.h"
#include "models.h"

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
                cout << "Ainda em construcao pela Pessoa 1..." << endl;
            }
            else if (opcao == 3) {
                cout << "Ainda em construcao pela Pessoa 1..." << endl;
            }
        }
    } else {
        // MODO BATCH (Para o professor testar via script)
        cout << "A executar algoritmo em modo Batch..." << endl;
    }

    // O GRAFO COMEÇA A SER CONSTRUIDO AQUI ABAIXO!

    return 0;
}