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
  int centroidSize = this->centroid.size();
  int pointsSize = this->points.size();
  for(int i=0; i<centroidSize; i++) {
    double sum = 0;
    for(int j=0; j<pointsSize; j++) {
      int index = this->points[j];
      vector<double> p = dataset.getdVector(index).getCoordinates();
      sum += p[i];
    }
    sum = sum / pointsSize;
    this->centroid[i] = sum;
  }
};

//update centroid via pam
void cluster::pamUpdateCentroid(data &dataset) {
  string datasetMetric = dataset.getMetric();
  double minSum = -1;
  int bestCentroid = -1;
  int pointsSize = this->points.size();
  for(int i=0; i<pointsSize; i++) {
    double sum = 0;
    int index1 = this->points[i];
    int flag = 0;
    int j=0;
    while((j<pointsSize) && flag == 0) {
      double dis;
      int index2 = this->points[j];
      if(datasetMetric == "euclidean")
        dis = dataset.euclideanDis(index1, index2);
      if(datasetMetric == "cosine")
        dis = dataset.cosineDis(index1, index2);
      sum += dis;
      if((sum > minSum) && (minSum != -1))
        flag = 1;
      j++;
    }
    if((sum < minSum) || (minSum == -1)) {
      minSum = sum;
      bestCentroid = index1;
    }
  }
  this->centroid = dataset.getdVector(bestCentroid).getCoordinates();
  this->centroidIndex = bestCentroid;
};

//return 1 if index belongs to the cluster, otherwise 0
int cluster::isMember(int index) {
  if(find(this->points.begin(), this->points.end(), index) == this->points.end())
    return 0;
  else
    return 1;
};
