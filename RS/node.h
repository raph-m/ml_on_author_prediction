#pragma once

#include <vector>
#include <string>


// interface du noeud d'un arbre
class Node {

private:

    std::string action;

public:

    // set de nodes
    typedef std::vector<Node*> NodeSet;

public:

    Node( const std::string action ) :
        action(action) {}

    virtual ~Node( void ) {}

    // const signifie que la méthode n'est pas autorisé
    // à changer l'objet à laquelle elle s'applique
    std::string get_action (void) {
        return action;
    }

    // retourne la classification d'un noeud
    // la syntaxe virtual ... = 0 veut dire que la méthode
    // est une méthode purement virtuelle
    // ie. on est obligé de la réécrire dans chaque sous-classe
    virtual bool get_classification (void) = 0;

    virtual bool is_leaf (void) = 0;

    virtual void add_child (Node* node) = 0;

    virtual NodeSet get_children (void) = 0;

};




