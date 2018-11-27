#ifndef CLUSTER_H
#define CLUSTER_H

#include "data.h"

class cluster {
  private:
    int centroidIndex;
    vector<double> centroid;
    vector<int> points;
  public:
    cluster(int i, data &dataset);
    ~cluster();
    vector<double> getCentroid(void);
    void printCentroid(void);
    void printPoints(void);
    void addPoint(int index);
    int getNumberOfPoints();
    int getIndex(void);
};

#endif
