#ifndef INITIALIZATION_H
#define INITIALIZATION_H

vector<int> RandomSelectionOfCentroids(int numberOfClusters, data &dataset);
vector<int> kMeansPlusPlus(int numberOfClusters, data &dataset);
int binarySearch(vector<double> &partialSums, double x);

#endif
