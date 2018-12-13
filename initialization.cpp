#include "./hFiles/headers.h"
#include "./hFiles/generalFunctions.h"
#include "./hFiles/initialization.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

//select randomly centroids for the clusters(uniform distribution)
vector<int> RandomSelectionOfCentroids(data &dataset) {
  vector<int> centroids;
  int x = 0;
  while( x < numberOfClusters) {
    int c = UniformDistNumGenerator(0, dataset.getN());
    if(find(centroids.begin(), centroids.end(), c) == centroids.end()) {
        centroids.push_back(c);
        x++;
    }
  }
  //cout << "Random centroids were selected!" << endl;
  return centroids;
};

//select centroids based on k-means++
vector<int> kMeansPlusPlus(data &dataset) {
  vector<int> centroids;
  int datasetN = dataset.getN();
  string datasetMetric = dataset.getMetric();
  int c = UniformDistNumGenerator(0, datasetN-1);                               //choose random first centroid [0, 4999]
  centroids.push_back(c);
  int t = 1;
  while(t < numberOfClusters) {                                                 //for every cluster, find a centroid
    vector<double> partialSums;
    vector<int> points;
    double ps = 0.0;
    partialSums.push_back(ps);
    points.push_back(-1);
    for(int i=0; i<datasetN; i++) {                                             //for every point in dataset
      if(find(centroids.begin(), centroids.end(), i) == centroids.end()) {      //if this point isn't a centroid
        double minDis = -1;
        vector<double> x1 = dataset.getdVector(i).getCoordinates();
        for(int j=0; j<centroids.size(); j++) {                                 //find the min distance from centroids
          double dis;
          vector<double> x2 = dataset.getdVector(centroids[j]).getCoordinates();
          if(datasetMetric == "euclidean")
            dis = euclideanDistance(x1, x2);
          if(datasetMetric == "cosine")
            dis = cosineDistance(x1, x2);
          if((dis < minDis) || (minDis == -1))
            minDis = dis;
        }
        ps += pow(minDis, 2);
        partialSums.push_back(ps);                                              //store current partial summary
        points.push_back(i);                                                    //store current point
      }
    }
    double x = UniformDistNumGenerator(0.1, partialSums.back());                //some magic
    int l = binarySearch(partialSums, x);
    centroids.push_back(points[l]);
    t++;
  }
  //cout << "Centroids were selected with k-Means++!" << endl;
  return centroids;
};

//return the position of first value in vector which is bigger than x
int binarySearch(vector<double> &partialSums, double x) {
  vector<double>::iterator l;
  l = lower_bound(partialSums.begin(), partialSums.end(), x);
  return l - partialSums.begin();
};
