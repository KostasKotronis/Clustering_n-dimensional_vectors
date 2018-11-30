#include "./hFiles/headers.h"
#include "./hFiles/data.h"
#include "./hFiles/generalFunctions.h"
#include "./hFiles/initialization.h"
#include "./hFiles/assignment.h"
#include "./hFiles/evaluation.h"
#include "./hFiles/hashTable.h"
#include "./hFiles/update.h"

vector<string> supportedMetrics{"euclidean", "cosine"};

int numberOfClusters, numberOfHashFunctions, numberOfHashTables;
int maxUpdateIterations;
string initialization, assignment, update;
int N, MCUBE, PROBES;

int main(int argc, char* argv[]) {
  maxUpdateIterations = 25;
  initialization = "random";
  assignment = "lloyd";
  update = "kmeans";
  numberOfClusters = 20;
  numberOfHashFunctions = 4;
  numberOfHashTables = 5;

  N = 5;
  MCUBE = 100;
  PROBES = 30;
  int complete  = 0;
  int everyCombination  = 0;
  string inputFileName, outputFileName, metric, configurationFileName;
  data dataset;
  bool foundInputFile = false;
  bool foundOutputFile = false;
  bool foundMetric = false;
  bool foundConfigurationFile = false;
  for(int i=0; i<argc; i++) {           //get the arg
    if(!strcmp(argv[i], "-i")) {        //input file
      foundInputFile = true;
      inputFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-o")) {        //output file
      foundOutputFile = true;
      outputFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-d")) {        //metric
      foundMetric = true;
      metric = argv[++i];
    }
    if(!strcmp(argv[i], "-c")) {        //conf file
      foundConfigurationFile = true;
      configurationFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-complete")) {        //complete  for results
      complete = 1;
    }
    if(!strcmp(argv[i], "-all")) {        //complete  for results
      everyCombination = 1;
    }
  }
  if(!foundInputFile) {                                                                                                //if inputfile is not given as an argument
    cout << "Input file wasn't given in the program's arguments. Please define dataset's path:";
    cin >> inputFileName;
  }
  if(!foundOutputFile) {                                                                                                //if inputfile is not given as an argument
    cout << "Output file wasn't given in the program's arguments. Please define dataset's path:";
    cin >> outputFileName;
  }
  dataset = createDataset(inputFileName);
  if(foundConfigurationFile)                                                                                        //if configurationFile is given as an argument, store new values, otherwise keep default values
    readConfigurationFile(configurationFileName);
  if(!foundMetric) {                                                                                                //if metric is not given as an argument
    cout << "Metric wasn't given in the program's arguments. Default value is given.. (euclidean)" << endl;
    dataset.setMetric("euclidean");
  }
  else {
    if(find(supportedMetrics.begin(), supportedMetrics.end(), metric) == supportedMetrics.end()) {                        //check if metric is supported from the program
      cout << "Unknown metric. Default value is given.. (euclidean)" << endl;
      dataset.setMetric("euclidean");
    }
    else
      dataset.setMetric(metric);
  }
  ofstream outputFile;                                                          //stream class to write on files
  if(everyCombination) {
    cout << "fuck yeah! "<< endl;
    for(int i=0; i<2; i++){
      if(i == 0)
        initialization = "random";
      if(i == 1)
        initialization = "kmeans++";
      for(int j=0; j<3; j++) {
        if(j == 0)
          assignment = "lloyd";
        if(j == 1)
          assignment = "lsh";
        if(j == 2)
          assignment = "cube";
        for(int z=0; z<2; z++) {
          if(z == 0)
            update = "kmeans";
          if(z == 1)
            update = "pam";
          cout << "initialization: " << initialization << " assignment: " << assignment << " update: " << update << endl;
          //initialization
          vector<int> centroids;
          if(initialization == "random")
            centroids = RandomSelectionOfCentroids(dataset);
          if(initialization == "kmeans++")
            centroids = kMeansPlusPlus(dataset);
          vector<cluster> clusters;
          for(int i=0; i<centroids.size(); i++) {
            cluster c(centroids[i], dataset);
            clusters.push_back(c);
          }
          //assignment & update
          if(update == "kmeans")
            kmeans(dataset, assignment, clusters);
          if(update == "pam")
            pam(dataset, assignment, clusters);
          //
          outputFile.open(outputFileName, std::ios::app);
          if(!outputFile.is_open()) {                                           //in case of open crashed
            cout << "outputFile.open crashed. Program is terminating..." << endl;
            exit (EXIT_FAILURE);
          }
          outputFile << "Algorithm: " << initialization << "x" <<assignment << "x" << update << endl << "Metric: " << dataset.getMetric() << endl;
          for(int i=0; i<clusters.size(); i++) {
            if(update == "kmeans") {
              outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: (";
              vector<double> c = clusters[i].getCentroid();
              for(int j=0; j<clusters[i].getCentroid().size(); j++)
                outputFile << c[j] << " ";
              outputFile << ")}" << endl;
            }
            else
              outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: " << clusters[i].getIndex() << "}" << endl;
          }
          //evaluation
          silhouette(clusters, dataset, outputFileName);
          outputFile << "/* si=average s(p) of points in cluster i, stotal=average s(p) of points in dataset */" << endl;
          if(complete) {
            outputFile << "/* Additionally with command line parameter –complete */" << endl;
            for(int i=0; i<clusters.size(); i++) {
              outputFile << "CLUSTER-" << i << " {";
              vector<int> p = clusters[i].getPoints();
              for(int j=0; j<clusters[i].getPoints().size(); j++)
                outputFile << p[j] << " ";
              outputFile << "}" << endl;
            }
          }
          outputFile.close();
        }
      }
    }
  }
  else
  {
    cout << "initialization: " << initialization << " assignment: " << assignment << " update: " << update << endl;
    //initialization
    vector<int> centroids;
    if(initialization == "random")
      centroids = RandomSelectionOfCentroids(dataset);
    if(initialization == "kmeans++")
      centroids = kMeansPlusPlus(dataset);
    vector<cluster> clusters;
    for(int i=0; i<centroids.size(); i++) {
      cluster c(centroids[i], dataset);
      clusters.push_back(c);
    }
    //assignment & update
    if(update == "kmeans")
      kmeans(dataset, assignment, clusters);
    if(update == "pam")
      pam(dataset, assignment, clusters);
    //
    outputFile.open(outputFileName, std::ios::app);
    if(!outputFile.is_open()) {                                           //in case of open crashed
      cout << "outputFile.open crashed. Program is terminating..." << endl;
      exit (EXIT_FAILURE);
    }
    outputFile << "Algorithm: " << initialization << "x" <<assignment << "x" << update << endl << "Metric: " << dataset.getMetric() << endl;
    for(int i=0; i<clusters.size(); i++) {
      if(update == "kmeans") {
        outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: (";
        vector<double> c = clusters[i].getCentroid();
        for(int j=0; j<clusters[i].getCentroid().size(); j++)
          outputFile << c[j] << " ";
        outputFile << ")}" << endl;
      }
      else
        outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: " << clusters[i].getIndex() << "}" << endl;
    }
    //evaluation
    silhouette(clusters, dataset, outputFileName);
    outputFile << "/* si=average s(p) of points in cluster i, stotal=average s(p) of points in dataset */" << endl;
    if(complete) {
      outputFile << "/* Additionally with command line parameter –complete */" << endl;
      for(int i=0; i<clusters.size(); i++) {
        outputFile << "CLUSTER-" << i << " {";
        vector<int> p = clusters[i].getPoints();
        for(int j=0; j<clusters[i].getPoints().size(); j++)
          outputFile << p[j] << " ";
        outputFile << "}" << endl;
      }
    }
    outputFile.close();
  }
  cout << "Program is terminating... C ya!" << endl;
  return 0;
}
