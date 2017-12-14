#pragma once

#include "dataset.h"
#include "leafnode.h"
#include "splitnode.h"

class RandomTree {

private :

    Node* root;

public:

    RandomTree (void) : root (null(Node)) {}

    ~RandomTree (void){
        burn();
    }

    void burn (void) {
        if (root != null(Node)){
            delete root;
            root = null(Node);
        }
    }

    Node* get_root (void) {
        return root;
    }

    std::string draw( void ) const;

    void grow_decision_tree (Dataset & data,
         const Dataset::KeyList & split_keys,
         const unsigned int keys_per_node,
         const unsigned int decision_column);

    // input un pointeur à la rangée de data
    // output la classification
    bool classify (const double* const & row);

private:

    // méthode utilse pour runner l'arbre
    // keys_per_node le nombre de clés à comparer
    // action : l'action qui a mené à ce noeud
    void _grow_decision_tree(Dataset & data,
         const Dataset::KeyList & split_keys,
         const unsigned int keys_per_node,
         const unsigned int decision_column,
         Node* const parent,
         const std::string action);

};
