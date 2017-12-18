#include "multiclass_rs.h"


#include <iostream>

using namespace std;

int main() {

    int numberOfTrees = 3;
    int row_count = 11;
    string train = "apprentissage.txt";
    string test = "test_full.txt";
    multiclass_rs * classifier1 = new multiclass_rs(numberOfTrees, 10);
    double * predictions_random_forest = classifier1->run_random_forest(train, test);
    for (int j = 0; j < row_count; j++) {
        cout << predictions_random_forest[j] << endl;
    }


}
