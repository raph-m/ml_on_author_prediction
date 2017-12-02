#include "randomforest.h"
#include "testRandom.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

namespace test_rd
{

  vector<string> Tokenize(const string& str,const string& delimiters) {

    vector<string> tokens;
    string::size_type delimPos = 0, tokenPos = 0, pos = 0;

    if(str.length() < 1)
      return tokens;

    while (true) {

      delimPos = str.find_first_of(delimiters, pos);
      tokenPos = str.find_first_not_of(delimiters, pos);

      if(string::npos != delimPos) {
        if(string::npos != tokenPos){
          if(tokenPos<delimPos)
            tokens.push_back(str.substr(pos,delimPos-pos));
          else
            tokens.push_back("");
        }
        else
          tokens.push_back("");
        pos = delimPos+1;
      }
      else {
        if(string::npos != tokenPos)
          tokens.push_back(str.substr(pos));
        else
          tokens.push_back("");
        break;
      }

    return tokens;
  }

}

}

void test_setup_tree (void) {

    // charger les données test
    string line;
    vector<string> lines;
    ifstream file("/home/redouane/Documents/data.data", ios_base::in);
    while (getline(file, line, '\n'))
        lines.push_back(line);

    // tokenize la première ligne
    vector<string> l1t = test_rd::Tokenize(lines[0], ",");

    // créer dataset
    const unsigned int row_count = lines.size();
    const unsigned int col_count = l1t.size();
    Dataset dsr (row_count, col_count);

    // convertir les données
    unsigned int donated = 0;
    for (unsigned int row = 0; row < dsr.get_row(); ++row) {
        // tokenize la ligne
        vector<string> tokens = test_rd::Tokenize(lines[row], ",");
        // la class est dans la dernière colonne
        dsr[row][0] = (tokens[col_count-1] == "Donate") ? 1.0 : 0.0;
        // récupérer les features
        for (unsigned int col = 1; col < col_count; ++col)
            dsr[row][col] = atof(tokens[col - 1].c_str());
    }

    cout << dsr.get_row() << endl;

    // configurer les clefs
    Dataset::KeyList & keys = dsr.get_keys();
    keys["class"] = 0;
    keys["recency"] = 1;
    keys["frequency"] = 2;
    keys["monetary"] = 3;
    keys["time"] = 4;
    Dataset::KeyList split_keys = dsr.get_keys();
    split_keys.erase("class");

    // créer l'arbre
    RandomTree tree;
    tree.grow_decision_tree(dsr, split_keys, split_keys.size(),0);

}

void test_tree_constructor (void) {
    RandomTree tree;
    if (tree.get_root() != null(Node))
        cout << "Tree constructor failed" << endl;
}

void test_classify (void) {

    // construire un sample dataset
    Dataset ds( 7, 4 );
    ds[0][0] = 0; ds[0][1] = 1; ds[0][2] = 9; ds[0][3] = 7;
    ds[1][0] = 0; ds[1][1] = 2; ds[1][2] = 9; ds[1][3] = 3;
    ds[2][0] = 0; ds[2][1] = 3; ds[2][2] = 9; ds[2][3] = 4;
    ds[3][0] = 0; ds[3][1] = 4; ds[3][2] = 9; ds[3][3] = 2;
    ds[4][0] = 1; ds[4][1] = 5; ds[4][2] = 9; ds[4][3] = 6;
    ds[5][0] = 1; ds[5][1] = 6; ds[5][2] = 1; ds[5][3] = 1;
    ds[6][0] = 1; ds[6][1] = 7; ds[6][2] = 1; ds[6][3] = 5;
    Dataset::KeyList & keys = ds.get_keys();
    keys["x"] = 0;
    keys["A"] = 1;
    keys["B"] = 2;
    keys["C"] = 3;

    // construire random tree
    Dataset::KeyList split_keys;
    split_keys["A"] = 1;
    split_keys["B"] = 2;
    split_keys["C"] = 3;
    RandomTree tree;
    tree.grow_decision_tree(ds, split_keys, 2u, 0u);

    // classification
    for (unsigned int row = 0; row < ds.get_row(); ++row) {
        bool correct_class = ds[row][0];
        bool classifier_return = tree.classify(ds[row]);
        if (classifier_return != correct_class)
            cout << "Classify failed" << endl;
    }

}

// ds2 pointeur null
void test_setup_forest (void) {

    // charger les données
    string line;
    vector<string> lines;
    ifstream file("/home/redouane/Documents/seq.csv", ios_base::in);
    while (getline(file, line, '\n'))
        lines.push_back(line);

    // tokenize la première ligne
    vector<string> l1t = test_rd::Tokenize(lines[0], "\t");

    // créer le dataset
    const unsigned int row_count = lines.size();
    const unsigned int col_count = l1t.size() - 2; // on ne considère pas la classe et les ids
    const unsigned int feature_count = col_count - 1; // classe et les features

    Dataset * ds = new Dataset( row_count, col_count );
    Dataset & dsr = *ds;
    cout << dsr.get_row() << endl;

    // convertir les données
    for (unsigned int row = 0; row < row_count; ++row) {
        // tokenize la ligne
        vector<string> tokens = test_rd::Tokenize(lines[row], "\t");
        // la classe est dans la deuxième colonne
        dsr[row][0] = ( atof(tokens[1].c_str()) <= 1.0 ) ? 1.0 : 0.0;
        // on charge les features
        for (unsigned int col = 1; col < col_count; ++col)
            dsr[row][col] = atof(tokens[col+1].c_str());
    }

    // configurer les clefs
    Dataset::KeyList & keys = ds->get_keys();
    keys["class"] = 0;
    for (unsigned int feature = 1; feature <= feature_count; ++feature) {
        stringstream feature_ident;
        feature_ident << "f" << feature;
        keys[feature_ident.str()] = feature;
    }



}




int main( const int argc, const char ** argv ) {

    test_setup_tree();
    test_tree_constructor();
    test_classify();

    test_setup_forest();

    return 0;
}
