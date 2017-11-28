#pragma once

#include "randomtree.h"

#include <pthread.h>
#include <semaphore.h>

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
    unsigned int classify (const double* const & row);

    // clears the built forest
    void burn (void) {
        for (Forest::iterator iter = forest.begin();
             iter != forest.end(); ++iter) {
            delete (*iter);
            *iter = null(RandomTree);
        }
        forest.clear();
    }

private:
    // défini une queue de résultat thread-safe
    class ResultQueue {

    private:

        // exclusion mutuelle
        pthread_mutex_t mutex;

        // semaphore : objet verrouillable
        // avec un compteur
        sem_t semaphore;
        Forest results;

    public:

        ResultQueue (void) {

            // initialisation de mutex
            mutex = PTHREAD_MUTEX_INITIALIZER;

            //initialisation du sémaphore
            sem_init( &semaphore, 0, 0);

        }

        void push (RandomTree * result) {
            pthread_mutex_lock (&mutex);
            results.push_back(result);
            pthread_mutex_unlock(&mutex);
            sem_post (&semaphore);
        }

        RandomTree* pop(void) {
            sem_wait(&semaphore);
            pthread_mutex_lock(&mutex);
            RandomTree* result = results[results.size()-1];
            results.pop_back();
            pthread_mutex_unlock(&mutex);
            return result;
        }

    };

    class ForestTask {

    private:

        pthread_t thread;
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
        int spawn (void) {
            return pthread_create(&thread, NULL, ForestTask::route,
                                  reinterpret_cast<void*>(this));
        }

    private:

        // class_pointer points to the task class to execute
        static void * route (void * class_pointer) {
            ForestTask *task = reinterpret_cast<ForestTask*>(class_pointer);
            task->run();
            return null(void);
        }

        void run (void){
            // iteratively build each tree
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

