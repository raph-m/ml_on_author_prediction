#include"myknn_algo.h"

using namespace std;

int main()
{
   int NumbersofNeighbors=3;
   string FichierAClassifier = "/home/redouane/Downloads/finalTest.csv";
   string TrainDataSet = "/home/redouane/Downloads/finalMatrix.csv";
   double* Predictions=KNN( NumbersofNeighbors,FichierAClassifier , TrainDataSet);
   cout << *Predictions << endl;
   return 0;
}
