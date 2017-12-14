#pragma once

#include "node.h"

#include <sstream>

class SplitNode : public Node {

private:

    NodeSet children;
    const unsigned int column;
    const double threshold;

public:

    SplitNode(const std::string action,
              const unsigned int column,
              const double threshold) :
        Node(action), column(column), threshold(threshold) {}

    virtual ~SplitNode (void) {
        if (children.size() > 0) {
            for (NodeSet::iterator iter = children.begin(); iter != children.end(); ++iter){
                delete *iter;
            }
        }
    }

    virtual bool get_classification (void) const {
        return false;
    }

    virtual bool is_leaf (void) const {
        return false;
    }

    virtual void add_child (Node* const node) {
        children.push_back(node);
    }

    virtual NodeSet get_children (void) {
        return children;
    }

    unsigned int get_column (void) const {
        return column;
    }

    double get_threshold (void) const {
        return threshold;
    }

    virtual std::string draw( void ) const
        {
          std::stringstream ss;
          ss << column;
          std::string draw_string =
            "split(" + get_action() +
            ")\\n" + ss.str() +"\"";
          return draw_string;
    }

};
