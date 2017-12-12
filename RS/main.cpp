#include "multiclass_rs.h"


int main() {

    string train = "apprentissage.txt";
    string test = "test_data.txt";
    multiclass_rs * classifier = new multiclass_rs(100, 3);
    classifier->run_random_forest_train(train, test);

}
