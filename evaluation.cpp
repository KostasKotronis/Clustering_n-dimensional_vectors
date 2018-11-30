#include "./hFiles/headers.h"
#include "./hFiles/evaluation.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

//evaluate a cluster: Compute average s (i) over all i in some cluster
void silhouette(vector<cluster> clusters, data &dataset, string &outputFileName) {
  double sTotal = 0;
  ofstream outputFile;                                          //stream class to write on files
  outputFile.open(outputFileName, std::ios::app);
  if(!outputFile.is_open()) {                                   //in case of open crashed
    cout << "outputFile.open crashed. Program is terminating..." << endl;
    exit (EXIT_FAILURE);
  }
  outputFile << "Silhouette: [";
  for(int i=0; i<clusters.size(); i++) {                                        //for every cluster
    double si = 0;
    for(int j=0; j<clusters[i].getPoints().size(); j++) {                       //for every point in cluster
      double a = 0;
      int index = clusters[i].getPoint(j);
      vector<double> x1 = dataset.getdVector(index).getCoordinates();
      vector<double> x2 = clusters[i].getCentroid();                            //get distance of centroid (a)
      if(dataset.getMetric() == "euclidean")
        a = euclideanDistance(x1, x2);
      if(dataset.getMetric() == "cosine")
        a = cosineDistance(x1, x2);
      double b = -1;
      for(int z=0; z<clusters.size(); z++) {                                    //get distance of second closest centroid (b)
        if(i != z) {
          double bb;
          vector<double> x1 = dataset.getdVector(index).getCoordinates();
          vector<double> x2 = clusters[z].getCentroid();
          if(dataset.getMetric() == "euclidean")
            bb = euclideanDistance(x1, x2);
          if(dataset.getMetric() == "cosine")
            bb = cosineDistance(x1, x2);
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
      si += s;
    }
    si = si /clusters[i].getPoints().size();                                    //calculate average si for every cluster
    //cout << i << ") " << si << endl;
    outputFile << si <<",";
    sTotal += si;
  }
  sTotal = sTotal /clusters.size();                                             //calculate average s for the dataset
  outputFile << sTotal <<"]" << endl;
  outputFile.close();
  //cout << "stotal: " << sTotal << endl;
};
