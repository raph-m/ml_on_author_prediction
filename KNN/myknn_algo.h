#ifndef MYKNN_ALGO_H
#define MYKNN_ALGO_H
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include "knn.h"
#include "distances.h"
#include "map"
#include"algorithm"

using namespace std;

// this function stores the data for a given Path-file.
vector<vector<double>> Store_data(string Path);

//this function returns a map key= document , value= author
map <vector<double>, double> BuildMap(vector<vector<double>> data);

// return the moost frequent author
double MFA( vector<double> Authors);

// Final Step of the algorithm
double* KNN(int NumbersofNeighbors, string FichierAClassifier , string TrainDataSet);

// Casting function
int* KNN_(int NumbersofNeighbors, string FichierAClassifier , string TrainDataSet);



#endif // MYKNN_ALGO_H
