#ifndef EVALUATION_H
#define EVALUATION_H

#include "data.h"
#include "cluster.h"

void silhouette(vector<cluster> clusters, data &dataset, string &outputFileName);

#endif
