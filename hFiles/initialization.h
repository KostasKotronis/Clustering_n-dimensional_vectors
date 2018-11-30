#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "data.h"

vector<int> RandomSelectionOfCentroids(data &dataset);
vector<int> kMeansPlusPlus(data &dataset);
int binarySearch(vector<double> &partialSums, double x);

#endif
