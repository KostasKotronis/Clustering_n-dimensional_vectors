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
    cluster(void);
    ~cluster();
    vector<double> getCentroid(void);
    void setCentroid(vector<double> &c);
    void printCentroid(void);
    int getPoint(int p);
    vector<int> getPoints(void);
    void setPoints(vector<int> &p);
    void printPoints(void);
    void clearPoints(void);
    void addPoint(int index);
    int getNumberOfPoints(void);
    int getIndex(void);
    void createClone(cluster &clone);
    int equalCluster(cluster &c);
    void kMeansUpdateCentroid(data &dataset);
    void pamUpdateCentroid(data &dataset);
    int isMember(int index);
};

#endif
