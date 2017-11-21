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

X_train.convertTo(X_train, CV_32FC1);
y_train.convertTo(y_train,CV_32SC1);

//on définis les colonnes d'entrainements
int nsamples_all = X_train.rows;
Mat sample_idx = Mat::zeros( 1, X_train.rows, CV_8U );
sample_idx = sample_idx.colRange(0, nsamples_all);
sample_idx.setTo(Scalar::all(1));

const Ptr<ml::TrainData> trainData= ml::TrainData::create(X_train, ml::ROW_SAMPLE, y_train, noArray(), sample_idx);

Ptr<cv::ml::NormalBayesClassifier> normalBayesC=ml::StatModel::train<ml::NormalBayesClassifier>(trainData);
normalBayesC->predictProb(X_test,y_test,y_test_Prob);

}


int main( void )
{


Mat X=Mat(1000, 10, CV_32FC1);
Mat y=Mat(1000, 1, CV_32SC1);
Mat test=Mat(5000, 10, CV_32FC1);
Mat rendre=Mat(5000, 1, CV_32SC1);
Mat rendre2=Mat(5000, 1,CV_32FC1);
const Scalar low=0;
const Scalar higt=1;
cv::randu(X,low,higt);
cv::randu(test,low,higt);
cout << "Etape 2" << endl;
NBayesClassifier(X,y,test,rendre,rendre2);

}
