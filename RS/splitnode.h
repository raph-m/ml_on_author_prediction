#pragma once

#include "node.h"

class SplitNode : public Node {

private:

    NodeSet children;
    unsigned int column;
    double threshold;

public:

    SplitNode(std::string action,
              unsigned int column,
              double threshold) :
        Node(action), column(column), threshold(threshold) {}

    virtual ~SplitNode (void) {
        if (children.size() > 0) {
            for (NodeSet::iterator iter = children.begin(); iter != children.end(); ++iter){
                delete *iter;
            }
        }
    }

    virtual bool get_classification (void) {
        return false;
    }

    virtual bool is_leaf (void) {
        return false;
    }

    virtual void add_child (Node* node) {
        children.push_back(node);
    }

    virtual NodeSet get_children (void) {
        return children;
    }

    unsigned int get_column (void){
        return column;
    }

    double get_threshold (void) {
        return threshold;
    }

};
