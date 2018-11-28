#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"

/*~cluster~*/
/****************************************/
cluster::cluster(int i, data &dataset) {
  this->centroidIndex = i;
  this->centroid = dataset.getdVector(i).getCoordinates();
};

cluster::cluster() {
  this->centroidIndex = -1;
};

cluster::~cluster() {};

vector<double> cluster::getCentroid(void) {
  return this->centroid;
};

void cluster::setCentroid(vector<double> &c) {
  this->centroid = c;
};

void cluster::printCentroid(void) {
  cout << "{ ";
  for(int i=0; i<10; i++)
    cout << this->centroid[i] << " ";
  cout << "}" << endl;
};

vector<int> cluster::getPoints(void) {
  return this->points;
};

void cluster::setPoints(vector<int> &p) {
  this->points = p;
};

void cluster::printPoints(void) {
  cout << "[ ";
  for(int i=0; i<this->points.size(); i++)
    cout << this->points[i] << " ";
  cout << "]" << endl;
};

void cluster::clearPoints(void) {
  this->points.clear();
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

void cluster::createClone(cluster &clone) {
  clone.setCentroid(this->centroid);
  clone.setPoints(this->points);
};

int cluster::equalCluster(cluster &c) {
  if( (this->centroid == c.getCentroid()) && (this->points == c.getPoints()) )
    return 1;
  else
    return 0;
};

void cluster::updateCentroid(data &dataset) {
  for(int i=0; i<this->centroid.size(); i++) {      // {1,2,... 204}
    double sum = 0;
    for(int j=0; j<this->points.size(); j++) {       //{index1, index2,...}
      int index = this->points[j];
      sum += dataset.getdVector(index).getCoordinate(i);
    }
    sum = sum / (this->points.size());
    this->centroid[i] = sum;
  }
}
