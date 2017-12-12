#pragma once

#include "node.h"

class LeafNode : public Node {

private:

    bool classification;

public:

    LeafNode(std::string action, bool classification) :
        Node(action), classification(classification) {
    }

    virtual bool get_classification (void) {
        return classification;
    }

    virtual bool is_leaf (void) {
        return true;
    }

    virtual void add_child (Node* node) {
    }

    virtual NodeSet get_children (void) {
        NodeSet set;
        return set;
    }

};
