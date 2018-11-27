#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "hashTable.h"

void lloyd(vector<cluster> &clusters, data &dataset);
void LSH(vector<cluster> &clusters, data &dataset, vector<hashTable> &hTables, vector<int> &r, int L, int k);
double initRadius(vector<cluster> &clusters, data &dataset);

#endif
