#include <iostream>
#include <string>
#include <fstream>          //ifstream
#include <cstring>          //strcmp
#include <vector>
#include <list>
#include <stdlib.h>         //atof
#include <sstream>          //stringstream
#include <chrono>           //normal distribution
#include <random>           //normal distribution   //uniform real distribution
#include <algorithm>        //find
#include <cmath>            //pow
#include <unordered_map>    //unordered_map
#include <cstdio>           //clock
#include <ctime>            //clock
#include <set>
#include <iterator>

#define W 3
#define ZERO 0.0
#define MEAN 0.0
#define STDDEV 1.0
#define N 10                 //number of iterations in range-search
#define MCUBE 500           //M for cube
#define PROBES 10            //#probes

using namespace std;

extern int memoryOfDataset;

extern int memoryOfHashTables;

extern int correctDistance;

extern int numberOfQueries;
