//
// Created by afons on 16/03/2026.
//

#ifndef PROJECT1SAMPLEDATASETS_PARSER_H
#define PROJECT1SAMPLEDATASETS_PARSER_H

#include <string>
#include <vector>
#include "models.h"

void lerFicheiro(const std::string& filename, std::vector<Submission>& submissoes, std::vector<Reviewer>& revisores, Config& config);

void escreverOutput(const std::string& filename, std::vector<Assignment>& assignments);

#endif //PROJECT1SAMPLEDATASETS_PARSER_H