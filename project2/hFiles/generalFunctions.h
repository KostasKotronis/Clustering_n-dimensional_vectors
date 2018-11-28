#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

#include "cluster.h"

//data createDataset(string &inputFileName);
double NormalDistNumGenerator(void);
double UniformDistNumGenerator(void);
vector<int> kRandomNum(int k, int maxNum);
int binaryToDecimalConverter(vector<int>& bits);

// cluster
dVector parseLine(string &line);
data createDataset(string &inputFileName);
void readConfigurationFile(string &configurationFileName);
double UniformDistNumGenerator(double minimum, double maximum);

#endif
