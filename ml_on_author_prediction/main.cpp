#include "mainwindow.h"
#include <QApplication>
#include <iostream>
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

void NBayesClassifier(Mat & X_train,Mat & y_train,Mat & X_test,Mat & y_test,Mat & y_test_Prob){
cout << "NBayes learn"<<endl;

X_train.convertTo(X_train, CV_32FC1);
y_train.convertTo(y_train,CV_32SC1);

//on définis les colonnes d'entrainements
int nsamples_all = X_train.rows;
Mat sample_idx = Mat::zeros( 1, X_train.rows, CV_8U );
sample_idx = sample_idx.colRange(0, nsamples_all);
sample_idx.setTo(Scalar::all(1));

const Ptr<ml::TrainData> trainData= ml::TrainData::create(X_train, ml::ROW_SAMPLE, y_train, noArray(), sample_idx);

//cv::ml::TrainData::loadFromCSV() a utiliser avec un fichier CSV
Ptr<cv::ml::NormalBayesClassifier> normalBayesC=ml::StatModel::train<ml::NormalBayesClassifier>(trainData);
normalBayesC->predictProb(X_test,y_test,y_test_Prob);
//cout<<y_test << endl;
}

void KNNClassifier(Mat & X_train,Mat & y_train,Mat & X_test,Mat & y_test){
cout << "KNN learn"<<endl;
X_train.convertTo(X_train, CV_32FC1);
y_train.convertTo(y_train,CV_32SC1);

//on définis les colonnes d'entrainements
int nsamples_all = X_train.rows;
Mat sample_idx = Mat::zeros( 1, X_train.rows, CV_8U );
sample_idx = sample_idx.colRange(0, nsamples_all);
sample_idx.setTo(Scalar::all(1));

const Ptr<ml::TrainData> trainData= ml::TrainData::create(X_train, ml::ROW_SAMPLE, y_train, noArray(), sample_idx);

//cv::ml::TrainData::loadFromCSV() a utiliser avec un fichier CSV
Ptr<cv::ml::KNearest> knn=ml::StatModel::train<ml::KNearest>(trainData);
knn->predict(X_test,y_test);
//cout<<y_test << endl;
}

void SvmClassifier(Mat & X_train,Mat & y_train,Mat & X_test,Mat & y_test){
cout << "SVM learn"<<endl;
X_train.convertTo(X_train, CV_32FC1);
y_train.convertTo(y_train,CV_32SC1);

//on définis les colonnes d'entrainements
int nsamples_all = X_train.rows;
Mat sample_idx = Mat::zeros( 1, X_train.rows, CV_8U );
sample_idx = sample_idx.colRange(0, nsamples_all);
sample_idx.setTo(Scalar::all(1));

//cv::ml::TrainData::loadFromCSV() a utiliser avec un fichier CSV
const Ptr<ml::TrainData> trainData= ml::TrainData::create(X_train, ml::ROW_SAMPLE, y_train, noArray(), sample_idx);


Ptr<cv::ml::SVM> svm=ml::StatModel::train<ml::SVM>(trainData);
svm->predict(X_test,y_test);
//cout<<y_test << endl;
}

float Accuracy(Mat & y_true,Mat & y_test){
    float g=0;
    for(int i=0;i<y_true.rows;i++){
        if(y_true.at<int>(i,0)==y_test.at<int>(i,0))
            g++;
    }
   // Mat ac=y_true-y_test;
   // cv::absdiff(y_true,y_test,ac);
   // cout<< ac << endl;
  // cv::Scalar_<double> g=cv::sum(cv::sum((ac)));
   float a = y_true.rows;

   cout << "accuracy="<<g/a<<endl;

   return g;
}

double Precision(Mat & y_true,Mat & y_test,int NumerosAuteur){

    double tp=0;
    double fp=0;
    for(int i=0;i<y_true.rows;i++){
        if(y_test.at<int>(i,0)==NumerosAuteur){
            if(y_true.at<int>(i,0)==NumerosAuteur)
                tp++;
            else
                fp++;
        }
    }

    return tp/(tp+fp);

}

double Recall(Mat & y_true,Mat & y_test,int NumerosAuteur){

    double tp=0;
    double fn=0;
    for(int i=0;i<y_true.rows;i++){
        if(y_true.at<int>(i,0)==NumerosAuteur){
            if(y_test.at<int>(i,0)==NumerosAuteur)
                tp++;
            else
                fn++;
        }
    }

    return tp/(tp+fn);

}

double FMesure(Mat & y_true,Mat & y_test,int NumerosAuteur){
    double prec=Precision(y_true,y_test,NumerosAuteur);
    double rec=Recall(y_true,y_test,NumerosAuteur);

    return 2.0*(prec*rec)/(prec+rec);
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
void AfficheResultat(Mat & y,string livre[],string Auteurs[],int sign){

//cout << y <<endl;
cout <<"Affichage des Résultats :"<< endl;
    for(int i=0;i<y.rows;i++){
        //cout << type2str(y.type())<< endl;

        if(sign){
         int id=y.at<int>(i,0);
         cout << "le livre "<<livre[i] << "a été écrit par "<< Auteurs[id] <<endl;
        }
        else{
            int id2=y.at<float>(i,0);
           cout << "le livre "<<livre[i] << " a été écrit par "<< Auteurs[id2] <<endl;
          }
    }

}

void Test(){

    int NbLivres_train=50;
    int NbAuteurs_train=5;
    int NbFeatures=1000;

    int NbLivres_test=5;




    string livre []={"livre A","Livre B","livre C","Livre D","Livre E"};
    string Auteur []={"Auteur A","Auteur AE","Auteur AH","Auteur ANJ","Auteur AP"};

    Mat X=Mat(NbLivres_train,NbFeatures, CV_32FC1);
    Mat y=Mat(NbLivres_train, 1, CV_32SC1);
    Mat test=Mat(NbLivres_test, NbFeatures, CV_32FC1);

    Mat resKNN;
    Mat resNB;
    Mat resSVM;

    Mat rendre2=Mat(NbLivres_test, 1,CV_32FC1);

    const Scalar low=0;
    const Scalar higt=66;
    cv::randn(X,low,higt);
    cv::randn(test,low,higt);


    const Scalar low2=0;
    const Scalar higt2=NbAuteurs_train;
    //Mat y2=Mat(1000, 1, CV_32SC1);
    cv::randu(y,low2,higt2);

    cout <<"Debut du Test " << endl;
    cout<<"resultats"<<endl;


    KNNClassifier(X,y,test,resKNN);
    AfficheResultat(resKNN,livre,Auteur,0);

    NBayesClassifier(X,y,test,resNB,rendre2);
    AfficheResultat(resNB,livre,Auteur,1);

    SvmClassifier(X,y,test,resSVM);
    AfficheResultat(resSVM,livre,Auteur,0);

     //cv::Scalar_<double> a=Accuracy(y,y2);
    //cout << FMesure(y,y,0) << endl;

}

void fct (Mat & y ,int NombreAuteur ,double* res) {



    for(int i=0;i<NombreAuteur;i++)
        res[i]=0;

    int rw=y.rows;
    for(int i=0;i<rw;i++){

        res[y.at<int>(i,0)]++;
    }

    for(int i=0;i<NombreAuteur;i++){
        res[i]=res[i]/rw;}



}

Mat Execution(Mat & X, Mat & y,Mat & X_test,Mat & y_test){

    Mat resKNN;
    Mat resNB;
    Mat resSVM;
    Mat rendre2=Mat(X_test.rows, 1,CV_32FC1);

    KNNClassifier(X,y,X_test,resKNN);
    cout<<"Accuracy KNN"<<endl;
    cout<<Accuracy(y_test,resKNN) << endl;

    NBayesClassifier(X,y,X_test,resNB,rendre2);
    cout<<"Accuracy NB"<<endl;
    cout<<Accuracy(y_test,resNB) << endl;

    SvmClassifier(X,y,X_test,resSVM);
    cout<<"Accuracy SVM"<<endl;
    cout<<Accuracy(y_test,resSVM) << endl;

return resKNN;
}


int main( int argc, char *argv[] )
{
    //const Ptr<ml::TrainData> trainData= cv::ml::TrainData::loadFromCSV("/Users/hugotouvron/Desktop/Datatest.csv",1);
    const Ptr<ml::TrainData> trainData= cv::ml::TrainData::loadFromCSV("/Users/hugotouvron/Desktop/apprentissage10.csv",0,0,2,String(),',');
    const Ptr<ml::TrainData> testData= cv::ml::TrainData::loadFromCSV("/Users/hugotouvron/Desktop/test10.csv",0,0,2,String(),',');
    const Ptr<ml::TrainData> idData= cv::ml::TrainData::loadFromCSV("/Users/hugotouvron/Desktop/test10.csv",0,0,1,String(),',');

    Mat X=trainData->getSamples();
    Mat y1=trainData->getResponses();
    Mat y=y1.col(1);

    cout<< X << endl;
    cout<< y << endl;

    Mat X_test=testData->getSamples();
    Mat y_test1=testData->getResponses();
    Mat y_test=y_test1.col(1);

    cout<< X_test << endl;
    cout<< y_test << endl;

    Mat id =idData->getSamples();

    list<int> livre;
    for(int i=0;i<id.rows;i++){

        livre.push_back(id.at<int>(i,0));
    }

    y_test1.col(1)=Execution(X,y,X_test,y_test);

    cout<< 'resultat' << endl;
    cout<< y_test1 << endl;

    int NbAUT=10;
    Mat r=y_test1.col(1);
    double res[NbAUT];

    for(int i=0;i<NbAUT;i++)
        res[i]=0;

    int rw=r.rows;
    for(int i=0;i<rw;i++){
    int j=r.at<float>(i,0);
    cout << "test"<<j << endl;
        res[j]++;
    }


    for(int i=0;i<NbAUT;i++){
        res[i]=res[i]/rw;}


    for(int i=0;i<NbAUT;i++)
        cout <<"proba de " << res[i] << " pour l'auteur " << i+1<< endl;

    //Test();
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();

return 0;
}
