#include "./hFiles/headers.h"
#include "./hFiles/generalFunctions.h"

/*~euclidean~*/
/****************************************/

//calculate h
int euclideanProject(dVector &p, vector<double> &v, double &t) {
  int d = p.getDimension();
  double sp = 0;                                                                //scalar product v*p
  for(int i=0; i<d; i++)
      sp += v[i] * p.getCoordinate(i);
  int h = floor((sp+t)/W);
  return h;
};

//generate a g vector with k*h
vector<int> euclideanGenerateG(dVector &p, int k, vector<vector<double>> &v, vector<double> &t) {
  vector<int> g;
  for(int i=0; i<k; i++) {
    int h = euclideanProject(p, v[i], t[i]);
    g.push_back(h);
  }
  return g;
};

//only for cube
//generate the g vector and convert him from binary to decimal ->key!
int cubeGenerateKey(dVector &p, int k, vector<vector<double>> &v, vector<double> &t) {
  vector<int> g;
  vector<vector<double>> usedV;
  for(int i=0; i<k; i++) {
    int h = euclideanProject(p, v[i], t[i]);
    int f = ((h % 2) + 2) % 2;
    g.push_back(f);
  }
  int key = binaryToDecimalConverter(g);
  return key;
};

//only for lsh
//calculate the f to figure out in which bucket the point should go->key!
int fHushFunction(vector<int> &g, vector<int> &r, int n) {
  int k = g.size();
  unsigned int M = pow(2,32) - 5;
  unsigned int f = 0;
  for(int i=0; i<k; i++) {
    long long fi = r[i]*g[i];
    f += ((fi % M) + M) % M;
  }
  int TableSize = n/4;
  f = ((f % TableSize) + TableSize) % TableSize;
  return f;
};

//calculate the euclidean distance between 2 vectors
double euclideanDistance(vector<double> &x, vector<double> &y) {
  double distance = 0;
  for(int i=0; i<x.size(); i++)
    distance += pow(x[i]-y[i], 2);
  distance = pow(distance, 0.5);
    return distance;
};
