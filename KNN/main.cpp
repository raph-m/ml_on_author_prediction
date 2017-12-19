#include"myknn_algo.h"

using namespace std;

int main()
{
   int NumbersofNeighbors=3;
   string FichierAClassifier = "test_10.txt";
   string TrainDataSet = "apprentissage_10.txt";

   vector<vector<double>> Train_Data= Store_data(FichierAClassifier);
   vector<vector<double>> Test_Data= Store_data(TrainDataSet);
   int* Predictions=KNN_( NumbersofNeighbors,FichierAClassifier , TrainDataSet);

   //Print the Predictions
   int NmbofAuthors= Train_Data.size();

   for(int j=0;j<NmbofAuthors;j++)
       cout<<Predictions[j]<<endl;

   return 0;
}
