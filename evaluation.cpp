#include "./hFiles/headers.h"
#include "./hFiles/evaluation.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

//evaluate a cluster: Compute average s (i) over all i in some cluster
void silhouette(vector<cluster> clusters, data &dataset, string &outputFileName) {
  double sTotal = 0;
  ofstream outputFile;                                                          //stream class to write on files
  outputFile.open(outputFileName, std::ios::app);
  if(!outputFile.is_open()) {                                                   //in case of open crashed
    cout << "outputFile.open crashed. Program is terminating..." << endl;
    exit (EXIT_FAILURE);
  }
  outputFile << "Silhouette: [";
  for(int i=0; i<numberOfClusters; i++) {                                       //for every cluster
    int numberOfPoints = clusters[i].getNumberOfPoints();
    double si = 0;
    vector<double> centroid = clusters[i].getCentroid();                        //get distance of centroid (a
    for(int j=0; j<numberOfPoints; j++) {                                       //for every point in cluster
      double a = 0;
      int index = clusters[i].getPoint(j);
      vector<double> p = dataset.getdVector(index).getCoordinates();
      if(dataset.getMetric() == "euclidean")
        a = euclideanDistance(p, centroid);
      if(dataset.getMetric() == "cosine")
        a = cosineDistance(p, centroid);
      double b = -1;
      for(int z=0; z<numberOfClusters; z++) {                                   //get distance of second closest centroid (b)
        if(i != z) {
          double bb;
          vector<double> centroid2 = clusters[z].getCentroid();
          if(dataset.getMetric() == "euclidean")
            bb = euclideanDistance(p, centroid2);
          if(dataset.getMetric() == "cosine")
            bb = cosineDistance(p, centroid2);
          if((bb < b) || (b == -1))
              b = bb;
        }
      }
      double s;                                                                 //calculate s for every point
      if (b > a)
        s = (b - a) / b;
      else
        s = (b - a) / a;
      //cout << index << ". " << s << endl;
      if(!isnan(s))
        si += s;
    }
    if(numberOfPoints)
      si = si /numberOfPoints;                                                  //calculate average si for every cluster
    else
      si = 0;
    //cout << i << ") " << si << endl;
    outputFile << si <<",";
    sTotal += si;
  }
  sTotal = sTotal / numberOfClusters;                                           //calculate average s for the dataset
  //cout << "sil: " << sTotal << endl;
  outputFile << sTotal <<"]" << endl;
  outputFile.close();
  //cout << "stotal: " << sTotal << endl;
};
