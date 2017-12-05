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
vector<vector<long>> BasicKNN:: operator()( const vector<vector<long>>& docs,const vector<long>& Input_doc)const{
    //---------------------------
    //Initialization Step
    //---------------------------

    map<long,pair<vector<long>,long> > mapOfdocs;

    //Step1: take the first K documents
    vector< vector<long> >::const_iterator  it=docs.begin();
    for(int i=1;i<=k;i++)
    {
        mapOfdocs[i]=make_pair(*it,(*dist)(*it,Input_doc)); // the Key=i ,
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
        long delta=(*dist)(*it,Input_doc); // compute the distance

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
    vector<vector<long>> Results;

    //reserve K empty documents
    Results.reserve(k);
    map<long,pair<vector<long>,long> >::iterator Newit = mapOfdocs.begin();
    while(Newit != mapOfdocs.end())
    {
    Results.push_back(Newit->second.first);
    Newit++;
    }
    return Results;

}
