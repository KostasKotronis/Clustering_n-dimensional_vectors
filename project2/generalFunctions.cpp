#include "./hFiles/headers.h"
#include "./hFiles/data.h"

//return one normal dis number
double NormalDistNumGenerator(void) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();       // construct a trivial random generator engine from a time-based seed
  default_random_engine generator(seed);
  normal_distribution<double> distribution(MEAN,STDDEV);
  return distribution(generator);
};

//return one uniform dist number
double UniformDistNumGenerator(void) {
  random_device rd;                                                             //Will be used to obtain a seed for the random number engine
  mt19937 gen(rd());                                                            //Standard mersenne_twister_engine seeded with rd()
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
