#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "data.h"

class hashNode {
  private:
    int index;
    vector<int> g;
  public:
    hashNode(int index, vector<int> &g);
    hashNode(int index);
    ~hashNode();
    void printHashNode(void);
    vector<int> getG(void);
    int getIndex(void);
};

class hashTable {
  private:
    string metric;
    unordered_multimap<int, hashNode> table;
    vector<int> uniqueKeys;
    vector<vector<double>> kVs;
    vector<double> kTs;
    vector<vector<double>> kRs;
  public:
    hashTable(data &dataset, vector<int> &r, int k, string &algorithm);
    ~hashTable();
    void printTable(void);
    vector<hashNode> returnSameBucket(int key);
    bool usedG(vector<int> &g);                 //not used
    vector<int> getUniqueKeys(void);
    vector<double> getkTs(void);
    vector<vector<double>> getkVs(void);
    vector<vector<double>> getkRs(void);
};

#endif
