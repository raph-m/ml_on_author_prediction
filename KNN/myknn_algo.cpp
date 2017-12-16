#include "myknn_algo.h"

using namespace std;

// this function stores the data for a given Path-file.
vector<vector<double>> Store_data(string Path,int k){
    ifstream in(Path);

    if(!in.is_open()) {
        cout << "ERROR: File Open" << endl;
    }
    vector< vector<double> > data;
    string line;

    while(in.good()){

        getline(in,line);
        //cout<<line<<endl;;
        vector<double> vect;
        stringstream ss(line);
        double i;
        while (ss >> i)
        {
           vect.push_back(i);

           if (ss.peek() == ',')
               ss.ignore();
            }
        for(int l=0;l<k;l++)
            vect.erase(vect.begin());
        data.push_back(vect);

    }

    in.close();
    return data;

}

//this function returns a map key= document , value= author
map <vector<double>, double> BuildMap(vector<vector<double>> data)
{
    map <vector<double>, double> MapOfDocs;
    for(int i=0; i < data.size();i++)
    {
        vector<double> doc=data.at(i);
        double author= doc.at(0);
        doc.erase(doc.begin());
        MapOfDocs[doc]=author;

    }
    return MapOfDocs;
}

// return the moost frequent author

double MFA( vector<double> Authors)
{
    int n=Authors.size();
    int maxfreq=0;
    int freq=1;
    int maxind=0;
    sort(Authors.begin(),Authors.end());


    for(int i=0;i<n-1;i++)
    {
      if(Authors.at(i)==Authors.at(i+1))
      {
        freq++;
        if(freq > maxfreq)
        {
          maxfreq=freq;
          maxind=i;
        }
      }
      else
      {
        freq=1;
      }
    }
    return Authors.at(maxind);
}

// the final Step in KNN
double* KNN(int NumbersofNeighbors, string FichierAClassifier , string TrainDataSet)
{
    //Input
    vector<vector<double>> TestData=Store_data(FichierAClassifier,1);
    BasicKNN* basicKnn = new BasicKNN(NumbersofNeighbors, new Cosine());
    vector<vector<double>> TrainData=Store_data(TrainDataSet,2);


    vector<vector<double>> MapData=Store_data(TrainDataSet,1);
    map <vector<double>, double> MapOfdocs=BuildMap(MapData);

    int N=TestData.size();
    //Output
    double* Predictions=new double[N];


    for(int i=0;i<N;i++)
    {
       vector<double>& current_doc=TestData.at(i);
       vector< vector<double> > res = (*basicKnn)(TrainData, current_doc);

       vector<double> Authors;
       for(int j=0;j<NumbersofNeighbors;j++)
       {
          Authors.push_back(MapOfdocs.find(res.at(j))->second);
       }
       Predictions[i]=MFA(Authors);
    }
    return Predictions;
}










