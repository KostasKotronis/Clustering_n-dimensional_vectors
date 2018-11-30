#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

/*~cluster~*/
/****************************************/
//constructor
cluster::cluster(int i, data &dataset) {
  this->centroidIndex = i;
  this->centroid = dataset.getdVector(i).getCoordinates();
};

//constructor
cluster::cluster() {
  this->centroidIndex = -1;
};

//destructor
cluster::~cluster() {};

//get coordinates of centroid
vector<double> cluster::getCentroid(void) {
  return this->centroid;
};

//set coordinates of centroid
void cluster::setCentroid(vector<double> &c) {
  this->centroid = c;
};

//print coordinates of centroid
void cluster::printCentroid(void) {
  cout << "{ ";
  for(int i=0; i<this->centroid.size(); i++)
    cout << this->centroid[i] << " ";
  cout << "}" << endl;
};

//get the point with index p
int cluster::getPoint(int p) {
  return this->points[p];
};

//get vector of points
vector<int> cluster::getPoints(void) {
  return this->points;
};

//set vector of points
void cluster::setPoints(vector<int> &p) {
  this->points = p;
};

// print vector of points
void cluster::printPoints(void) {
  cout << "[ ";
  for(int i=0; i<this->points.size(); i++)
    cout << this->points[i] << " ";
  cout << "]" << endl;
};

//empty vector of points
void cluster::clearPoints(void) {
  this->points.clear();
};

//add point
void cluster::addPoint(int index) {
  this->points.push_back(index);
};

//get index of centroid in dataset
int cluster::getIndex(void) {
  return this->centroidIndex;
};

//return number of points in cluster
int cluster::getNumberOfPoints(void) {
  return this->points.size();
};

//copy coordinates of centroid
void cluster::createClone(cluster &clone) {
  clone.setCentroid(this->centroid);
  clone.setPoints(this->points);
};

//return 1 if current cluster is equal to c cluster
int cluster::equalCluster(cluster &c) {
  if(this->centroid == c.getCentroid())
    return 1;
  else
    return 0;
};

//update centroid via kmeans
void cluster::kMeansUpdateCentroid(data &dataset) {
  for(int i=0; i<this->centroid.size(); i++) {
    double sum = 0;
    for(int j=0; j<this->points.size(); j++) {
      int index = this->points[j];
      sum += dataset.getdVector(index).getCoordinate(i);
    }
    sum = sum / (this->points.size());
    this->centroid[i] = sum;
  }
};

//update centroid via pam
void cluster::pamUpdateCentroid(data &dataset) {
  int q = 0;
  double minSum = -1;
  int bestCentroid = -1;
  for(int i=0; i<this->points.size(); i++) {
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

//return 1 if index belongs to the cluster, otherwise 0
int cluster::isMember(int index) {
  if(find(this->points.begin(), this->points.end(), index) == this->points.end())
    return 0;
  else
    return 1;
};
