#pragma once
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

Mat KNNClassifier(Mat & X_train,Mat & y_train,Mat & X_test,Mat & y_test);

void ExecutionML(string apprentissage,string aClassifier,int NombreAuteur,double* res);

void Ensemble(double ** pred,int nombreAlgo,int NombreAuteur,int k,int* AuteurK,double* ProbaK);

