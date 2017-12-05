#pragma once

#include "randomforest.h"

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <set>

using namespace std;

class multiclass_rs {

public:

    vector<RandomForest*> * app_forests;
    int class_no = 2;


    multiclass_rs () {
        this->app_forests = new vector<RandomForest*>();
    }

    void run_random_forest_train (string train_filepath, string test_filepath);

    int run_random_forest (string text_filepath);

};
