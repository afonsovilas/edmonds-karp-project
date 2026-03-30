//
// Created by afons on 16/03/2026.
//

#ifndef PROJECT1SAMPLEDATASETS_MODELS_H
#define PROJECT1SAMPLEDATASETS_MODELS_H

#include <string>
#include <vector>

using namespace std;

struct Submission {
    int id;
    string title;
    string authors;
    string email;
    int primaryDomain;
    int secondaryDomain;
};

struct Reviewer {
    int id;
    string name;
    string email;
    int primaryExpertise;
    int secondaryExpertise;
};

struct Config {
    int minReviewsPerSubmission = 0;
    int maxReviewsPerReviewer = 0;
    int primaryReviewerExpertise = 0;
    int secondaryReviewerExpertise = 0;
    int primarySubmissionDomain = 0;
    int secondarySubmissionDomain = 0;
    int generateAssignments = 0;
    int riskAnalysis = 0;
    string outputFileName = "output.csv"; // Valor por defeito
};
struct Assignment {
    int submissionId;
    int reviewerId;
    int matchDomain;
};
struct MissingReview {
    int submissionId;
    int domain;
    int missingCount;
};

#endif //PROJECT1SAMPLEDATASETS_MODELS_H