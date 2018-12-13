#include "./hFiles/headers.h"
#include "./hFiles/data.h"
#include "./hFiles/hashTable.h"
#include "./hFiles/assignment.h"
#include "./hFiles/cluster.h"
#include "./hFiles/generalFunctions.h"

//update clusters via kmeans
void kmeans(data &dataset, string assignment, vector<cluster> &clusters) {
  int mUI = maxUpdateIterations;
  if(assignment == "lloyd") {
    int count;
    do {
      //cout << mUI << endl;
      vector<cluster> previousClusters;                                         //keep previous Clusters
      for(int i=0; i<numberOfClusters; i++) {
        cluster c;
        clusters[i].createClone(c);
        previousClusters.push_back(c);
        clusters[i].clearPoints();
      }
      lloyd(clusters, dataset);                                                 //assign points to centroids
      count = 0;
      for(int i=0; i<numberOfClusters; i++) {                                   //if previous_clusters == current_clusters ->terminate
        clusters[i].kMeansUpdateCentroid(dataset);                              //update centroids
        count += clusters[i].equalCluster(previousClusters[i]);
      }
      mUI--;
    } while( count != numberOfClusters && mUI);
  }
  else if(assignment == "lsh") {
    vector<hashTable> hashTables;
    int L = numberOfHashTables;
    int k = numberOfHashFunctions;
    vector<int> r = kRandomNum(k, 20);
    for(int i=0; i<L; i++) {                                                    //creates L hushTables
      hashTable hTable(dataset, r, k, assignment);
      hashTables.push_back(hTable);
    }
    //cout << "hashTables are ready!" << endl;
    int count;
    do {
      //cout << mUI << endl;
      vector<cluster> previousClusters;                                         //keep previous Clusters
      for(int i=0; i<numberOfClusters; i++) {
        cluster c;
        clusters[i].createClone(c);
        previousClusters.push_back(c);
        clusters[i].clearPoints();
      }
      LSH(clusters, dataset, hashTables, r, L, k);                              //assign points to centroids
      count = 0;
      for(int i=0; i<numberOfClusters; i++) {                                   //if previous_clusters == current_clusters ->terminate
        clusters[i].kMeansUpdateCentroid(dataset);                              //update centroids
        count += clusters[i].equalCluster(previousClusters[i]);
      }
      mUI--;
    } while( count != numberOfClusters && mUI);
  }
  else if(assignment == "cube") {
    int k = floor(log2(dataset.getN()));
    vector<int> r = kRandomNum(k, 20);
    hashTable hTable(dataset, r, k, assignment);
    //cout << "hashTable is ready!" << endl;
    int count;
    do {
      //cout << mUI << endl;
      vector<cluster> previousClusters;                                         //keep previous Clusters
      for(int i=0; i<numberOfClusters; i++) {
        cluster c;
        clusters[i].createClone(c);
        previousClusters.push_back(c);
        clusters[i].clearPoints();
      }
      cube(clusters, dataset, hTable, r, MCube, k, probes);                     //assign points to centroids
      count = 0;
      for(int i=0; i<numberOfClusters; i++) {                                   //if previous_clusters == current_clusters ->terminate
        clusters[i].kMeansUpdateCentroid(dataset);                              //update centroids
        count += clusters[i].equalCluster(previousClusters[i]);
      }
      mUI--;
    } while( count != numberOfClusters && mUI);
  }
  else {
    cout << "Unknown assignment algorithm. Program is terminating..." << endl;
    exit (EXIT_FAILURE);
  }
};

//update clusters via pam
void pam(data &dataset, string assignment, vector<cluster> &clusters) {
  int mUI = maxUpdateIterations;
  if(assignment == "lloyd") {
    int count;
    do {
      //cout << mUI << endl;
      vector<cluster> previousClusters;                                         //keep previous Clusters
      for(int i=0; i<numberOfClusters; i++) {
        cluster c;
        clusters[i].createClone(c);
        previousClusters.push_back(c);
        clusters[i].clearPoints();
      }
      lloyd(clusters, dataset);                                                 //assign points to centroids
      count = 0;
      for(int i=0; i<numberOfClusters; i++) {                                   //if previous_clusters == current_clusters ->terminate
        clusters[i].pamUpdateCentroid(dataset);                                 //update centroids
        count += clusters[i].equalCluster(previousClusters[i]);
      }
      mUI--;
    } while( count != numberOfClusters && mUI);
  }
  else if(assignment == "lsh") {
    vector<hashTable> hashTables;
    int L = numberOfHashTables;
    int k = numberOfHashFunctions;
    vector<int> r = kRandomNum(k, 20);
    for(int i=0; i<L; i++) {                                                    //creates L hushTables
      hashTable hTable(dataset, r, k, assignment);
      hashTables.push_back(hTable);
    }
    //cout << "hashTables are ready!" << endl;
    int count;
    do {
      //cout << mUI << endl;
      vector<cluster> previousClusters;                                         //keep previous Clusters
      for(int i=0; i<numberOfClusters; i++) {
        cluster c;
        clusters[i].createClone(c);
        previousClusters.push_back(c);
        clusters[i].clearPoints();
      }
      LSH(clusters, dataset, hashTables, r, L, k);                              //assign points to centroids
      count = 0;
      for(int i=0; i<numberOfClusters; i++) {                                   //if previous_clusters == current_clusters ->terminate
        clusters[i].pamUpdateCentroid(dataset);                                 //update centroids
        count += clusters[i].equalCluster(previousClusters[i]);
      }
      mUI--;
    } while( count != numberOfClusters && mUI);
  }
  else if(assignment == "cube") {
    //int k = floor(log2(dataset.getN()));
    int k = numberOfHashFunctions;
    vector<int> r = kRandomNum(k, 20);
    hashTable hTable(dataset, r, k, assignment);
    //cout << "hashTable is ready!" << endl;
    int count;
    do {
      //cout << mUI << endl;
      vector<cluster> previousClusters;                                         //keep previous Clusters
      for(int i=0; i<numberOfClusters; i++) {
        cluster c;
        clusters[i].createClone(c);
        previousClusters.push_back(c);
        clusters[i].clearPoints();
      }
      cube(clusters, dataset, hTable, r, MCube, k, probes);                     //assign points to centroids
      count = 0;
      for(int i=0; i<numberOfClusters; i++) {                                   //if previous_clusters == current_clusters ->terminate
        clusters[i].pamUpdateCentroid(dataset);                                 //update centroids
        count += clusters[i].equalCluster(previousClusters[i]);
      }
      mUI--;
    } while( count != numberOfClusters && mUI);
  }
  else {
    cout << "Unknown assignment algorithm. Program is terminating..." << endl;
    exit (EXIT_FAILURE);
  }
};
