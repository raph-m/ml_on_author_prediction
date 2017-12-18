#pragma once

#include "ml_on_author_prediction/RS/node.h"

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

    virtual std::string draw( void ) const
        {
          std::string draw_string =
            "leaf(" + get_action() +
            ")\\n" + (classification ? "1" : "0") +"\"";
          return draw_string;
        }


};
