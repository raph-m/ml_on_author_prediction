#include "multiclass_rs.h"


int main() {

    string train = "apprentissage.txt";
    string test = "test_full.txt";
    multiclass_rs * classifier1 = new multiclass_rs(15, 10);
    multiclass_rs * classifier2 = new multiclass_rs(16, 10);
    multiclass_rs * classifier3 = new multiclass_rs(17, 10);
    multiclass_rs * classifier4 = new multiclass_rs(18, 10);
    classifier1->run_random_forest_test(train, test);
    classifier2->run_random_forest_test(train, test);
    classifier3->run_random_forest_test(train, test);
    classifier4->run_random_forest_test(train, test);

}
