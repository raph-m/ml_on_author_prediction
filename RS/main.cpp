#include "multiclass_rs.h"


#include <iostream>

using namespace std;

int main() {

    int numberOfTrees = 3;
    int numberOfAuthors = 10;
    string train = "../../finalMatrix.csv";
    string test = "../../finalTest.csv";
    multiclass_rs * classifier1 = new multiclass_rs(numberOfTrees, numberOfAuthors);
    // classifier1->run_random_forest_test(train, test);
    double * predictions_random_forest = classifier1->run_random_forest(train, test);


}
