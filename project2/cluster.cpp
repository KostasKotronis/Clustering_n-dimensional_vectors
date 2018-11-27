#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"

/*~cluster~*/
/****************************************/
cluster::cluster(int i, data &dataset) {
  this->centroidIndex = i;
  this->centroid = dataset.getdVector(i).getCoordinates();
};

cluster::~cluster() {};

vector<double> cluster::getCentroid(void) {
  return this->centroid;
};

void cluster::printCentroid(void) {
  cout << "{ ";
  for(int i=0; i<this->centroid.size(); i++)
    cout << this->centroid[i] << " ";
  cout << "}" << endl;
};

void cluster::printPoints(void) {
  cout << "[ ";
  for(int i=0; i<this->points.size(); i++)
    cout << this->points[i] << " ";
  cout << "]" << endl;
};

void cluster::addPoint(int index) {
  this->points.push_back(index);
};

int cluster::getIndex(void) {
  return this->centroidIndex;
};

int cluster::getNumberOfPoints(void) {
  return this->points.size();
};
