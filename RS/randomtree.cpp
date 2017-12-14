#include "randomtree.h"

#include <list>
#include <sstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

void RandomTree::grow_decision_tree(
  Dataset & data,
  const Dataset::KeyList & split_keys,
  const unsigned int keys_per_node,
  const unsigned int decision_column) {

    burn();

    if (split_keys.size() < keys_per_node)
        return;

    _grow_decision_tree (data, split_keys,
          keys_per_node, decision_column, root, "<root>");

}

void RandomTree::_grow_decision_tree(
  Dataset & data,
  const Dataset::KeyList & split_keys,
  const unsigned int keys_per_node,
  const unsigned int decision_column,
  Node * const parent,
  const std::string action ){

    ProbaMap pm_class =
        data.count_thresh( decision_column, 0 );

    // choix d'un subset random de clés
    Dataset::KeyList sample_keys;
    while (sample_keys.size() < keys_per_node){
        unsigned int index = rand() % split_keys.size();
        Dataset::KeyList::const_iterator iter = split_keys.begin();
        unsigned int index_dec = index;
        while (index_dec --) {
            ++iter;
        }
        if (sample_keys.find(iter->first) == sample_keys.end())
            sample_keys[iter->first] = iter->second;
    }

    // Détermination du plus grand IG

    bool          finished_splitting    = true;
    double        highest_ig            =  0.0;
    unsigned int  highest_ig_column     =  0;
    std::string   highest_ig_column_str = "";
    double        highest_ig_threshold  = -1.0;
    for ( Dataset::KeyList::const_iterator key = sample_keys.begin();
        key != sample_keys.end(); ++key ) {
        double threshold = 0.0;
        double ig = data.IG( decision_column, key->second, threshold );
        if ( ig > highest_ig ) {
            finished_splitting = false;
            highest_ig = ig;
            highest_ig_threshold = threshold;
            highest_ig_column = key->second;
            highest_ig_column_str = key->first;
        }
    }

    if ( finished_splitting ) {
       // détermine la bonne classe
       bool best_class = (pm_class["LE"] < pm_class["G"]);

       // crée la feuille
       LeafNode * leaf = new LeafNode(action, best_class);
       if (parent == null(Node)) {
         root = leaf;
       }
       else {
         parent->add_child(leaf);
       }
   }

   else {

        // création du split node
        Node* split = new SplitNode(action, highest_ig_column, highest_ig_threshold);

        // division des données en deux
        DatasetSplitPair split_pair = data.split(highest_ig_column, highest_ig_threshold);

        // construction du noeud <=
        stringstream action_le;
        action_le << highest_ig_column_str << " <= " << highest_ig_threshold;
        _grow_decision_tree(split_pair.ds_le, split_keys, keys_per_node,
                            decision_column, split, action_le.str());

        //construction du noeud >
        stringstream action_g;
        action_g << highest_ig_column_str << " > " << highest_ig_threshold;
        _grow_decision_tree(split_pair.ds_g, split_keys, keys_per_node,
                            decision_column, split, action_g.str());

        // ajout du noeud à l'arbre
        if (parent == null(Node))
            root = split;
        else
            parent ->add_child(split);

   }
}

//------------------------------------------------------------------------------------------

bool RandomTree::classify(const double* const & row) {

    Node* next_node = this->get_root();

    while (next_node != null(Node)){

        if(next_node->is_leaf()){

            return next_node->get_classification();
        }
        else {

            SplitNode *split = reinterpret_cast<SplitNode*>(next_node);

            unsigned int split_column = split -> get_column();

            double split_threshold = split->get_threshold();


            Node::NodeSet children = split->get_children();

            // 2u represents unsigned literal for 2
            if(children.size() == 2u){
                unsigned int child_index = (row[split_column] > split_threshold) ? 1 : 0;

                next_node = children[child_index];

            }

            // shouldn't have more or less than 2 children
            else {
                return false;
            }
        }
    }


    // shouldn't happen
    return false;
}

std::string RandomTree::draw( void ) const
{
  stringstream output;
  if (root != null(Node))
  {
    output << "graph G{\n";
    list<Node*> open_list;
    open_list.push_back(root);

    unsigned int node_id      = 0;
    unsigned int last_node_id = 0;
    while ( !open_list.empty() )
    {
      Node * node = open_list.front();

      // Draw node.
      output << "N" << node_id << "[" << node->draw() << "];\n";

      // Has children?
      if ( !node->is_leaf() )
      {
        Node::NodeSet children = node->get_children();
        for (
          Node::NodeSet::const_iterator iter = children.begin();
          iter != children.end(); ++iter )
        {
          Node * child = *iter;

          ++last_node_id;
          output << "N" << node_id << " -- N" << last_node_id << ";\n";
          open_list.push_back( child );
        }
      }

      // Pop element and increment ID.
      open_list.pop_front();
      ++node_id;
    }
    output << "}";
  }
  return output.str();
}

