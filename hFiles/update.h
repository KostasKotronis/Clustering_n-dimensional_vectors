#ifndef UPDATE_H
#define UPDATE_H

#include "data.h"

void kmeans(data &dataset, string assignment, vector<cluster> &clusters);
void pam(data &dataset, string assignment, vector<cluster> &clusters);

#endif
