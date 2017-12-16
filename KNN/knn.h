#ifndef KNN_H
#define KNN_H
// -----------------------------
#include<vector>
#include "distances.h"
// -----------------------------
using  namespace std;

//  ----------------------------------
//  Base Class encompasses K-nearest-neighbors
//  implemented may be used as
//  Machine Learning  Algorithms
// -----------------------------------
class KNN
{
public:
    KNN(){};    //constructor of the base Class
    // ---------------------------------------------------------------------
    // an instance of this class is specific implementation of KNN Algorithm
    // in order to have the same behavior of function I have overloaed the
    //operator ()
    // operator () is a virtual function and must have different behavior
    //whitin inherited classes
    // this function takes all articles and a Query document and returns the
    //K nearest documents of this query documents
    // ----------------------------------------------------------------------
    virtual vector< vector<double> > operator()(const vector< vector<double> >& docs,const vector<double>& Input_doc)const=0;
    virtual ~KNN(){}

};
// -----------------------------------
// the first inherited class:
// this implementation of KNN is
// O(#documtns)
// -----------------------------------
class BasicKNN : public KNN{

public:
    // -----------------------------------
    // Initializing the interger k
    // Choosing a distance to measure
    //the similarity between two documents
    // -----------------------------------
    BasicKNN(int k_,Distances* dist_);
    // -----------------------------------------
    // How do we search over documents ?
    // the dynamic data stucture have been chosen
    // to represent all documents
    // ------------------------------------------
    virtual vector< vector<double> > operator()(const vector< vector<double> >& docs,const vector<double>& Input_doc)const;

    virtual ~BasicKNN() {}; //virtual destructor
private:
    int k; // the number of nearseet neighbors
    Distances* dist; // the distacne object from Distances class
};

#endif // KNN_H
