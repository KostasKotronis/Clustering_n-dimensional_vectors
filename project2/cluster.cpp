#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

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
  if( (this->centroid == c.getCentroid())  )                  //&& (this->points == c.getPoints())
    return 1;
  else
    return 0;
};

void cluster::kMeansUpdateCentroid(data &dataset) {
  for(int i=0; i<this->centroid.size(); i++) {      // {1,2,... 204}
    double sum = 0;
    for(int j=0; j<this->points.size(); j++) {       //{index1, index2,...}
      int index = this->points[j];
      sum += dataset.getdVector(index).getCoordinate(i);
    }
    sum = sum / (this->points.size());
    this->centroid[i] = sum;
  }
};

void cluster::pamUpdateCentroid(data &dataset) {
  int q = 0;
  double minSum = -1;
  int bestCentroid = -1;
  for(int i=0; i<this->points.size(); i++) {       //{index1, index2,...}
    double sum = 0;
    int index1 = this->points[i];
    for(int j=0; j<this->points.size(); j++) {
      q++;
      double dis;
      int index2 = this->points[j];
      vector<double> x1 = dataset.getdVector(index1).getCoordinates();
      vector<double> x2 = dataset.getdVector(index2).getCoordinates();
      if(dataset.getMetric() == "euclidean")
        dis = euclideanDistance(x1, x2);
      if(dataset.getMetric() == "cosine")
        dis = cosineDistance(x1, x2);
      sum += dis;
    }
    if((sum < minSum) || (minSum == -1)) {
      minSum = sum;
      bestCentroid = index1;
    }
    this->centroid.clear();
    this->centroid = dataset.getdVector(bestCentroid).getCoordinates();
    this->centroidIndex = bestCentroid;
  }
  cout << q << endl;
};

int cluster::isMember(int index) {
  if(find(this->points.begin(), this->points.end(), index) == this->points.end())
    return 0;
  else
    return 1;
};
