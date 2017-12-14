#include "multiclass_rs.h"

using namespace std;

/**
 * This function takes a training set and a testing set and trains a random forest
 * and evaluates its performance on the training test
 * @brief multiclass_rs::run_random_forest_test
 * @param train_filepath
 * @param test_filepath
 * @param perf
 */
void multiclass_rs::run_random_forest_test ( string train_filepath, string test_filepath) {

    // TRAIN & TEST
    // class_no classifiers 1 vs all

    cout << "Loading data..." << endl;

    // charger les données train
    string line;
    vector<string> lines;
    ifstream file(train_filepath, ios_base::in);
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

    // charger les données test
    string line_test;
    vector<string> lines_test;
    ifstream file_test(test_filepath, ios_base::in);
    while (getline(file_test, line_test, '\n'))
        lines_test.push_back(line_test);

    // tokenize la première ligne
    vector<string> l1t_test;
    size_t pos_test = 0;
    std::string token_test;
    while ((pos_test = lines_test[0].find(delimiter)) != std::string::npos) {
        token_test = lines_test[0].substr(0, pos_test);
        l1t_test.push_back(token_test);
        lines_test[0].erase(0, pos_test + delimiter.length());
    }

    cout << "Training random forest..." << endl;

    // créer le dataset train
    unsigned int row_count = lines.size();
    unsigned int col_count = l1t.size() - 1; // on ne considère pas les ids
    unsigned int feature_count = col_count - 1; // classe et les features

    // créer le dataset test
    unsigned int row_count_test = lines_test.size();

    // dataset train
    Dataset dsr( row_count, col_count );
    double label [row_count];

    // dataset test
    Dataset * ds_t = new Dataset(row_count_test, col_count);
    Dataset & ds_test = * ds_t;
    // store true labels
    double label_test [row_count_test];

    // données train
    // la classe est dans la deuxième colonne
    label[0] = atof(l1t[1].c_str());
    for (unsigned int col = 1; col < col_count; ++col)
        dsr[0][col] = atof(l1t[col+1].c_str());
    for ( unsigned int row = 1; row < row_count; ++row ) {
        // tokenize la ligne
        vector<string> tokens1;
        size_t pos1 = 0;
        std::string token1;
        while ((pos1 = lines[row].find(delimiter)) != std::string::npos) {
            token1 = lines[row].substr(0, pos1);
            tokens1.push_back(token1);
            lines[row].erase(0, pos1 + delimiter.length());
        }
        // la classe est dans la deuxième colonne
        label[row] = atof(tokens1[1].c_str());
        // on charge les features
        for (unsigned int col = 1; col < col_count; ++col)
            dsr[row][col] = atof(tokens1[col+1].c_str());
    }

    // données test
    // la classe est dans la deuxième colonne
    label_test[0] = atof(l1t_test[1].c_str());
    for (unsigned int col = 1; col < col_count; ++col)
        ds_test[0][col] = atof(l1t_test[col+1].c_str());
    for (unsigned int row = 1; row < row_count_test; ++row ) {
        // tokenize la ligne
        vector<string> tokens2;
        size_t pos2 = 0;
        std::string token2;
        while ((pos2 = lines_test[row].find(delimiter)) != std::string::npos) {
            token2 = lines_test[row].substr(0, pos2);
            tokens2.push_back(token2);
            lines_test[row].erase(0, pos2 + delimiter.length());
        }
        // la classe est dans la deuxième colonne
        label_test[row] = atof(tokens2[1].c_str());
        // on charge les features
        for (unsigned int col = 1; col < col_count; ++col)
            ds_test[row][col] = atof(tokens2[col+1].c_str());
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
    Dataset::KeyList split_keys = dsr.get_keys();
    split_keys.erase("class");

    // contient le plus grand nombre d'arbre ayant voté i pour un certain i
    int votes [row_count_test];
    // contient le i associé à la valeur dans vote
    double pred_labels [row_count_test];
    for (unsigned int row = 0; row < row_count_test; ++row) {
        votes[row] = 0;
        pred_labels[row] = 0;
    }

    // généralisation multiclass 1 vs. all

    // les différents classificateurs 1 vs. all seront dans app_forests

    cout << "Starting 1 vs. all classifications" << endl;

    for (int i=0; i < class_no;i++ ) {

        cout << i << endl;

        for (unsigned int row = 0; row < row_count ; ++row) {
            if (fabs(label[row] - double(i)) < 0.5)
                dsr[row][0] = 1.0;
            else
                dsr[row][0] = 0.0;
        }

        /*
        // oversampling
        // remplit avec un quart de i

        Dataset dsr_i( row_count, col_count );

        int count_class_i = 0;
        set<int> rows_i;

        for ( unsigned int row = 0; row < row_count; ++row ){
            if (fabs(dsr[row][0]- i) < 0.5){
                count_class_i++;
                rows_i.insert(row);
            }
        }

        if ( count_class_i != 0) {

            set<int> rows_to_fill;
            for ( unsigned int row = 0; row < row_count; ++row )
                rows_to_fill.insert(row);

            // on transforme la classe i en 1.0
            int counter = 0;
            for ( unsigned int row = 0; row < row_count; ++row ){
                if (rows_i.find(row) != rows_i.end()){
                      dsr_i[row][0] = 1.0;
                        for ( unsigned int col = 1; col < col_count; ++col )
                            dsr_i[row][col] = dsr[row][col];
                        rows_to_fill.erase(row);
                }
            }

            while (counter < row_count / 4 - count_class_i) {

                int random_row = rows_to_fill.size()*rand() / RAND_MAX;
                set<int>::const_iterator ite(rows_to_fill.begin());
                advance(ite,random_row);
                int fill_row = *ite;

                int randomize = (count_class_i - 1)*rand() / RAND_MAX;
                set<int>::const_iterator it(rows_i.begin());
                advance(it,randomize);
                int curr_row = *it;

                dsr_i[fill_row][0] = 1.0;
                for ( unsigned int col = 1; col < col_count; ++col ){
                    dsr_i[fill_row][col] = dsr[curr_row][col];
                }

                rows_to_fill.erase(fill_row);
                counter++;
           }

           while (!rows_to_fill.empty()) {

                int fill_row = *(rows_to_fill.begin());

                int taken_row = row_count * rand() / RAND_MAX;

                if(fabs(dsr[taken_row][0] - i) > 0.5) {
                    dsr_i[taken_row][0] = 0.0;
                    for ( unsigned int col = 1; col < col_count; ++col )
                        dsr_i[fill_row][col] = dsr[taken_row][col];
                    rows_to_fill.erase(fill_row);
                }

           }
           */

        // Data should be loaded. Time to grow the forest.
        cout << "Grow..." << endl;
        RandomForest forest_classifier;

        forest_classifier.grow_forest(dsr, 0, (dsr.get_row()*4) / 5, split_keys, 32, this->trees_per_forest );

        // cout << forest_classifier.get_forest()[0]->draw() << endl;

        cout << "Grown!" << endl;

        cout << "Classifying..." << endl;

        for (unsigned int row = 0; row < row_count_test; ++row ) {
           int votes1 = forest_classifier.classify(ds_test[row]);
           if (votes1 > votes[row]){
              votes[row] = votes1;
              pred_labels[row] = i;
           }
        }

   }

    //Evaluate
    unsigned int quality = 0;
    for (unsigned int row =0; row < row_count_test; ++row) {
        // is the predicted class the right one?
        // cout << label_test[row] << "   " << pred_labels[row] << endl;
        bool t =(fabs(label_test[row] -  pred_labels[row]) < 0.5);
        if (t) {
            quality = quality + 1;
        }
    }

    cout << quality << "  " << row_count_test << endl;

}



/**
 * This function takes into a csv made of blocks taken from an input text
 * Ignore two first columns (IDs and classes randomly placed)
 * @brief run_random_forest
 * @param text_filepath
 */

int multiclass_rs::run_random_forest (string train_filepath, string text_filepath) {

    // TRAIN & CLASSIFY
    // class_no classifiers 1 vs all

    cout << "Loading data..." << endl;

    // charger les données train
    string line;
    vector<string> lines;
    ifstream file(train_filepath, ios_base::in);
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

    // charger les données à classifier
    string line_test;
    vector<string> lines_test;
    ifstream file_test(text_filepath, ios_base::in);
    while (getline(file_test, line_test, '\n'))
        lines_test.push_back(line_test);

    // tokenize la première ligne
    vector<string> l1t_test;
    size_t pos_test = 0;
    std::string token_test;
    while ((pos_test = lines_test[0].find(delimiter)) != std::string::npos) {
        token_test = lines_test[0].substr(0, pos_test);
        l1t_test.push_back(token_test);
        lines_test[0].erase(0, pos_test + delimiter.length());
    }

    cout << "Training random forest..." << endl;

    // créer le dataset train
    unsigned int row_count = lines.size();
    unsigned int col_count = l1t.size() - 1; // on ne considère pas les ids
    unsigned int feature_count = col_count - 1; // classe et les features

    // créer le dataset test
    unsigned int row_count_test = lines_test.size();

    // dataset train
    Dataset dsr( row_count, col_count );
    double label [row_count];

    // dataset test
    Dataset * ds_t = new Dataset(row_count_test, col_count);
    Dataset & ds_test = * ds_t;
    // store true labels
    double label_test [row_count_test];

    // données train
    // la classe est dans la deuxième colonne
    label[0] = atof(l1t[1].c_str());
    for (unsigned int col = 1; col < col_count; ++col)
        dsr[0][col] = atof(l1t[col+1].c_str());
    for ( unsigned int row = 1; row < row_count; ++row ) {
        // tokenize la ligne
        vector<string> tokens1;
        size_t pos1 = 0;
        std::string token1;
        while ((pos1 = lines[row].find(delimiter)) != std::string::npos) {
            token1 = lines[row].substr(0, pos1);
            tokens1.push_back(token1);
            lines[row].erase(0, pos1 + delimiter.length());
        }
        // la classe est dans la deuxième colonne
        label[row] = atof(tokens1[1].c_str());
        // on charge les features
        for (unsigned int col = 1; col < col_count; ++col)
            dsr[row][col] = atof(tokens1[col+1].c_str());
    }

    // données test
    // la classe est dans la deuxième colonne
    label_test[0] = atof(l1t_test[1].c_str());
    for (unsigned int col = 1; col < col_count; ++col)
        ds_test[0][col] = atof(l1t_test[col+1].c_str());
    for (unsigned int row = 1; row < row_count_test; ++row ) {
        // tokenize la ligne
        vector<string> tokens2;
        size_t pos2 = 0;
        std::string token2;
        while ((pos2 = lines_test[row].find(delimiter)) != std::string::npos) {
            token2 = lines_test[row].substr(0, pos2);
            tokens2.push_back(token2);
            lines_test[row].erase(0, pos2 + delimiter.length());
        }
        // la classe est dans la deuxième colonne
        label_test[row] = atof(tokens2[1].c_str());
        // on charge les features
        for (unsigned int col = 1; col < col_count; ++col)
            ds_test[row][col] = atof(tokens2[col+1].c_str());
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
    Dataset::KeyList split_keys = dsr.get_keys();
    split_keys.erase("class");


    // contient le plus grand nombre d'arbre ayant voté i pour un certain i
    int votes [row_count_test];
    // contient le i associé à la valeur dans vote
    double pred_labels [row_count_test];
    for (unsigned int row = 0; row < row_count_test; ++row) {
        votes[row] = 0;
        pred_labels[row] = 0;
    }

    // généralisation multiclass 1 vs. all

    // les différents classificateurs 1 vs. all seront dans app_forests

    cout << "Starting 1 vs. all classifications" << endl;

    for (int i=0; i < class_no;i++ ) {

        cout << i << endl;

        for (unsigned int row = 0; row < row_count ; ++row) {
            if (fabs(label[row] - double(i)) < 0.5)
                dsr[row][0] = 1.0;
            else
                dsr[row][0] = 0.0;
        }



        // Data should be loaded. Time to grow the forest.
        cout << "Grow..." << endl;
        RandomForest forest_classifier;

        forest_classifier.grow_forest(dsr, 0, (dsr.get_row()*4) / 5, split_keys, 32, this->trees_per_forest );

        // cout << forest_classifier.get_forest()[0]->draw() << endl;

        cout << "Grown!" << endl;

        cout << "Classifying..." << endl;

        for (unsigned int row = 0; row < row_count_test; ++row ) {
           int votes1 = forest_classifier.classify(ds_test[row]);
           if (votes1 > votes[row]){
              votes[row] = votes1;
              pred_labels[row] = i;
           }
        }

   }

    // return best class
    int result = 0;
    int count_max = 0;

    for (int i=0; i < class_no; i++) {
        int curr_count = 0;
        for (unsigned int row = 0; row < row_count_test; ++row ) {
            if (fabs(pred_labels[row] - i) < 0.5)
                curr_count += votes[row];
        }
        if (curr_count > count_max) {
            count_max = curr_count;
            result = i;
        }
    }


   cout << count_max << " " << row_count_test << endl;

   return result;

}

