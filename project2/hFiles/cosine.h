#ifndef COSINE_H
#define COSINE_H

#include "headers.h"

int cosineProject(dVector &p, vector<double> &r);

vector<int> cosineGenerateG(dVector &p, int k, vector<vector<double>> &kRs);

double cosineDistance(vector<double> &x, vector<double> &y);

#endif
