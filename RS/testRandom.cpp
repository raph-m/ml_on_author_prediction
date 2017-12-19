#include "ml_on_author_prediction/RS/randomforest.h"
#include "ml_on_author_prediction/RS/testRandom.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

void test_setup_tree (void) {

    // charger les données test
    string line;
    vector<string> lines;
    ifstream file("data.data", ios_base::in);
    while (getline(file, line, '\n'))
        lines.push_back(line);

    // tokenize la première ligne
    vector<string> l1t;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = lines[0].find(delimiter)) != std::string::npos) {
        token = lines[0].substr(0, pos);
        l1t.push_back(token);
        lines[0].erase(0, pos + delimiter.length());
    }
    // créer dataset
    const unsigned int row_count = lines.size();
    const unsigned int col_count = l1t.size();

    Dataset dsr (row_count, col_count);

    // convertir les données
    dsr[0][0] = (l1t[col_count-1] == "Donate") ? 1.0 : 0.0;
    for (unsigned int col = 1; col < col_count; ++col)
        dsr[0][col] = atof(l1t[col - 1].c_str());

    for (unsigned int row = 1; row < dsr.get_row(); ++row) {

        // tokenize la ligne
        vector<string> tokens;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;

        while ((pos = lines[row].find(delimiter)) != std::string::npos) {
            token = lines[row].substr(0, pos);
            tokens.push_back(token);
            lines[row].erase(0, pos + delimiter.length());
        }

        // la class est dans la dernière colonne
        dsr[row][0] = (tokens[col_count-1] == "Donate") ? 1.0 : 0.0;

        // récupérer les features
        for (unsigned int col = 1; col < col_count; ++col){
            dsr[row][col] = atof(tokens[col - 1].c_str());
        }
    }

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

    cout << "Success 1" << endl;

}

void test_tree_constructor (void) {
    RandomTree tree;
    if (tree.get_root() != null(Node))
        cout << "Tree constructor failed" << endl;
    cout << "Success 2" << endl;
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
        cout << correct_class << "     " << classifier_return << endl;
        if (classifier_return != correct_class)
            cout << "Classify failed" << endl;
    }

    cout << "Success 3" << endl;

}


void test_forest (void) {

    // charger les données
    string line;
    vector<string> lines;
    ifstream file("seq.csv", ios_base::in);
    while (getline(file, line, '\n'))
        lines.push_back(line);

    // tokenize la première ligne
    vector<string> l1t;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = lines[0].find(delimiter)) != std::string::npos) {
        token = lines[0].substr(0, pos);
        l1t.push_back(token);
        lines[0].erase(0, pos + delimiter.length());
    }

    // créer le dataset
    const unsigned int row_count = lines.size();
    const unsigned int col_count = l1t.size() - 2; // on ne considère pas la classe et les ids
    const unsigned int feature_count = col_count; // classe et les features

    Dataset * ds = new Dataset( row_count, col_count );
    Dataset & dsr = *ds;

    // la classe est dans la deuxième colonne
    dsr[0][0] = ( atof(l1t[1].c_str()) <= 1.0 ) ? 1.0 : 0.0;
    for (unsigned int col = 1; col < col_count; ++col)
        dsr[0][col] = atof(l1t[col+1].c_str());

    // convertir les données
    for (unsigned int row = 1; row < row_count; ++row) {
        // tokenize la ligne
        vector<string> tokens;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        while ((pos = lines[row].find(delimiter)) != std::string::npos) {
            token = lines[row].substr(0, pos);
            tokens.push_back(token);
            lines[row].erase(0, pos + delimiter.length());
        }
        // la classe est dans la deuxième colonne
        dsr[row][0] = ( atof(tokens[1].c_str()) <= 1.0 ) ? 1.0 : 0.0;
        // on charge les features
        for (unsigned int col = 1; col < col_count; ++col)
            dsr[row][col] = atof(tokens[col+1].c_str());
    }

    // configurer les clefs
    Dataset::KeyList & keys = dsr.get_keys();
    keys["class"] = 0;
    for (unsigned int feature = 1; feature <= feature_count; ++feature) {
        stringstream feature_ident;
        feature_ident << "f" << feature;
        keys[feature_ident.str()] = feature;
    }

    // on affecte les split_keys
    Dataset::KeyList split_keys = ds->get_keys();
    split_keys.erase("class");

    // on fait grandir la forêt
    cout << "Grow..." << endl;
    RandomForest rand_forest;
    int num_trees = 50;
    rand_forest.grow_forest(*ds, 0, ds->get_row()/3, split_keys, 16, num_trees);
    cout << rand_forest.get_forest()[0]->draw() << endl;
    cout << "Grown!" << endl;

    // on classifie les données
    unsigned int tp = 0;
    unsigned int tn = 0;
    unsigned int fp = 0;
    unsigned int fn = 0;
    for (unsigned int row = 0; row < ds->get_row(); ++row) {

        bool c = 2*rand_forest.classify((*ds)[row]) > num_trees;

        bool t = fabs((*ds)[row][0] - 1.0) < 0.5;
        if (c&&t)
            ++tp;
        else if (c&&!t)
            ++fp;
        else if (!c&&t)
            ++fn;
        else if (!c&&!t)
            ++tn;
        else cout << "WTF" << endl;
    }

    double accuracy = (tp + tn) * 100.0 / (tp + fp + tn + fn);
    double precision = tp * 100.0 / (tp + fp);
    double true_negative_rate = tn * 100.0 / (tn + fp);
    double recall = tp * 100.0 / (tp + fn);
    cout
        << "Accuracy          : " << accuracy << "%\n"
        << "Precision         : " << precision << "%\n"
        << "True negative rate: " << true_negative_rate << "%\n"
        << "Recall            : " << recall << "%\n" << endl;

    cout << "Success 4" << endl;

}



/*
int main( const int argc, const char ** argv ) {

    //test_setup_tree();
    //test_tree_constructor();
    //test_classify();
    test_forest();

    return 0;
} */


