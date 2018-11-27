#include "./hFiles/headers.h"
#include "./hFiles/data.h"
//read inputFile line by line
//for every line, creat a dVector. then store the dVector in a list
//In the end, returns the list with the dVectors
/*data createDataset(string &inputFileName) {
  data dt;
  list <dVector> l;
  string line, metric;
  int d,id = 0;
  bool foundMetric = false;
  ifstream inputFile;
  inputFile.open(inputFileName);                        //opening input file...
  if(!inputFile.is_open()) {                            //in case of open crashed
    cout << "inputFile.open crashed. Program is terminating..." << endl;
    exit (EXIT_FAILURE);
  }
  while( getline(inputFile, line) ) {                    //read inputfile line bu line
    dVector v;
    stringstream ln(line);
    if(line[0] == '@') {                                 //get metric
      string l;
      ln >> l;
      dt.setMetric(l);
      foundMetric = true;
    }
    else {                                                //get points
      double c;
      while (ln >> c)
        v.addCoordinate(c);

      if(!v.isCoordinatesEmpty()) {
        v.setId(id++);
        dt.addDVector(v);                                  //store vector in the list
        //..= sizeof(id) + sizeof(dimension)+ sizeof(coordinates)
        memoryOfDataset += (sizeof(int) + sizeof(int) + (sizeof(double)*v.getDimension()));
        //v.printdVector();
      }
    }
  }
  if(!foundMetric) {                                       //set default value in metric
    cout << "metric function wasnt detected in inputFile. Default value is given.." << endl;
    dt.setMetric("@euclidean");
  }
  //size of metric
  memoryOfDataset += dt.getMetric().size();
  inputFile.close();
  return dt;
};*/

//return one normal dis number
double NormalDistNumGenerator(void) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();     // construct a trivial random generator engine from a time-based seed
  default_random_engine generator(seed);
  normal_distribution<double> distribution(MEAN,STDDEV);
  return distribution(generator);
};

//return one uniform dist number
double UniformDistNumGenerator(void) {
  random_device rd;                                     //Will be used to obtain a seed for the random number engine
  mt19937 gen(rd());                                    //Standard mersenne_twister_engine seeded with rd()
  uniform_real_distribution<double> dis(ZERO, W);
  return dis(gen);
};

//return k random numbers in range
vector<int> kRandomNum(int k, int maxNum) {
  vector<int> r;
  srand((unsigned)time(0));
  for(int i=0; i<k; i++)
    r.push_back((rand()%maxNum));
  return r;
};

//convert given binary to decimal number
int binaryToDecimalConverter(vector<int> &bits) {
  int result = 0;
  int base = 1;
  for(int i = bits.size()-1 ; i >= 0 ; i--)
  {
    result += bits[i]*base;
    base *= 2;
  }
  return result;
};
