#pragma once

#include "randomtree.h"

class RandomForest {

public:

    typedef std::vector<RandomTree*> Forest;

private:

    Forest forest;

public:

    void grow_forest(
          Dataset & dataset,
          const unsigned int decision_column,
          const unsigned int bootstrap_size,
          const Dataset::KeyList & split_keys,
          const unsigned int keys_per_node,
          const unsigned int tree_count );

    // classify a row using the majority vote from the forest
    int classify (double * row);

    // clears the built forest
    void burn (void) {
        for (Forest::iterator iter = forest.begin();
             iter != forest.end(); ++iter) {
            delete (*iter);
            *iter = null(RandomTree);
        }
        forest.clear();
    }

    Forest get_forest(){
        return forest;
    }

private:
    // défini une queue de résultat thread-safe
    class ResultQueue {

    private:

        Forest results;

    public:

        ResultQueue (void) {

        }

        void push (RandomTree * result) {
            results.push_back(result);
        }

        RandomTree* pop(void) {
            RandomTree* result = results[results.size()-1];
            results.pop_back();
            return result;
        }

    };

    class ForestTask {

    private:

        Dataset & dataset;
        const unsigned int decision_column;
        const unsigned int bootstrap_size;
        const Dataset::KeyList & split_keys;
        const unsigned int keys_per_node;
        const unsigned int tree_count;
        RandomForest::ResultQueue * const result_queue;

    public:

        ForestTask(Dataset & dataset,
                   const unsigned int decision_column,
                   const unsigned int bootstrap_size,
                   const Dataset::KeyList & split_keys,
                   const unsigned int keys_per_node,
                   const unsigned int tree_count,
                   ResultQueue * const result_queue ) :
                     dataset(dataset),
                     decision_column(decision_column),
                     bootstrap_size(bootstrap_size),
                     split_keys(split_keys),
                     keys_per_node(keys_per_node),
                     tree_count(tree_count),
                     result_queue(result_queue) {}

        // launches process
        void spawn (void) {
            for (unsigned int tree_index = 0; tree_index < tree_count; ++tree_index) {
                // generate bootstrap sample
                Dataset bootstrap = dataset.bootstrap_sample(bootstrap_size);
                // generate a random tree
                RandomTree *tree = new RandomTree;
                tree ->grow_decision_tree(bootstrap, split_keys, keys_per_node, decision_column);

                result_queue->push(tree);
             }
        }

    };

};

