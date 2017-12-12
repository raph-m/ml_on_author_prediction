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

    int trees_per_forest;
    int class_no;
    vector<RandomForest*> app_forests;



    multiclass_rs (int trees, int classes) {
        trees_per_forest = trees;
        class_no = classes;
    }

    void run_random_forest_train (string train_filepath, string test_filepath);

    int run_random_forest (string text_filepath);

};
