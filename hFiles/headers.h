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

#define ZERO 0.0
#define MEAN 0.0
#define STDDEV 1.0

using namespace std;

extern int numberOfClusters;
extern int numberOfHashTables;
extern int numberOfHashFunctions;

extern int maxUpdateIterations;
extern int maxRangeSearchIterations;

extern string initialization;
extern string assignment;
extern string update;

extern int W;
extern int MCube;            //M for cube
extern int probes;           //#probes
