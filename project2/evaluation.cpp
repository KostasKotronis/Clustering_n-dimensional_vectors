#include "./hFiles/headers.h"
#include "./hFiles/evaluation.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"

int silhouette(vector<cluster> clusters, data &dataset) {
  double ss = 0;
  for (int i=0; i<dataset.getN(); i++) {
    double a=0, b=-1, s;
    for (int j=0; j<clusters.size(); j++) {
      int belongs = clusters[j].isMember(i);
      if(belongs) {
        vector<double> x1 = dataset.getdVector(i).getCoordinates();
        vector<double> x2 = clusters[j].getCentroid();
        if(dataset.getMetric() == "euclidean")
          a = euclideanDistance(x1, x2);
        if(dataset.getMetric() == "cosine")
          a = cosineDistance(x1, x2);
      }
      else {
        double bb;
        vector<double> x1 = dataset.getdVector(i).getCoordinates();
        vector<double> x2 = clusters[j].getCentroid();
        if(dataset.getMetric() == "euclidean")
          bb = euclideanDistance(x1, x2);
        if(dataset.getMetric() == "cosine")
          bb = cosineDistance(x1, x2);
        if((bb < b) || (b == -1))
            b = bb;
      }
    }
    if (b > a)
      s = (b - a) / b;
    else
      s = (b - a) / a;
      cout << i << ". " << s << endl;
    ss +=s;
  }
  ss = ss /dataset.getN();
  cout << "meso s:" << ss << endl;
}
