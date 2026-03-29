//
// Created by afons on 16/03/2026.
//

#include <iostream>
#include <vector>
#include "Parser.h"

using namespace std;

int main() {
    vector<Submission> submissoes;
    vector<Reviewer> revisores;

    cout << "A ler o ficheiro..." << endl;
    lerFicheiro("../Project1SampleDatasets/input/dataset1.csv", submissoes, revisores);

    cout << "Leitura concluida com sucesso!" << endl;
    cout << "Total de Submissoes prontas a usar: " << submissoes.size() << endl;
    cout << "Total de Revisores prontos a usar: " << revisores.size() << endl;

    return 0;
}