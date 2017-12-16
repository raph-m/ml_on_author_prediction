// -----------------------------
#include "distances.h"
#include <algorithm>  // to use transform funtion
#include <numeric>    // to use  accumulate function
#include <math.h>      // to use pow function

#include <cmath>
#include <vector>
#include <functional>
// ------------------------------
// Euclidean constructor
// ------------------------------
Euclidean::Euclidean(int p_) :p(p_)
{
}
Euclidean::~Euclidean()
{
}
// ------------------------------
// This operator returns the euclidean distance between
// X1 and X2
// ------------------------------
double Euclidean::operator()(const vector<double>& X1, const vector<double>& X2)const
{

/*
        vector<double>	result;
        transform(X1.begin(), X1.end(), X2.begin(), back_inserter(result),
                  [&](double x, double y) {return pow(abs(x-y),p); });

        result.shrink_to_fit(); //Requests the removal of unused capacity

        return  pow(accumulate(result.begin(), result.end(), 0.0),1.0/p);*/

        double dist;

        for (int i = 0; i < X1.size(); i++)
        {
            dist += pow( (double)(X1.at(i) - X2.at(i)) , 2.0);
        }


    return  sqrt(dist);


}
// ------------------------------
// This operator returns the Cosine Similarity between
// X1 and X2
// ------------------------------
double Cosine::operator()(const vector<double>& X1,const  vector<double>& X2) const
{
        //vector<double>::const_iterator X1_Iter = X1.begin();
        //vector<double>::const_iterator X2_Iter = X2.begin();

/*
        auto X1_Iter = X1.begin();
        auto X2_Iter = X2.begin();

        double current_product = 0.0;
        double d_X1 = 0.0;
        double d_X2 = 0.0;
        for (; X1_Iter != X1.end(); X1_Iter++,X2_Iter++)
        {
                current_product += (double)((*X1_Iter) * (*X2_Iter));
                d_X1 += (double) (*X1_Iter) * (*X1_Iter);
                d_X2 += (double) (*X2_Iter) * (*X2_Iter);
        }
        return current_product / (sqrt(d_X1) * sqrt(d_X2));
        */



        double current_product = 0;

        double d_X1 = 0.0;
        double d_X2 = 0.0;

        for (int i = 0; i < X1.size(); i++)
        {
                current_product += (double)X1.at(i) * X2.at(i);

                d_X1 += (double) X1.at(i) * X1.at(i);
                d_X2 += (double) X2.at(i) * X2.at(i);
        }


        return current_product / (sqrt(d_X1) * sqrt(d_X2));


}

Cosine::~Cosine()
{
}

double Distances::operator()(const vector<double>& X1,const  vector<double>& X2) const
{
       return 0;
}

