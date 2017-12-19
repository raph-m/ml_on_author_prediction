// --------------------------------------------------------
#include "knn.h"
#include<map>         // to use the map as data structure
#include <algorithm>  // to use Sawp function
#include<utility>     // to use Pair
// --------------------------------------------------------

// -----------------------------
// BasicKNN constructor
// -----------------------------

BasicKNN::BasicKNN(int k_,Distances* dist_):k(k_),dist(dist_)
{
 }
// -----------------------------
// The Core of the Algorithm
// -----------------------------

//the arguments are :
//         -Space of All documents : vector of vectors
//         - Target document: Input_docs
vector< vector<double> > BasicKNN:: operator()( const vector< vector<double> >& docs,const vector<double>& Input_doc)const{
    //---------------------------
    //Initialization Step
    //---------------------------

    map<double,pair<vector<double>,double> > mapOfdocs;

    //Step1: take the first K documents
    vector< vector<double> >::const_iterator  it=docs.begin();
    for(int i=1;i<=k;i++)
    {
        mapOfdocs[i]=make_pair(*it, (double)((*dist)(*it,Input_doc))); // the Key=i ,
                                                      // the value=(doc, distance(doc, Input)
        it++;
    }

    //Step2: Sort the mapOfdocuments
    int i, j;
    for (i = 1; i <=k; i++)

        // Last i elements are already in place
        for (j = 1; j <= k-i; j++)
            if (mapOfdocs[j].second >mapOfdocs[j+1].second )
               swap(mapOfdocs[j], mapOfdocs[j+1]);

    // Step3: Update mapOfdocs by looping over the rest of documents
    //in the corpus

    for(;it!=docs.end();it++)
    {
        double delta= (double)(*dist)(*it,Input_doc); // compute the distance

        if(delta > mapOfdocs[k].second) continue; // do nothing if this doc is near than the Kth document

        // else: update the mapOfdocuments
        mapOfdocs[j]=make_pair(*it,delta);

        // Find the  right rank of this document
        for(int j=k-1;j>=1;j--)
        {
            if (mapOfdocs[j].second >mapOfdocs[j+1].second )
            {
                swap(mapOfdocs[j], mapOfdocs[j+1]);
            }

        }

    }

    //return K  most similar documents
    vector< vector<double> > Results;

    //reserve K empty documents
    Results.reserve(k);
    map<double,pair<vector<double>,double> >::iterator Newit = mapOfdocs.begin();
    while(Newit != mapOfdocs.end())
    {
    Results.push_back(Newit->second.first);
    Newit++;
    }
    return Results;

};
int BasicKNN::KNN_randomized(vector<int>& authors,vector<double> input_doc)
{
      vector<int> UniqueValue;
      vector<int>::iterator it;
      it = unique (authors.begin(), authors.end());
      for (it=authors.begin(); it!=authors.end(); ++it)
          UniqueValue.push_back(*it);

      // using built-in random generator:
      random_shuffle ( UniqueValue.begin(), UniqueValue.end() );

      if(input_doc.size())
          return UniqueValue.at(0);
      // using myrandom:
      //return UniqueValue[rand() % UniqueValue.size() ];
}
