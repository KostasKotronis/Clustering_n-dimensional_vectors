#ifndef EUCLIDEAN_H
#define EUCLIDEAN_H

int euclideanProject(dVector &p,vector<vector<double>> &v, vector<double> &t);

vector<int> euclideanGenerateG(dVector &p, int k, vector<vector<double>> &v, vector<double> &t);

int fHushFunction(vector<int> &g, vector<int> &r, int n);

double euclideanDistance(vector<double> &x, vector<double> &y);

int cubeGenerateKey(dVector &p, int k, vector<vector<double>> &v, vector<double> &t);

#endif
