//
// Created by afons on 16/03/2026.
//

#include "Parser.h"
#include "models.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void lerFicheiro(const string& filename, vector<Submission>& submissoes, vector<Reviewer>& revisores, Config& config) {    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o ficheiro " << filename << "!" << endl;
        return;
    }

    string line;
    int seccaoAtual = 0;

    while (getline(file, line)) {
        size_t cardinalPos = line.find('#');
        if (cardinalPos != string::npos) {
            string textAfter = trim(line.substr(cardinalPos));
            if (textAfter == "#Submissions") { seccaoAtual = 1; continue; }
            if (textAfter == "#Reviewers") { seccaoAtual = 2; continue; }
            if (textAfter == "#Parameters" || textAfter == "#Control") { seccaoAtual = 3; continue; }

            if (cardinalPos == 0) continue;
            line = line.substr(0, cardinalPos);
        }

        line = trim(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string token;

        if (seccaoAtual == 1) {
            Submission s;
            s.secondaryDomain = -1;

            getline(ss, token, ','); s.id = stoi(trim(token));
            getline(ss, token, ','); s.title = trim(token);
            getline(ss, token, ','); s.authors = trim(token);
            getline(ss, token, ','); s.email = trim(token);
            getline(ss, token, ','); s.primaryDomain = stoi(trim(token));

            if (getline(ss, token, ',')) {
                token = trim(token);
                if (!token.empty()) s.secondaryDomain = stoi(token);
            }
            submissoes.push_back(s);
        }
        else if (seccaoAtual == 2) {
            Reviewer r;
            r.secondaryExpertise = -1;

            getline(ss, token, ','); r.id = stoi(trim(token));
            getline(ss, token, ','); r.name = trim(token);
            getline(ss, token, ','); r.email = trim(token);
            getline(ss, token, ','); r.primaryExpertise = stoi(trim(token));

            if (getline(ss, token, ',')) {
                token = trim(token);
                if (!token.empty()) r.secondaryExpertise = stoi(token);
            }
            revisores.push_back(r);
        }
        else if (seccaoAtual == 3) {
            // Formato: "Chave, Valor"
            string key, val;
            if (getline(ss, key, ',')) {
                if (getline(ss, val)) {
                    key = trim(key);
                    val = trim(val);

                    if (key == "MinReviewsPerSubmission") config.minReviewsPerSubmission = stoi(val);
                    else if (key == "MaxReviewsPerReviewer") config.maxReviewsPerReviewer = stoi(val);
                    else if (key == "PrimaryReviewerExpertise") config.primaryReviewerExpertise = stoi(val);
                    else if (key == "SecondaryReviewerExpertise") config.secondaryReviewerExpertise = stoi(val);
                    else if (key == "PrimarySubmissionDomain") config.primarySubmissionDomain = stoi(val);
                    else if (key == "SecondarySubmissionDomain") config.secondarySubmissionDomain = stoi(val);
                    else if (key == "GenerateAssignments") config.generateAssignments = stoi(val);
                    else if (key == "RiskAnalysis") config.riskAnalysis = stoi(val);
                    else if (key == "OutputFileName") config.outputFileName = val;
                }
            }
        }
    }
    file.close();
}
