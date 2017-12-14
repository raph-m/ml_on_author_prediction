#include "randomforest.h"

#include <iostream>

using namespace std;




void RandomForest::grow_forest(
  Dataset & dataset,
  const unsigned int decision_column,
  const unsigned int bootstrap_size,
  const Dataset::KeyList & split_keys,
  const unsigned int keys_per_node,
  const unsigned int tree_count ) {

    // if a forest exists, destroy it
    burn();

    ResultQueue results;

    // initialize tasks
    ForestTask *task = new ForestTask(
                    dataset, decision_column, bootstrap_size, split_keys,
                    keys_per_node, tree_count, &results);
    task -> spawn();

    // get results
    unsigned int on_tree = 0;
    while (forest.size() < tree_count) {
        RandomTree* result = results.pop();
        forest.push_back(result);
        ++on_tree;
        if ((on_tree%1) == 0) {
            cout << " ; " << on_tree << " of " << tree_count << endl;
        }
    }

}

// returns the number of tree that voted 1

int RandomForest::classify (double * row) {

    if (forest.size() <= 0)
        return false;

    int votes_1 = 0;
    for (int tree_index = 0; tree_index < forest.size(); ++tree_index) {
        bool classification = forest[tree_index]->classify(row);
        if (classification)
            ++votes_1;
    }

    return votes_1;
}
