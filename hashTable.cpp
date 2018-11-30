#include "./hFiles/headers.h"
#include "./hFiles/hashTable.h"
#include "./hFiles/euclidean.h"
#include "./hFiles/cosine.h"
#include "./hFiles/generalFunctions.h"
/*~hashNode~*/
/****************************************/
hashNode::hashNode(int index, vector<int> &g) {
  this->index = index;
  this->g = g;
};

hashNode::hashNode(int index) {
  this->index = index;
};

hashNode::~hashNode() {};

//print the node
void hashNode::printHashNode(void){
  cout << "index: " << this->index <<  " ";
  cout << "g{ ";
  for(auto i=this->g.begin(); i!=this->g.end(); ++i)
    cout << *i << " ";
  cout << "}";
};

//return g of node
vector<int> hashNode::getG(void) {
  return this->g;
};

//return index of node
int hashNode::getIndex(void) {
  return this->index;
};

/*~hashTable~*/
/****************************************/

hashTable::hashTable(data &dataset, vector<int> &r, int k, string &algorithm) {
  this->table.max_load_factor(2);
  this->metric = dataset.getMetric();
  if( dataset.getMetric() == "euclidean") {
    for(int j=0; j<k; j++) {
      vector<double> v;
      for(int i=0; i<dataset.getD(); i++) {
        v.push_back(NormalDistNumGenerator());
      }
      this->kVs.push_back(v);
      double t = UniformDistNumGenerator();                                     //get t
      this->kTs.push_back(t);
    }
  }
  if ( dataset.getMetric() == "cosine") {
    for(int j=0; j<k; j++) {
      vector<double> rr;
      for(int i=0; i<dataset.getD(); i++) {
        rr.push_back(NormalDistNumGenerator());
      }
      this->kRs.push_back(rr);
    }
  }
  for(int i=0; i<dataset.getN(); i++) {                                         //for every point in dataset
    if( dataset.getMetric() == "euclidean") {
      int key;
      if(algorithm == "lsh") {
        dVector x = dataset.getdVector(i);
        vector<int> g = euclideanGenerateG(x, k, this->kVs, this->kTs);         //generate a g vector
        key = fHushFunction(g, r, dataset.getN());                              //from that g ->generate f
        hashNode node(i, g);
        this->table.insert(make_pair(key, node));                               //insert the node to the table
      }
      if(algorithm == "cube") {
        dVector x = dataset.getdVector(i);
        key = cubeGenerateKey(x, k, this->kVs, this->kTs);
        if(find(this->uniqueKeys.begin(), this->uniqueKeys.end(), key) == this->uniqueKeys.end())
          this->uniqueKeys.push_back(key);
        hashNode node(i);
        this->table.insert(make_pair(key, node));                               //insert the node to the table
      }
    }
    if( dataset.getMetric() == "cosine") {
      dVector x = dataset.getdVector(i);
      vector<int> g = cosineGenerateG(x, k, this->kRs);                         //generate a g vector
      int key = binaryToDecimalConverter(g);                                    //from that g ->generate key
      if(algorithm == "cube") {
        if(find(this->uniqueKeys.begin(), this->uniqueKeys.end(), key) == this->uniqueKeys.end())
          this->uniqueKeys.push_back(key);
      }
      hashNode node(i);
      this->table.insert(make_pair(key, node));                                 //insert the node to the table
    }
  }
};

hashTable::~hashTable() {};

//print all the table
void hashTable::printTable(void) {
  cout << "metric: " << this->metric << endl;
  unordered_multimap<int, hashNode>::iterator i;
  for(i = this->table.begin(); i != this->table.end(); i++) {
    cout << "<" << i->first << ", ";
    i->second.printHashNode();
    cout << "> " << endl;
  }
  cout << "t{ ";
  for(auto i=this->kTs.begin(); i!=this->kTs.end(); ++i)
    cout << *i << " ";
  cout << "}" << endl;
  if(this->kVs.empty())
    cout << "k-V vector is empty" << endl;
  else
    cout << "k-V vector is full" << endl;
  if(this->kRs.empty())
    cout << "k-R vector is empty" << endl;
  else
    cout << "k-R vector is full" << endl;
  if(this->uniqueKeys.empty())
    cout << "uniqueKeys vector is empty" << endl;
  else
    cout << "uniqueKeys vector is full" << endl;
};

//return bucket with key <key>
vector<hashNode> hashTable::returnSameBucket(int key) {
  vector<hashNode> h;
  auto itr = this->table.equal_range(key);
  for(auto it = itr.first; it != itr.second; it++)
    h.push_back(it->second);
  return h;
};

//(not used)
//return true if given g is already used in this table -> false otherwise
bool hashTable::usedG(vector<int> &g) {
  unordered_multimap<int, hashNode>::iterator i;
  for(i = this->table.begin(); i != this->table.end(); i++) {
    if(i->second.getG() == g)
      return true;
  }
  return false;
};

vector<int> hashTable::getUniqueKeys(void) {
  return this->uniqueKeys;
};

vector<double> hashTable::getkTs(void) {
  return this->kTs;
};

vector<vector<double>> hashTable::getkVs(void) {
  return this->kVs;
};

vector<vector<double>> hashTable::getkRs(void) {
  return this->kRs;
};
