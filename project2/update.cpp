#include "./hFiles/headers.h"
#include "./hFiles/data.h"
#include "./hFiles/hashTable.h"
#include "./hFiles/assignment.h"
#include "./hFiles/cluster.h"
#include "./hFiles/generalFunctions.h"

//void LSH(vector<cluster> &clusters, data &dataset, vector<hashTable> &hTables, vector<int> &r, int L, int k);
void kmeans(data &dataset, string assignment, vector<cluster> &clusters) {
    if(assignment == "lloyd") {
      int count;
      do {
        vector<cluster> previousClusters;                                            //keep previous Clusters
        for(int i=0; i<clusters.size(); i++) {
          cluster c;
          clusters[i].createClone(c);
          previousClusters.push_back(c);
          clusters[i].clearPoints();
        }

        count = 0;
        for(int i=0; i<clusters.size(); i++)
          count += clusters[i].equalCluster(previousClusters[i]);
        //cout << count << endl;
        //getchar();
        //cout << "b: ";
        //previousClusters[1].printCentroid();
        lloyd(clusters, dataset);


        for(int i=0; i<clusters.size(); i++)
          clusters[i].updateCentroid(dataset);

        /*for(int i=0; i<clusters.size(); i++) {
          cout << i << ". ";
          clusters[i].printCentroid();
        }*/
        //cout << "a: ";
        //clusters[1].printCentroid();
        //getchar();
        /*clusters[1].printPoints();
        cout << clusters[1].getNumberOfPoints() << endl;
        getchar();*/
        count = 0;
        for(int i=0; i<clusters.size(); i++)
          count += clusters[i].equalCluster(previousClusters[i]);
        //cout << count << endl;
      } while( count != clusters.size() );
    }
    /*if(assignment == "lsh") {
      vector<hashTable> hashTables;
      int L = numberOfHashTables;
      int k = numberOfHushFunctions;
      vector<int> r = kRandomNum(k, 20);
      for(int i=0; i<L; i++) {                                                                          //creates L hushTables
      hashTable hTable(dataset, r, k, assignment);
      hashTables.push_back(hTable);
      }
      cout << "hashTables are ready!" << endl;
      do {
        LSH(clusters, dataset, hashTables, r, L, k);
      } while(0);
    }
    if(assignment == "cube") {

      int k = floor(log2(dataset.getN()));
      vector<int> r = kRandomNum(k, 20);
      hashTable hTable(dataset, r, k, assignment);
      cout << "hashTable is ready!" << endl;
      do {

        cube(clusters, dataset, hTable, r, MCUBE, k, PROBES);

      } while(0);
    }*/
}
