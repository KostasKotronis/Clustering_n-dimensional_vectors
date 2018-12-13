#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"
#include "./hFiles/assignment.h"
#include "./hFiles/hashTable.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"
#include "./hFiles/generalFunctions.h"

//assign points to clusters via lloyd algorithm
void lloyd(vector<cluster> &clusters, data &dataset) {
  string datasetMetric = dataset.getMetric();
  int datasetN = dataset.getN();
  for(int i=0; i<datasetN; i++) {
    double minDis = -1;
    int closestCentroid = -1;
    vector<double> x1 = dataset.getdVector(i).getCoordinates();
    for(int j=0; j<numberOfClusters; j++) {                                     //find the min distance from centroids
      double dis;
      vector<double> x2 = clusters[j].getCentroid();
      if(datasetMetric == "euclidean")
        dis = euclideanDistance(x1, x2);
      if(datasetMetric == "cosine")
        dis = cosineDistance(x1, x2);
      if((dis < minDis) || (minDis == -1)) {
        minDis = dis;
        closestCentroid = j;
      }
    }
    clusters[closestCentroid].addPoint(i);                                      //add point to the cluster with min distance from centroid
  }
  //cout << "We have a Lloyd assignment right here!" << endl;
};

//assign most points to clusters via lsh algorithm ->rest :lloyd
void LSH(vector<cluster> &clusters, data &dataset, vector<hashTable> &hTables, vector<int> &r, int L, int k) {
  string datasetMetric = dataset.getMetric();
  int datasetN = dataset.getN();
  double Radius = initRadius(clusters, datasetMetric);                          //calculate radius
  int cl[datasetN];                                                             //for every point, store the centroid
  double dis[datasetN];                                                         //for every point, store the distance from centroid
  int finalized[datasetN];                                                      //for every point, store 0 if i can change the values, 1 otherwise
  fill_n(cl, datasetN, 0);
  fill_n(dis, datasetN, -1);
  fill_n(finalized, datasetN, 0);
  int oldAssigned = -1;
  int currentAssigned = 0;
  int n = maxRangeSearchIterations;
  while(n && (oldAssigned != currentAssigned)) {
    oldAssigned = currentAssigned;
    currentAssigned = 0;
    for(int c=0; c<numberOfClusters; c++) {                                     //for every centroid
      int curCentroid = clusters[c].getIndex();
      dVector v = dataset.getdVector(curCentroid);                              //v = current centroid
      for(int i=0; i<L; i++) {                                                  //in every hashtable
        vector<int> g;
        int f;
        if(datasetMetric == "euclidean") {
          vector<vector<double>> vs = hTables[i].getkVs();
          vector<double> ts = hTables[i].getkTs();
          g = euclideanGenerateG(v, k, vs, ts);                                 //generate a g vector
          f = fHushFunction(g, r, dataset.getN());                              //from that g -> generate fhushfunction
        }
        else if(datasetMetric == "cosine") {
          vector<vector<double>> rs = hTables[i].getkRs();
          g = cosineGenerateG(v, k, rs);                                        //generate a g vector
          f = binaryToDecimalConverter(g);                                      //from that g -> generate f(key)
        }
        vector<hashNode> hn = hTables[i].returnSameBucket(f);                   //get the bucket
        for(auto j=hn.begin(); j!=hn.end(); ++j) {                              //if g match
          int jIndex = j->getIndex();
          if(finalized[jIndex] == 0) {
            double Distance;                                                    //calculate distance
            if(datasetMetric == "euclidean" && j->getG() == g) {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(jIndex).getCoordinates();
              Distance = euclideanDistance(x1, x2);
              if( (Distance < Radius) && ( (dis[jIndex] == -1) || (dis[jIndex] > Distance) ) ) {                                                //if distance is acceptable
                cl[jIndex] = c;
                dis[jIndex] = Distance;
              }
            }
            else if(datasetMetric == "cosine") {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(jIndex).getCoordinates();
              Distance = cosineDistance(x1, x2);
              if( (Distance < Radius) && ( (dis[jIndex] == -1) || (dis[jIndex] > Distance) ) ) {                                                //if distance is acceptable
                cl[jIndex] = c;
                dis[jIndex] = Distance;
              }
            }
          }
        }
      }
    }
    for(int c=0; c<datasetN; c++) {                                             //set finalized = 1 for every point that we have calculate distance
      if(dis[c] != -1) {
        finalized[c] = 1;
        currentAssigned++;
      }
    }
    Radius = Radius * 2;                                                        //increase radius
    n--;
  }
  for(int i=0; i<datasetN; i++) {                                               //we need some lloyd right here!
    double minDis = -1;
    int closestCentroid = -1;
    vector<double> x1 = dataset.getdVector(i).getCoordinates();
    if(finalized[i] == 0) {
      for(int j=0; j<numberOfClusters; j++) {
        double dist;
        vector<double> x2 = clusters[j].getCentroid();
        if(datasetMetric == "euclidean")
          dist = euclideanDistance(x1, x2);
        if(datasetMetric == "cosine")
          dist = cosineDistance(x1, x2);
        if((dist < minDis) || (minDis == -1)) {
          minDis = dist;
          closestCentroid = j;
        }
      }
      cl[i] = closestCentroid;
      dis[i] = minDis;
      finalized[i] = 1;
    }
    clusters[cl[i]].addPoint(i);
  }
  //cout <<"LSH assignment completed." << endl;
};

//assign most points to clusters via cube algorithm ->rest :lloyd
void cube(vector<cluster> &clusters, data &dataset, hashTable &hTable, vector<int> &r, int M, int k, int probes) {
  string datasetMetric = dataset.getMetric();
  int datasetN = dataset.getN();
  double Radius = initRadius(clusters, datasetMetric);                          //calculate radius
  int cl[datasetN];                                                             //for every point, store the centroid
  double dis[datasetN];                                                         //for every point, store the distance from centroid
  int finalized[datasetN];                                                      //for every point, store 0 if i can change the values, 1 otherwise
  fill_n(cl, datasetN, 0);
  fill_n(dis, datasetN, -1);
  fill_n(finalized, datasetN, 0);
  int oldAssigned = -1;
  int currentAssigned = 0;
  int n = maxRangeSearchIterations;
  while(n && (oldAssigned != currentAssigned)) {                               //number of iterations
    oldAssigned = currentAssigned;
    currentAssigned = 0;
    for(int c=0; c<numberOfClusters; c++) {                                     //for every centroid
      int curCentroid = clusters[c].getIndex();
      dVector v = dataset.getdVector(curCentroid);                              //v = current centroid
      vector<int> g;
      int f;
      if(datasetMetric == "euclidean") {
        vector<vector<double>> vs = hTable.getkVs();
        vector<double> ts = hTable.getkTs();
        g = euclideanGenerateG(v, k, vs, ts);                                   //generate a g vector
        f = fHushFunction(g, r, dataset.getN());                                //from that g -> generate fhushfunction
      }
      else if(datasetMetric == "cosine") {
        vector<vector<double>> rs = hTable.getkRs();
        g = cosineGenerateG(v, k, rs);                                          //generate a g vector
        f = binaryToDecimalConverter(g);                                        //from that g -> generate f(key)
      }
      int originF = f;
      int countM = M, countProbes = probes;
      vector<int> uKeys = hTable.getUniqueKeys();
      while(countM && countProbes) {
        vector<hashNode> hn = hTable.returnSameBucket(f);                       //get the bucket
        for(auto j=hn.begin(); j!=hn.end(); ++j) {
          int jIndex = j->getIndex();
          if(finalized[jIndex] == 0) {
            double Distance;                                                    //calculate distance
            if(datasetMetric == "euclidean") {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(jIndex).getCoordinates();
              Distance = euclideanDistance(x1, x2);
            }
            else if(datasetMetric == "cosine") {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(jIndex).getCoordinates();
              Distance = cosineDistance(x1, x2);
            }
            if( (Distance < Radius) && ( (dis[jIndex] == -1) || (dis[jIndex] > Distance) ) ) {                                                //if distance is acceptable
                cl[jIndex] = c;
                dis[jIndex] = Distance;
              }
            countM--;
            if(countM == M)
              break;
          }
        }
        countProbes--;
        uKeys.erase(remove(uKeys.begin(), uKeys.end(), f), uKeys.end());        //erase the current f from used keys to avoid loop
        //hamming distance
        int minHammingDistance = 99999;
        int minKey;
        if(countProbes) {
          int minHammingDistance = 99999;
          int minKey;
          for(auto i=uKeys.begin(); i!=uKeys.end(); ++i) {                      //for every used keys, find the nearest
            int hm = __builtin_popcount(originF^(*i));                          //calculate hamming distance
            if(hm < minHammingDistance) {
              minHammingDistance = hm;
              minKey = *i;
            }
          }
          f = minKey;
        }
      }
    }
    for(int c=0; c<datasetN; c++) {                                             //set finalized = 1 for every point that we have calculate distance
      if(dis[c] != -1) {
        finalized[c] = 1;
        currentAssigned++;
      }
    }
    Radius = Radius * 2;
    n--;
  }
  for(int i=0; i<datasetN; i++) {                                               //we need some lloyd right here!
    double minDis = -1;
    int closestCentroid = -1;
    if(finalized[i] == 0) {                                                     //not finalized
      for(int j=0; j<numberOfClusters; j++) {
        double dist;
        vector<double> x1 = dataset.getdVector(i).getCoordinates();
        vector<double> x2 = clusters[j].getCentroid();
        if(datasetMetric == "euclidean")
          dist = euclideanDistance(x1, x2);
        if(datasetMetric == "cosine")
          dist = cosineDistance(x1, x2);
        if((dist < minDis) || (minDis == -1)) {
          minDis = dist;
          closestCentroid = j;
        }
      }
      cl[i] = closestCentroid;
      dis[i] = minDis;
      finalized[i] = 1;
    }
    clusters[cl[i]].addPoint(i);
  }
  //cout <<"Cube assignment completed." << endl;
};

//calculate and return the min distance (/2) between centroids
double initRadius(vector<cluster> &clusters, string &datasetMetric) {
  //string datasetMetric = dataset.getMetric();
  double minDis = -1;
  for(int i=0; i<numberOfClusters; i++) {
    vector<double> x1 = clusters[i].getCentroid();
    for(int j=0; j<numberOfClusters; j++) {
      if(i < j) {
        double dis;
        vector<double> x2 = clusters[j].getCentroid();
        if(datasetMetric == "euclidean")
          dis = euclideanDistance(x1, x2);
        if(datasetMetric == "cosine")
          dis = cosineDistance(x1, x2);
        if((dis < minDis) || (minDis == -1))
          minDis = dis;
      }
    }
  }
  return minDis/2;
};
