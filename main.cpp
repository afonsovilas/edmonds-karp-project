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
        // O menu interativo vai ser feito aqui mais tarde!
        cout << "[MENU INTERATIVO AINDA EM CONSTRUCAO]" << endl;
    }

    // O GRAFO COMEÇA A SER CONSTRUIDO AQUI ABAIXO!

    return 0;
}