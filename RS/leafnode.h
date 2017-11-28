#pragma once

#include "node.h"

class LeafNode : public Node {

private:

    const bool classification;

public:

    LeafNode(const std::string action, const bool classification) :
        Node(action), classification(classification) {
    }

    virtual bool get_classification (void) const {
        return classification;
    }

    virtual bool is_leaf (void) const {
        return true;
    }

    virtual void add_child (Node* const node) {}

    virtual NodeSet get_children (void) {
        NodeSet set;
        return set;
    }

};
