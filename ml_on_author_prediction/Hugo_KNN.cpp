#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <opencv/cv.h>
#include <opencv/ml.h>
#include <opencv/highgui.h>
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/ml/ml.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
using namespace cv;
using namespace std;

Mat KNNClassifier(Mat & X_train,Mat & y_train,Mat & X_test,Mat & y_test){

    X_train.convertTo(X_train, CV_32FC1);
    y_train.convertTo(y_train,CV_32SC1);

    //on définis les colonnes d'entrainements
    int nsamples_all = X_train.rows;
    Mat sample_idx = Mat::zeros( 1, X_train.rows, CV_8U );
    sample_idx = sample_idx.colRange(0, nsamples_all);
    sample_idx.setTo(Scalar::all(1));

    const Ptr<ml::TrainData> trainData= ml::TrainData::create(X_train, ml::ROW_SAMPLE, y_train, noArray(), sample_idx);

    Ptr<cv::ml::KNearest> knn=ml::StatModel::train<ml::KNearest>(trainData);
    knn->predict(X_test,y_test);

return y_test;
}

void ExecutionML(string apprentissage,string aClassifier,int NombreAuteur,double* res){

    const Ptr<ml::TrainData> trainData= cv::ml::TrainData::loadFromCSV(apprentissage,0,0,2,String(),',');
    const Ptr<ml::TrainData> testData= cv::ml::TrainData::loadFromCSV(aClassifier,0,0,2,String(),',');

    Mat X=trainData->getSamples();
    Mat y1=trainData->getResponses();
    Mat y=y1.col(1);

    Mat X_test=testData->getSamples();
    Mat y_test1=testData->getResponses();
    Mat y_test=y_test1.col(1);

    y_test1.col(1)=KNNClassifier(X,y,X_test,y_test);



    Mat r=y_test1.col(1);


    for(int i=0;i<NombreAuteur;i++)
        res[i]=0;

    int rw=r.rows;
    for(int i=0;i<rw;i++){
    int j=r.at<float>(i,0);
        res[j]++;
    }


    for(int i=0;i<NombreAuteur;i++){
        res[i]=res[i]/rw;}

   //affichage des proba pour chaque auteur à commenter
    for(int i=0;i<NombreAuteur;i++)
        cout <<"proba de " << res[i] << " pour l'auteur " << i+1<< endl;




}

void Ensemble(double ** pred,int nombreAlgo,int NombreAuteur,int k,int* AuteurK,double* ProbaK){

    double res[NombreAuteur];

    for(int i=0;i<NombreAuteur;i++)
        res[i]=0;

    for(int i=0;i<NombreAuteur;i++){
        for(int j=0;j<nombreAlgo;j++){

            res[i]=res[i]+pred[j][i];
        }
    }

    for(int i=0;i<NombreAuteur;i++)
        res[i]=res[i]/nombreAlgo;

    vector <int> a;


    for(int i=0;i<NombreAuteur;i++){
        a.push_back(res[i]);


}

    sort(a.begin(), a.end());

   int b[NombreAuteur];




    for (int i = 0; i < k; ++i){
        ProbaK[i]=a.at(k-1-i);

        for(int j=0;j<NombreAuteur;j++){
            if(res[j]==ProbaK[i]){
                b[i]=j;
                res[j]=-1;
            }
        }

        AuteurK[i]=b[i];
    }


}

