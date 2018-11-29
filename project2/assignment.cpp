#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"
#include "./hFiles/assignment.h"
#include "./hFiles/hashTable.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"
#include "./hFiles/generalFunctions.h"

//assign points to clusters via lloyd algorithm
void lloyd(vector<cluster> &clusters, data &dataset) {
  for(int i=0; i<dataset.getN(); i++) {
    double minDis = -1;
    int closestCentroid = -1;
    for(int j=0; j<clusters.size(); j++) {                                               //find the min distance from centroids
      double dis;
      vector<double> x1 = dataset.getdVector(i).getCoordinates();
      vector<double> x2 = clusters[j].getCentroid();
      if(dataset.getMetric() == "euclidean")
        dis = euclideanDistance(x1, x2);
      if(dataset.getMetric() == "cosine")
        dis = cosineDistance(x1, x2);
      if((dis < minDis) || (minDis == -1)) {
        minDis = dis;
        closestCentroid = j;
      }
    }
    clusters[closestCentroid].addPoint(i);                                              //add point to the cluster with min distance from centroid
  }
  //cout << "We have a Lloyd assignment right here!" << endl;
};

//void LSHanswerQueries(data &dataset, vector<hashTable> &hTables, string &queryFileName, string &outputFileName, vector<int> &r, int L, int k) {
void LSH(vector<cluster> &clusters, data &dataset, vector<hashTable> &hTables, vector<int> &r, int L, int k) {
  double Radius = initRadius(clusters, dataset);                //calculate radius
  int cl[dataset.getN()];                                       //for every point, store the centroid
  double dis[dataset.getN()];                                   //for every point, store the distance from centroid
  int finalized[dataset.getN()];                                //for every point, store 0 if i can change the values, 1 otherwise
  fill_n(cl, dataset.getN(), 0);
  fill_n(dis, dataset.getN(), -1);
  fill_n(finalized, dataset.getN(), 0);
  for(int n=0; n<N; n++) {                                                  //number of iterations(defined in headers.h)
    for(int c=0; c<clusters.size(); c++) {                                      //for every centroid
      int curCentroid = clusters[c].getIndex();
      dVector v = dataset.getdVector(curCentroid);                              //v = current centroid
      for(int i=0; i<L; i++) {                                                  //in every hashtable
        vector<int> g;
        int f;
        if(dataset.getMetric() == "euclidean") {
          vector<vector<double>> vs = hTables[i].getkVs();
          vector<double> ts = hTables[i].getkTs();
          g = euclideanGenerateG(v, k, vs, ts);                                   //generate a g vector
          f = fHushFunction(g, r, dataset.getN());                               //from that g -> generate fhushfunction
        }
        else if(dataset.getMetric() == "cosine") {
          vector<vector<double>> rs = hTables[i].getkRs();
          g = cosineGenerateG(v, k, rs);                                        //generate a g vector
          f = binaryToDecimalConverter(g);                                     //from that g -> generate f(key)
        }
        vector<hashNode> hn = hTables[i].returnSameBucket(f);                 //get the bucket
        for(auto j=hn.begin(); j!=hn.end(); ++j) {                                       //if g match
          if(finalized[j->getIndex()] == 0) {
            double Distance;                                                             //calculate distance
            if(dataset.getMetric() == "euclidean" && j->getG() == g) {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(j->getIndex()).getCoordinates();
              Distance = euclideanDistance(x1, x2);
              if( (Distance < Radius) && ( (dis[j->getIndex()] == -1) || (dis[j->getIndex()] > Distance) ) ) {                                                //if distance is acceptable
                cl[j->getIndex()] = c;
                dis[j->getIndex()] = Distance;
              }
            }
            else if(dataset.getMetric() == "cosine") {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(j->getIndex()).getCoordinates();
              Distance = cosineDistance(x1, x2);
              if( (Distance < Radius) && ( (dis[j->getIndex()] == -1) || (dis[j->getIndex()] > Distance) ) ) {                                                //if distance is acceptable
                cl[j->getIndex()] = c;
                dis[j->getIndex()] = Distance;
              }
            }
          }
        }
      }
    }
    for(int c=0; c<dataset.getN(); c++) {                                                     //set finalized = 1 for every point that we have calculate distance
      if(dis[c] != -1)
        finalized[c] = 1;
    }
    Radius = Radius * 2;                                                                      //increase radius
  }
  for(int i=0; i<dataset.getN(); i++) {                                                       //we need some lloyd right here!
    double minDis = -1;
    int closestCentroid = -1;
    if(finalized[i] == 0) {
      for(int j=0; j<clusters.size(); j++) {
        double dis;
        vector<double> x1 = dataset.getdVector(i).getCoordinates();
        vector<double> x2 = clusters[j].getCentroid();
        if(dataset.getMetric() == "euclidean")
          dis = euclideanDistance(x1, x2);
        if(dataset.getMetric() == "cosine")
          dis = cosineDistance(x1, x2);
        if((dis < minDis) || (minDis == -1)) {
          minDis = dis;
          closestCentroid = j;
        }
      }
      cl[i] = closestCentroid;
      dis[i] = minDis;
      finalized[i] = 1;
    }
  }
  for(int c=0; c<dataset.getN(); c++) {                                                       //store point to the correct cluster
    clusters[cl[c]].addPoint(c);
  }
  //cout <<"LSH assignment completed." << endl;
};


void cube(vector<cluster> &clusters, data &dataset, hashTable &hTable, vector<int> &r, int M, int k, int probes) {
  double Radius = initRadius(clusters, dataset);                //calculate radius
  int cl[dataset.getN()];                                       //for every point, store the centroid
  double dis[dataset.getN()];                                   //for every point, store the distance from centroid
  int finalized[dataset.getN()];                                //for every point, store 0 if i can change the values, 1 otherwise
  fill_n(cl, dataset.getN(), 0);
  fill_n(dis, dataset.getN(), -1);
  fill_n(finalized, dataset.getN(), 0);
  for(int n=0; n<N; n++) {                                                  //number of iterations(defined in headers.h)
    for(int c=0; c<clusters.size(); c++) {                                      //for every centroid
      int curCentroid = clusters[c].getIndex();
      dVector v = dataset.getdVector(curCentroid);                              //v = current centroid
      vector<int> g;
      int f;
      if(dataset.getMetric() == "euclidean") {
        vector<vector<double>> vs = hTable.getkVs();
        vector<double> ts = hTable.getkTs();
        g = euclideanGenerateG(v, k, vs, ts);                                   //generate a g vector
        f = fHushFunction(g, r, dataset.getN());                               //from that g -> generate fhushfunction
      }
      else if(dataset.getMetric() == "cosine") {
        vector<vector<double>> rs = hTable.getkRs();
        g = cosineGenerateG(v, k, rs);                                        //generate a g vector
        f = binaryToDecimalConverter(g);                                     //from that g -> generate f(key)
      }
      int originF = f;
      int countM = M, countProbes = probes;
      vector<int> uKeys = hTable.getUniqueKeys();
      while(countM && countProbes) {
        vector<hashNode> hn = hTable.returnSameBucket(f);         //get the bucket
        for(auto j=hn.begin(); j!=hn.end(); ++j) {
          if(finalized[j->getIndex()] == 0) {
            double Distance;                                          //calculate distance
            if(dataset.getMetric() == "@euclidean") {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(j->getIndex()).getCoordinates();
              Distance = euclideanDistance(x1, x2);
            }
            else if(dataset.getMetric() == "@cosine") {
              vector<double> x1 = v.getCoordinates();
              vector<double> x2 = dataset.getdVector(j->getIndex()).getCoordinates();
              Distance = cosineDistance(x1, x2);
            }
            if( (Distance < Radius) && ( (dis[j->getIndex()] == -1) || (dis[j->getIndex()] > Distance) ) ) {                                                //if distance is acceptable
                cl[j->getIndex()] = c;
                dis[j->getIndex()] = Distance;
              }
            countM--;
            if(countM == M)
              break;
          }
        }
        countProbes--;
        uKeys.erase(remove(uKeys.begin(), uKeys.end(), f), uKeys.end());          //erase the current f from used keys to avoid loop
        //hamming distance
        int minHammingDistance = 99999;
        int minKey;
        if(countProbes) {
          int minHammingDistance = 99999;
          int minKey;
          for(auto i=uKeys.begin(); i!=uKeys.end(); ++i) {        //for every used keys, find the nearest
            int hm = __builtin_popcount(originF^(*i));            //calculate hamming distance
            if(hm < minHammingDistance) {
              minHammingDistance = hm;
              minKey = *i;
            }
          }
          f = minKey;
        }
      }
    }
    for(int c=0; c<dataset.getN(); c++) {                                                     //set finalized = 1 for every point that we have calculate distance
      if(dis[c] != -1)
        finalized[c] = 1;
    }
    Radius = Radius * 2;
  }
  /*int count1=0;
  for(int i=0; i<dataset.getN(); i++) {
    if(finalized[i] == 1)
      count1++;
  }
  cout << "assigned: " << count1 << endl;*/
  for(int i=0; i<dataset.getN(); i++) {                                                       //we need some lloyd right here!
    double minDis = -1;
    int closestCentroid = -1;
    if(finalized[i] == 0) {
      for(int j=0; j<clusters.size(); j++) {
        double dis;
        vector<double> x1 = dataset.getdVector(i).getCoordinates();
        vector<double> x2 = clusters[j].getCentroid();
        if(dataset.getMetric() == "euclidean")
          dis = euclideanDistance(x1, x2);
        if(dataset.getMetric() == "cosine")
          dis = cosineDistance(x1, x2);
        if((dis < minDis) || (minDis == -1)) {
          minDis = dis;
          closestCentroid = j;
        }
      }
      cl[i] = closestCentroid;
      dis[i] = minDis;
      finalized[i] = 1;
    }
  }
  for(int c=0; c<dataset.getN(); c++) {                                                       //store point to the correct cluster
    clusters[cl[c]].addPoint(c);
  }
  //cout <<"Cube assignment completed." << endl;
};

//calculate and return the min distance (/2) between centroids
double initRadius(vector<cluster> &clusters, data &dataset) {
  double minDis = -1;
  for(int i=0; i<clusters.size(); i++) {
    for(int j=0; j<clusters.size(); j++) {
      if(i < j) {
        double dis;
        vector<double> x1 = clusters[i].getCentroid();
        vector<double> x2 = clusters[j].getCentroid();
        if(dataset.getMetric() == "euclidean")
          dis = euclideanDistance(x1, x2);
        if(dataset.getMetric() == "cosine")
          dis = cosineDistance(x1, x2);
        if((dis < minDis) || (minDis == -1))
          minDis = dis;
      }
    }
  }
  return minDis/2;
}
