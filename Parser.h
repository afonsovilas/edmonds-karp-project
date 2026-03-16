//
// Created by afons on 16/03/2026.
//

#ifndef PROJECT1SAMPLEDATASETS_PARSER_H
#define PROJECT1SAMPLEDATASETS_PARSER_H

#include <string>
#include <vector>

struct Submission {
    int id;
    std::string title, authors, email;
    int primaryDomain, secondaryDomain;
};

struct Reviewer {
    int id;
    std::string name, email;
    int primaryExpertise, secondaryExpertise;
};

void lerFicheiro(const std::string& filename, std::vector<Submission>& submissoes, std::vector<Reviewer>& revisores);

#endif //PROJECT1SAMPLEDATASETS_PARSER_H