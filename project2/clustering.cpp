#include "./hFiles/headers.h"
#include "./hFiles/data.h"
#include "./hFiles/generalFunctions.h"
#include "./hFiles/initialization.h"
#include "./hFiles/assignment.h"
#include "./hFiles/hashTable.h"

vector<string> supportedMetrics{"euclidean", "cosine"};

int main(int argc, char* argv[]) {
  string inputFileName, metric, configurationFileName;
  data dataset;
  bool foundInputFile = false;
  bool foundMetric = false;
  bool foundConfigurationFile = false;
  for(int i=0; i<argc; i++) {           //get the arg
    if(!strcmp(argv[i], "-i")) {        //input file
      foundInputFile = true;
      inputFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-d")) {        //metric
      foundMetric = true;
      metric = argv[++i];
    }
    if(!strcmp(argv[i], "-c")) {        //conf file
      foundConfigurationFile = true;
      configurationFileName = argv[++i];
    }
  }
  if(!foundInputFile) {                                                                                                //if inputfile is not given as an argument
    cout << "Input file wasn't given in the program's arguments. Please define dataset's path:";
    cin >> inputFileName;
  }
  dataset = createDataset(inputFileName);
  int numberOfClusters = 13, numberOfHushFunctions = 4, numberOfHashTables = 5;
  if(foundConfigurationFile)                                                                                        //if configurationFile is given as an argument, store new values, otherwise keep default values
    readConfigurationFile(configurationFileName, numberOfClusters, numberOfHushFunctions, numberOfHashTables);
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
  //initialization
  cout << endl << "-->step 1: initialization" << endl << endl;
  vector<int> centroids;
  centroids = RandomSelectionOfCentroids(numberOfClusters, dataset);
  //centroids = kMeansPlusPlus(numberOfClusters, dataset);
  vector<cluster> clusters;
  for(int i=0; i<centroids.size(); i++) {
    cluster c(centroids[i], dataset);
    clusters.push_back(c);
  }
  //assignment
  cout << endl << "-->step 1: assignment" << endl << endl;
  //lloyd(clusters, dataset);

  /*vector<hashTable> hashTables;
  int L = numberOfHashTables;
  int k = numberOfHushFunctions;
  vector<int> r = kRandomNum(k, 20);
  string algorithm = "lsh";
  for(int i=0; i<L; i++) {                                                                          //creates L hushTables
   hashTable hTable(dataset, r, k, algorithm);
   hashTables.push_back(hTable);
  }
  cout << "hashTables are ready!" << endl;
  LSH(clusters, dataset, hashTables, r, L, k);*/

  int k = floor(log2(dataset.getN()));
  vector<int> r = kRandomNum(k, 20);
  string algorithm = "cube";
  hashTable hTable(dataset, r, k, algorithm);
  //hTable.printTable();
  //getchar();
  cout << "hashTable is ready!" << endl;
  cube(clusters, dataset, hTable, r, MCUBE, k, PROBES);
  for(int i=0; i<numberOfClusters; i++) {
    cout <<i << ". centroid: "<< clusters[i].getIndex() << " nop: " << clusters[i].getNumberOfPoints() << endl;
  }


  return 0;
}
