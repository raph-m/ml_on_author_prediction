#pragma once

#include "ml_on_author_prediction/RS/randomforest.h"

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


    multiclass_rs (int trees, int classes) {
        trees_per_forest = trees;
        class_no = classes;
    }

    void run_random_forest_test (string train_filepath, string test_filepath);

    int * run_random_forest (string train_filepath, string text_filepath);

};
