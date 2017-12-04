#include "randomforest.h"
#include "testDataPrep.h"

#include <iostream>

using namespace std;
/*

void test_entropy (void) {

    ProbaMap pmap;

    // test vide
    if( fabs(pmap.entropy()) > 1e-9) {
        cout << "Entropy 1 failed" << endl;
    }

    // test avec une entrée
    pmap["e1"] = 1.0;
    if( fabs(pmap.entropy()) > 1e-9) {
        cout << "Entropy 2 failed" << endl;
    }

    // test avec quelques entrées dont un 0
    pmap["e1"] = 0.5;
    pmap["e2"] = 0.5;
    pmap["e3"] = 0.0;
    if( fabs(pmap.entropy() - 1 ) > 1e-9) {
        cout << "Entropy 3 failed" << endl;
    }

    pmap["e1"] = 0.1;
    pmap["e2"] = 0.2;
    pmap["e3"] = 0.3;
    pmap["e4"] = 0.4;
    if( fabs(pmap.entropy() - 1.846439345 ) > 1e-9) {
        cout << "Entropy 4 failed" << endl;
    }

}

void test_column (void) {

    // initialisation
    const unsigned int original_row_count = 10;
    const unsigned int shrink_row_count   = 5;
    unsigned int *data_pointer = null(unsigned int);
    IntegerCol icol( original_row_count );
    if (original_row_count != icol.get_row()) {
        cout << "Column 1 failed" << endl;
    }

    // affectations
    for (unsigned int row = 0; row < icol.get_row(); ++row) {
        if (row > 0) {
            if (icol.contains(row))
                cout << "Column 2 failed" << endl;
        }
        icol[row] = row;
        if (icol[row] != row)
            cout << "Column 3 failed" << endl;
        if (!icol.contains(row))
            cout << "Column 4 failed" << endl;
    }

    // test de resize en ayant diminué la taille
    data_pointer = &icol[0];
    icol.resize(shrink_row_count);
    if (data_pointer == &icol[0])
        cout << "Column 5 failed" << endl;
    if (icol.get_row() != shrink_row_count)
        cout << "Column 6 failed" << endl;
    for (unsigned int row = 0; row < icol.get_row(); ++row) {
        if (row != icol[row])
            cout << "Column 7 failed" << endl;
    }

    // test de expand en ayant augmenté la taille
    data_pointer = &icol[0];
    icol.resize(original_row_count);
    if (data_pointer == &icol[0])
        cout << "Column 8 failed" << endl;
    if (icol.get_row() != original_row_count)
        cout << "Column 9 failed" << endl;
    for (unsigned int row = 0; row < shrink_row_count; ++row) {
        if (icol[row] != row)
            cout << "Column 10 failed" << endl;
    }
    for (unsigned int row = shrink_row_count; row < icol.get_row(); ++row) {
        if (icol[row] != 0u)
            cout << "Column 11 failed" << endl;
    }

}

void test_matrix (void) {

    for ( unsigned int rowSpan = 1; rowSpan < 10; ++rowSpan ) {
       for ( unsigned int columnSpan = 1; columnSpan < 10; ++columnSpan ) {
         RealMatrix matrix( rowSpan, columnSpan );

         for ( unsigned int row = 0; row < matrix.get_row(); ++row ) {
             double * writeable = matrix[row];
             const double * const unwriteable = matrix[row];
             for ( unsigned int column = 0; column < matrix.column_count(); ++column) {
                 if (writeable[column] != 0.0)
                     cout << "Matrix 1 failed" << endl;
                 if (unwriteable[column] != 0.0)
                     cout << "Matrix 2 failed" << endl;
                 double index = row*matrix.column_count()+column;
                 writeable[column] = index;
                 if (index != writeable[column])
                     cout << "Matrix 3 failed" << endl;
                 if (index != unwriteable[column])
                     cout << "Matrix 4 failed" << endl;
             }
         }
       }
    }

}

void test_constructor_dataset (void) {

    const unsigned int rows = 5;
    const unsigned int cols = 5;

    // test de construction 'from scratch'
    Dataset ds1(rows, cols);
    if (rows != ds1.data_reference().get_row())
        cout << "Dataset Constructor 1 failed" << endl;
    for (unsigned int r = 0; r < ds1.data_reference().get_row(); ++r){
        if (r != ds1(r))
            cout << "Dataset Constructor 2 failed" << endl;
    }
    if (ds1.data_matrix().get_row() != rows)
        cout << "Dataset Constructor 3 failed" << endl;
    if (ds1.data_matrix().column_count() != cols)
        cout << "Dataset Constructor 4 failed" << endl;

    // test de construction à partir d'une référence
    unsigned int rows2 = 3;
    Dataset ds2(ds1, rows2);
    if (rows2 != ds2.data_reference().get_row())
        cout << "Dataset Constructor 4 failed" << endl;
    for (unsigned int r = 0; r < ds2.data_reference().get_row(); ++r){
        if (ds2(r) != 0u)
            cout << "Dataset Constructor 5 failed" << endl;
    }
    if (ds2.data_matrix().get_row() != rows)
        cout << "Dataset Constructor 6 failed" << endl;
    if (ds2.data_matrix().column_count() != cols)
        cout << "Dataset Constructor 7 failed" << endl;

}

void test_operator_array (void) {

    // initialisation
    const unsigned int rows = 8;
    const unsigned int cols = 1;
    Dataset ds(rows, cols);

    // remplissage
    for (unsigned int r = 0; r < rows; ++r){
        ds[r][0] = r + 1;
        if (static_cast<unsigned int>(ds.data_matrix()[r][0]) != r + 1)
            cout << "Operator array 1 failed" << endl;
    }

    // inversement des références aux lignes
    for (unsigned int r = 0; r < rows; ++r)
        ds(rows-r-1) = r;
    for (unsigned int r = 0; r < rows; ++r) {
        ds[r][0] = r + 8;
        if (static_cast<unsigned int>(ds.data_matrix()[rows - r - 1][0]) != r + 8)
            cout << "Operator array 2 failed" << endl;
    }
}

void test_count_thresh (void) {

    // initialisation
    const unsigned int rows = 10;
    const unsigned int cols = 1;
    ProbaMap pmap;
    Dataset ds1 (rows, cols);

    // remplissage
    for (unsigned int row = 0; row < rows; ++row)
        ds1[row][0] = row + 1;

    // comptage
    pmap = ds1.count_thresh(0,5);
    if (fabs(pmap["LE"] - 0.5) > 1e-9)
        cout << "Count thresh 1 failed" << endl;
    if (fabs(pmap["G"] - 0.5) > 1e-9)
        cout << "Count thresh 2 failed" << endl;
    pmap = ds1.count_thresh(0,10);
    if (fabs(pmap["LE"] - 1.0) > 1e-9)
        cout << "Count thresh 3 failed" << endl;
    if (fabs(pmap["G"] - 0.0) > 1e-9)
        cout << "Count thresh 4 failed" << endl;
    pmap = ds1.count_thresh(0,0);
    if (fabs(pmap["LE"] - 0.0) > 1e-9)
        cout << "Count thresh 5 failed" << endl;
    if (fabs(pmap["G"] - 1.0) > 1e-9)
        cout << "Count thresh 6 failed" << endl;

    // nouveau dataset
    Dataset ds2 (ds1, rows);

    // tous les éléments sont mappés vers l'élément 0 par défaut
    // ils pointent donc vers la valeur du premier élément de
    // la matrice data de ds1 (soit 1)

    // comptage
    pmap = ds2.count_thresh(0,5);
    if (fabs(pmap["LE"] - 1.0) > 1e-9)
        cout << "Count thresh 7 failed" << endl;
    if (fabs(pmap["G"] - 0.0) > 1e-9)
        cout << "Count thresh 8 failed" << endl;
    pmap = ds2.count_thresh(0,10);
    if (fabs(pmap["LE"] - 1.0) > 1e-9)
        cout << "Count thresh 9 failed" << endl;
    if (fabs(pmap["G"] - 0.0) > 1e-9)
        cout << "Count thresh 10 failed" << endl;
    pmap = ds2.count_thresh(0,1);
    if (fabs(pmap["LE"] - 1.0) > 1e-9)
        cout << "Count thresh 11 failed" << endl;
    if (fabs(pmap["G"] - 0.0) > 1e-9)
        cout << "Count thresh 12 failed" << endl;
    pmap = ds2.count_thresh(0,0);
    if (fabs(pmap["LE"] - 0.0) > 1e-9)
        cout << "Count thresh 13 failed" << endl;
    if (fabs(pmap["G"] - 1.0) > 1e-9)
        cout << "Count thresh 14 failed" << endl;

    // mapper les éléments
    for (unsigned int row = 0; row < rows; ++row)
        ds2(row) = row;

    // comptage
    pmap = ds2.count_thresh(0,5);
    if (fabs(pmap["LE"] - 0.5) > 1e-9)
        cout << "Count thresh 15 failed" << endl;
    if (fabs(pmap["G"] - 0.5) > 1e-9)
        cout << "Count thresh 16 failed" << endl;
    pmap = ds2.count_thresh(0,10);
    if (fabs(pmap["LE"] - 1.0) > 1e-9)
        cout << "Count thresh 17 failed" << endl;
    if (fabs(pmap["G"] - 0.0) > 1e-9)
        cout << "Count thresh 18 failed" << endl;
    pmap = ds2.count_thresh(0,0);
    if (fabs(pmap["LE"] - 0.0) > 1e-9)
        cout << "Count thresh 19 failed" << endl;
    if (fabs(pmap["G"] - 1.0) > 1e-9)
        cout << "Count thresh 20 failed" << endl;

}

void test_get_thresh (void) {

    // initialisation
    const unsigned int rows = 12;
    const unsigned int cols = 1;
    Dataset ds(rows, cols);

    // remplissage
    for (unsigned int row = 0; row < rows; ++row)
        ds[row][0] = row + 1;

    // obtention des seuils
    Dataset::ThreshVector thresholds = ds.get_thresh(0);
    for (unsigned int row = 0; row < rows; ++row) {
        if (fabs(thresholds[row] - static_cast<double>(row + 1)) > 1e-9)
            cout << "Get Thresh failed" << endl;
    }
}

void test_split (void) {

    // initialisation
    const unsigned int rows = 10;
    const unsigned int cols = 1;
    Dataset ds(rows, cols);

    // remplissage
    for (unsigned int row = 0; row < rows; ++row)
        ds[row][0] = row + 1;

    // division
    DatasetSplitPair split = ds.split(0, 5.0);
    if (split.ds_le.get_row() != 5u)
        cout << "Split 1 failed" << endl;
    if (split.ds_g.get_row() != 5u)
        cout << "Split 2 failed" << endl;
    for (unsigned int row = 0; row < split.ds_le.get_row(); ++row) {
        if (fabs(split.ds_le[row][0] - static_cast<double>(row + 1)) > 1e-9)
            cout << "Split 3 failed" << endl;
        if (fabs(split.ds_g[row][0] - static_cast<double>(row + 5 + 1)) > 1e-9)
            cout << "Split 4 failed" << endl;
    }

}

void test_IG (void) {

    // initialisation
    const unsigned int rows = 10;
    const unsigned int cols = 2;
    Dataset ds(rows, cols);

    // générer les données
    for (double class_thresh = 1.0; class_thresh < 10.0; class_thresh += 1.0) {

        for (unsigned row = 0; row < ds.get_row(); ++row) {
            ds[row][0] = row + 1;
            if (ds[row][0] > class_thresh)
                ds[row][1] = 1.0;
            else
                ds[row][1] = 0.0;
        }

        // trouver le meilleur IG
        double threshold = 0.0;
        ds.IG(1, 0, threshold);
        if (fabs(threshold - class_thresh) > 1e-9)
            cout << "IG failed" << threshold << class_thresh << endl;
    }

}

void test_bootstrap (void) {

    // initialisation
    const unsigned int rows = 10;
    const unsigned int cols = 1;
    Dataset ds (rows, cols);

    // générer les données
    for (unsigned int row = 0; row < rows; ++row)
        ds[row][0] = row + 1;

    // créer bootstrap sample
    for (unsigned int sample_size = 0; sample_size < 20; ++sample_size) {
        Dataset sample = ds.bootstrap_sample(sample_size);
        if (sample.get_row() != sample_size)
            cout << "Bootstrap 1 failed" << endl;
        for (unsigned int row = 0; row < sample_size; ++row) {
            if ((sample(row) < 0) || (sample (row) >= ds.get_row()))
                cout << "Bootstrap 2 failed" << endl;
        }
    }

}

void test_leafnode (void) {

    const std::string action = "Some action";
    const bool classification = true;

    // créer leaf node
    LeafNode node (action, classification);
    if (node.get_action() != action)
        cout << "LeafNode 1 failed" << endl;
    if (node.get_classification() != classification)
        cout << "LeafNode 2 failed" << endl;
    if (node.is_leaf() != true)
        cout << "LeafNode 3 failed" << endl;

    // essayer de rajouter des enfants
    LeafNode newChild (action, classification);
    Node::NodeSet set = node.get_children();
    if (static_cast<unsigned int>(set.size()) != 0u)
        cout << "LeafNode 4 failed" << endl;
    node.add_child(&newChild);
    set = node.get_children();
    if (static_cast<unsigned int>(set.size()) != 0u)
        cout << "LeafNode 5 failed" << endl;

}

void test_splitnode (void) {

    const std::string action = "Some action";
    const unsigned int column = 2;
    const double threshold = 17.5;

    // créer split node
    SplitNode node (action, column, threshold);
    if (node.get_action() != action)
        cout << "SplitNode 1 failed" << endl;
    if (node.get_classification() != false)
        cout << "SplitNode 2 failed" << endl;
    if (node.is_leaf() != false)
        cout << "SplitNode 3 failed" << endl;
    if (node.get_threshold() != threshold)
        cout << "SplitNode 4 failed" << endl;

    // essayer d'ajouter des enfants
    SplitNode * newChild = new SplitNode(action, column, threshold);
    Node::NodeSet set = node.get_children();
    if (static_cast<unsigned int>(set.size()) != 0u)
        cout << "SplitNode 5 failed" << endl;
    node.add_child(newChild);
    set = node.get_children();
    if (static_cast<unsigned int>(set.size()) != 1u)
        cout << "SplitNode 6 failed" << endl;
    if (dynamic_cast<SplitNode*>(set[0]) != newChild)
        cout << "SplitNode 7 failed" << endl;

} */

/*
int main( const int argc, const char ** argv ) {

    test_entropy();
    test_column();
    test_matrix();
    test_constructor_dataset();
    test_operator_array();
    test_count_thresh();
    test_get_thresh();
    test_split();
    test_IG();
    test_bootstrap();
    test_leafnode();
    test_splitnode();

    return 0;

} */

