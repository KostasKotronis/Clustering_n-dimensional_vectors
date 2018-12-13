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
int maxUpdateIterations, maxRangeSearchIterations;
string initialization, assignment, update;
int MCube, probes, W;

int main(int argc, char* argv[]) {
  clock_t start = clock();
  numberOfClusters = 20;                                                        //give default values to global variables
  numberOfHashFunctions = 4;
  numberOfHashTables = 5;
  maxUpdateIterations = 25;
  maxRangeSearchIterations = 25;
  initialization = "random";
  assignment = "lloyd";
  update = "kmeans";
  MCube = 100;
  probes = 30;
  W = 2;
  int complete  = 0;
  int everyCombination  = 0;
  string inputFileName, outputFileName, metric, configurationFileName;
  data dataset;
  bool foundInputFile = false;
  bool foundOutputFile = false;
  bool foundMetric = false;
  bool foundConfigurationFile = false;
  for(int i=0; i<argc; i++) {                                                   //read arguments
    if(!strcmp(argv[i], "-i")) {                                                //input file
      foundInputFile = true;
      inputFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-o")) {                                                //output file
      foundOutputFile = true;
      outputFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-d")) {                                                //metric
      foundMetric = true;
      metric = argv[++i];
    }
    if(!strcmp(argv[i], "-c")) {                                                //conf file
      foundConfigurationFile = true;
      configurationFileName = argv[++i];
    }
    if(!strcmp(argv[i], "-complete")) {                                         //complete  for results
      complete = 1;
    }
    if(!strcmp(argv[i], "-all")) {                                              //run every combination
      everyCombination = 1;
    }
    if(!strcmp(argv[i], "-I")) {                                                //initialization algorithm
      initialization = argv[++i];
    }
    if(!strcmp(argv[i], "-A")) {                                                //assignment algorithm
      assignment = argv[++i];
    }
    if(!strcmp(argv[i], "-U")) {                                                //update algorithm
      update = argv[++i];
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
  if(foundConfigurationFile)                                                                                            //if configurationFile is given as an argument, store new values, otherwise keep default values
    readConfigurationFile(configurationFileName);
  if(!foundMetric) {                                                                                                    //if metric is not given as an argument
    cout << "Metric wasn't given in the program's arguments. Default value is given.. (euclidean)" << endl;
    dataset.setMetric("euclidean");
  }
  else {
    if(find(supportedMetrics.begin(), supportedMetrics.end(), metric) == supportedMetrics.end()) {                      //check if metric is supported from the program
      cout << "Unknown metric. Default value is given.. (euclidean)" << endl;
      dataset.setMetric("euclidean");
    }
    else
      dataset.setMetric(metric);
  }
  ofstream outputFile;                                                          //stream class to write on files
  if(everyCombination) {
    cout << "Let's try everything! "<< endl;
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
          clock_t s = clock();
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
          cout << "   initialization completed!" << endl;
          //assignment & update
          if(update == "kmeans")
            kmeans(dataset, assignment, clusters);
          if(update == "pam")
            pam(dataset, assignment, clusters);
          cout << "   assignment & update completed!" << endl;
          outputFile.open(outputFileName, std::ios::app);
          if(!outputFile.is_open()) {                                           //in case of open crashed
            cout << "outputFile.open crashed. Program is terminating..." << endl;
            exit (EXIT_FAILURE);
          }
          //evaluation
          outputFile << "Algorithm: " << initialization << "x" <<assignment << "x" << update << endl << "Metric: " << dataset.getMetric() << endl;
          for(int i=0; i<numberOfClusters; i++) {
            if(update == "kmeans") {
              outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: (";
              vector<double> c = clusters[i].getCentroid();
              for(int j=0; j<clusters[i].getCentroid().size(); j++)
                outputFile << c[j] << " ";
              outputFile << ")}" << endl;
            }
            else
              outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: " << dataset.getdVectorId(clusters[i].getIndex()) << "}" << endl;
          }
          outputFile.close();
          silhouette(clusters, dataset, outputFileName);
          if(complete) {
            outputFile.open(outputFileName, std::ios::app);
            if(!outputFile.is_open()) {                                         //in case of open crashed
              cout << "outputFile.open crashed. Program is terminating..." << endl;
              exit (EXIT_FAILURE);
            }
            for(int i=0; i<numberOfClusters; i++) {
              outputFile << "CLUSTER-" << i << " {";
              vector<int> p = clusters[i].getPoints();
              for(int j=0; j<clusters[i].getPoints().size(); j++)
                outputFile << dataset.getdVectorId(p[j]) << " ";
              outputFile << "}" << endl;
            }
            outputFile.close();
          }
          cout << "   evaluation completed!" << endl;
          clock_t f = clock();
          //cout << "time: " << (unsigned int)(f - s) / (double CLOCKS_PER_SEC) <<" (s)" << endl;
        }
      }
    }
  }
  else
  {
    cout << "initialization: " << initialization << " assignment: " << assignment << " update: " << update << endl;
    clock_t s = clock();
    //initialization
    vector<int> centroids;
    if(initialization == "random")
      centroids = RandomSelectionOfCentroids(dataset);
    else if(initialization == "kmeans++")
      centroids = kMeansPlusPlus(dataset);
    else {
      cout << "Unknown initialization algorithm. Program is terminating..." << endl;
      exit (EXIT_FAILURE);
    }
    vector<cluster> clusters;
    for(int i=0; i<centroids.size(); i++) {
      cluster c(centroids[i], dataset);
      clusters.push_back(c);
    }
    cout << "   initialization completed!" << endl;
    //assignment & update
    if(update == "kmeans")
      kmeans(dataset, assignment, clusters);
    else if(update == "pam")
      pam(dataset, assignment, clusters);
    else {
        cout << "Unknown update algorithm. Program is terminating..." << endl;
        exit (EXIT_FAILURE);
    }
    cout << "   assignment & update completed!" << endl;

    outputFile.open(outputFileName, std::ios::app);
    if(!outputFile.is_open()) {                                                 //in case of open crashed
      cout << "outputFile.open crashed. Program is terminating..." << endl;
      exit (EXIT_FAILURE);
    }
    //evaluation
    outputFile << "Algorithm: " << initialization << "x" <<assignment << "x" << update << endl << "Metric: " << dataset.getMetric() << endl;
    for(int i=0; i<numberOfClusters; i++) {
      if(update == "kmeans") {
        outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: (";
        vector<double> c = clusters[i].getCentroid();
        for(int j=0; j<clusters[i].getCentroid().size(); j++)
          outputFile << c[j] << " ";
        outputFile << ")}" << endl;
      }
      else
        outputFile << "CLUSTER-" << i << " {size: " << clusters[i].getNumberOfPoints() << ", centroid: " << dataset.getdVectorId(clusters[i].getIndex()) << "}" << endl;
    }
    outputFile.close();
    silhouette(clusters, dataset, outputFileName);
    if(complete) {
      outputFile.open(outputFileName, std::ios::app);
      if(!outputFile.is_open()) {                                               //in case of open crashed
        cout << "outputFile.open crashed. Program is terminating..." << endl;
        exit (EXIT_FAILURE);
      }
      for(int i=0; i<numberOfClusters; i++) {
        outputFile << "CLUSTER-" << i << " {";
        vector<int> p = clusters[i].getPoints();
        for(int j=0; j<clusters[i].getPoints().size(); j++)
          outputFile << dataset.getdVectorId(p[j]) << " ";
        outputFile << "}" << endl;
      }
      outputFile.close();
    }
    cout << "   evaluation completed!" << endl;
    clock_t f = clock();
    //cout << "time: " << (unsigned int)(f - s) / (double CLOCKS_PER_SEC) <<" (s)" << endl;
  }
  clock_t fin = clock();
  //cout << "total time: " << (unsigned int)(fin - start) / (double CLOCKS_PER_SEC) <<" (s)" << endl;
  cout << "Program is terminating... C ya!" << endl;
  return 0;
};
