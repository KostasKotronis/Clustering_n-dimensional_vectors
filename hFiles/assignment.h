#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "hashTable.h"
#include "cluster.h"

void lloyd(vector<cluster> &clusters, data &dataset);
void LSH(vector<cluster> &clusters, data &dataset, vector<hashTable> &hTables, vector<int> &r, int L, int k);
void cube(vector<cluster> &clusters, data &dataset, hashTable &hTable, vector<int> &r, int M, int k, int probes);
double initRadius(vector<cluster> &clusters, string &datasetMetric);

#endif
