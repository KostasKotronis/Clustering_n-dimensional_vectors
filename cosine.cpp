#include "./hFiles/headers.h"
#include "./hFiles/generalFunctions.h"

/*~cosine~*/
/****************************************/
//calculate h
int cosineProject(dVector &p, vector<double> &r) {
  int d = p.getDimension();
  double sp = 0;                                                                //scalar product v*p
  for(int i=0; i<d; i++)
      sp += r[i] * p.getCoordinate(i);
  if(sp >= 0)
    return 1;
  else
    return 0;
};

//generate a g vector with k*h
vector<int> cosineGenerateG(dVector &p, int k, vector<vector<double>> &kRs) {
  vector<int> g;
  for(int i=0; i<k; i++) {
    int h = cosineProject(p, kRs[i]);
    g.push_back(h);
  }
  return g;
};

//calculate the cosine distance between 2 vectors
double cosineDistance(vector<double> &x, vector<double> &y) {
  double distance = 0;
  double sp = 0;
  double xm = 0;
  double ym = 0;
  for(int i=0; i<x.size(); i++) {                                               //scalar product v*p
      xm += pow(x[i],2);
      ym += pow(y[i],2);
      sp += x[i] * y[i];
  }
  xm = pow(xm, 0.5);
  ym = pow(ym, 0.5);
  distance = 1 - (sp/(xm*ym));
  return distance;
};
