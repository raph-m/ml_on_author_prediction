#include "multiclass_rs.h"

#include <time.h>

int main() {

    string train = "apprentissage.txt";
    string test = "test_full.txt";
    multiclass_rs * classifier1 = new multiclass_rs(13, 10);
    int * predictions_random_forest = classifier1->run_random_forest(train, test);

}
