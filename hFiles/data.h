#ifndef DATA_H
#define DATA_H

class dVector {
  private:
    int id;
    int dimension;
    vector<double> coordinates;
  public:
    dVector();
    dVector(int dvId);
    ~dVector();
    void setId(int i);
    int getDimension(void);
    int getId(void);
    void addCoordinate(double c);
    double getCoordinate(int i);
    vector<double> getCoordinates(void);
    void printdVector(void);
    bool isCoordinatesEmpty(void);
};

class data {
  private:
    vector<dVector> dVectors;
    string metric;
    int d;                      //dimension
    int n;
  public:
    data();
    ~data();
    void setMetric(string m);
    void addDVector(dVector &v);
    string getMetric(void);
    int getN(void);
    int getD(void);
    void printData(void);
    dVector getdVector(int i);
    int getdVectorId(int i);
    double linearSearchMinDistance(vector<double> &q);
    double euclideanDis(int index1 ,int index2);
    double cosineDis(int index1 ,int index2);
};

#endif
