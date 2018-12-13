#include "./hFiles/headers.h"
#include "./hFiles/cluster.h"


dVector parseLine(string &line) {
  dVector v;
  stringstream ln(line);
  string token;
  char delim = ',';                                                             //set delimeter
  bool isId = true;
  while (getline(ln, token, delim)) {
    if(isId) {                                                                  //store id
      int id = stoi(token);
      v.setId(id);
      isId = false;
    }
    else {                                                                      //store coordinates
      double c = stof(token);
      v.addCoordinate(c);
    }
  }
  return v;
};

//read inputFile line by line
//for every line, creat a dVector. then store the dVector in a list
//In the end, returns the list with the dVectors
data createDataset(string &inputFileName) {
  ifstream inputFile;
  string line;
  data d;
  inputFile.open(inputFileName);                                                //opening input file...
  if(!inputFile.is_open()) {                                                    //in case of open crashed
    cout << "inputFile.open crashed. Program is terminating..." << endl;
    exit (EXIT_FAILURE);
  }
  bool isId = true;
  while( getline(inputFile, line) ) {                                           //read inputfile line bu line
    dVector v = parseLine(line);
    d.addDVector(v);
    //v.printdVector();
  }
  inputFile.close();
  return d;
};

//read configuration file line by line
void readConfigurationFile(string &configurationFileName) {
  ifstream configurationFile;
  configurationFile.open(configurationFileName);                                //opening configuration file...
  if(configurationFile.is_open()) {                                             //if file can be opened, get new values, otherwise, keep default
    string line;
    while( getline(configurationFile, line) ) {                                 //read configurationfile line by line
      istringstream ln(line);
      string word;
      ln >> word;
      string n;
      if(word == "number_of_clusters:") {                                       //number of clusters
        string n;
        ln >> n;
        numberOfClusters = stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "number_of_hash_functions:") {                                 //number of hash functions
        string n;
        ln >> n;
        numberOfHashFunctions = stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "number_of_hash_tables:") {                                    //number of hash tables
        string n;
        ln >> n;
        numberOfHashTables = stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "maxUpdateIterations:") {                                      //max number of iterations in update
        string n;
        ln >> n;
        maxUpdateIterations = stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "maxRangeSearchIterations:") {                                 //max number of iterations in range search
        string n;
        ln >> n;
        maxRangeSearchIterations = stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "M_CUBE:") {                                                   //M constant for cube
        string n;
        ln >> n;
        MCube= stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "PROBES_CUBE:") {                                              //#probes constant for cube
        string n;
        ln >> n;
        probes= stoi(n);
        //cout << word << " " << n << endl;
      }
      if(word == "W:") {                                                        //W
        string n;
        ln >> n;
        W = stoi(n);
        //cout << word << " " << n << endl;
      }
    }
  }
};

//return one uniform dist number in a range
double UniformDistNumGenerator(double minimum, double maximum) {
  random_device rd;                                                             //Will be used to obtain a seed for the random number engine
  mt19937 gen(rd());                                                            //Standard mersenne_twister_engine seeded with rd()
  uniform_real_distribution<double> dis(minimum, maximum);
  return dis(gen);
};
