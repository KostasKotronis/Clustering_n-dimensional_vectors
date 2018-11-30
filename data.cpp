#include "./hFiles/headers.h"
#include "./hFiles/data.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

/*~dVector~*/
/****************************************/
dVector::dVector() {
  this->id = -1;
  this->dimension = 0;
};

dVector::~dVector() {};

void dVector::setId(int i) {
  this->id = i;
};

int dVector::getDimension(void) {
  return this->dimension;
};

int dVector::getId(void) {
  return this->id;
};

//add a new coordinate to the vector
void dVector::addCoordinate(double c) {
  this->coordinates.push_back(c);
  this->dimension++;
};

//check if coordinates is empty
bool dVector::isCoordinatesEmpty(void) {
  if(this->coordinates.empty())
    return true;
  else
    return false;
};

//return <i>-th coordinate
double dVector::getCoordinate(int i) {
  return this->coordinates[i];
};

//return all coordinates
vector<double> dVector::getCoordinates(void) {
  return this->coordinates;
};

//print the vector
void dVector::printdVector(void) {
  cout << "id: " << this->id <<"  dimension: " << this->dimension << " coordinates: { ";
  for(auto i=this->coordinates.begin(); i!=this->coordinates.end(); ++i)
    cout << *i << " ";
  cout << "}" << endl;
};

/*~data~*/
/****************************************/
data::data() {
  this->n = 0;
};

data::~data() {};

void data::setMetric(string m) {
  this->metric = m;
};

//add a new coordinate to the vector
void data::addDVector(dVector &v) {
  this->dVectors.push_back(v);
  this->d = v.getDimension();
  this->n++;
};

//get metric
string data::getMetric(void) {
  return this->metric;
};

//return n
int data::getN(void) {
  return this->n;
};

//return dimension
int data::getD(void) {
  return this->d;
};

void data::printData(void) {
  cout << "metric: " << this->metric << " dimension: " << this->d << " n: " << n << endl;
  for(auto i=this->dVectors.begin(); i!=this->dVectors.end(); ++i) {
    i->printdVector();
  }
};

//return <i>-th dVector
dVector data::getdVector(int i) {
  return this->dVectors[i];
};

//linear search to find real nn
double data::linearSearchMinDistance(vector<double> &q) {
  double minDistance = -1;
  for(auto i=this->dVectors.begin(); i!=this->dVectors.end(); ++i) {
      if(this->metric == "euclidean") {
        vector<double> x = i->getCoordinates();
        double dis = euclideanDistance(x ,q);
        if(minDistance == -1)
          minDistance = dis;
        else if(dis < minDistance) {
          minDistance = dis;
        }
      }
      if(this->metric == "cosine") {
        vector<double> x = i->getCoordinates();
        double dis = cosineDistance(x ,q);
        if(minDistance == -1)
          minDistance = dis;
        else if(dis < minDistance) {
          minDistance = dis;
        }
      }
  }
  return minDistance;
};
