#ifndef INITIALIZATION_H
#define INITIALIZATION_H

vector<int> RandomSelectionOfCentroids(data &dataset);
vector<int> kMeansPlusPlus(data &dataset);
int binarySearch(vector<double> &partialSums, double x);

#endif
