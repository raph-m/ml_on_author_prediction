#include "multiclass_rs.h"

using namespace std;

namespace std {

  vector<string> Tokenize(const string& str,const string& delimiters){
    vector<string> tokens;
    string::size_type delimPos = 0, tokenPos = 0, pos = 0;

    if(str.length() < 1)
      return tokens;

    while (true) {
      delimPos = str.find_first_of(delimiters, pos);
      tokenPos = str.find_first_not_of(delimiters, pos);

      // string::npos "until the end of the string"
      if(string::npos != delimPos) {
        if(string::npos != tokenPos) {
          if(tokenPos<delimPos)
            tokens.push_back(str.substr(pos,delimPos-pos));
          else
            tokens.push_back("");
        }
        else
          tokens.push_back("");

        pos = delimPos+1;
      }
      else
      {
        if(string::npos != tokenPos) {
          // substr until end of string
          tokens.push_back(str.substr(pos));
        }
        else
          tokens.push_back("");
        break;
      }
    }
    return tokens;
  }

}


void multiclass_rs::run_random_forest_train ( string train_filepath, string test_filepath) {

    // TRAIN
    // class_no classifiers 1 vs all



    cout << "Training random forest..." << endl;

    // Load in dataset.
    string line;
    vector<string> lines;

    ifstream file( train_filepath, ios_base::in );

    while ( getline(file, line, '\n') )
       lines.push_back( line );

    // Tokenize first line
    vector<string> l1t = Tokenize(lines[0], ",");

    // Create dataset.
    unsigned int row_count = lines.size();
    int col_count = l1t.size() - 1; // ignore ID
    unsigned int feature_count = col_count - 1; // ignore class and ID

    // Dataset.
    Dataset dsr( row_count, col_count );

    int labels [row_count];

    labels[0] = atof(l1t[1].c_str());
    for ( int col = 1; col < col_count; ++col )
      dsr[0][col] = atof(l1t[col + 1].c_str());

    // Convert data.

    for ( unsigned int row = 1; row < row_count; ++row ) {
        // Tokenize row.
        vector<string> tokens = Tokenize(lines[row], ",");

        // First element is the ID. Skip.
        // Class is the second row of the csv, first row of dsr
        // for now we don't migrate the classes
        // Fetch the features
        labels[row] = atof(l1t[1].c_str());
        for ( int col = 1; col < col_count; ++col )
          dsr[row][col] = atof(tokens[col + 1].c_str());
    }

    // Configure keys.
    Dataset::KeyList & keys = dsr.get_keys();
    keys["class"] = 0;
    for ( unsigned int feature = 1; feature <= feature_count; ++feature ){
        std::string feature_ident;
        feature_ident.append("f" + feature);
        keys[feature_ident] = feature;
    }

    // Determine split keys. All keys except class.
    Dataset::KeyList split_keys = dsr.get_keys();
    split_keys.erase("class");

    // généralisation multiclass 1 vs. all

    // les différents classificateurs 1 vs. all seront dans app_forests

    cout << "Starting 1 vs. all classifications" << endl;

    for (int i=0; i < class_no;i++ ) {

        cout << i << endl;

        for (unsigned int row = 0; row < row_count ; ++row) {
            if (labels[row] == i)
                dsr[row][0] = 1.0;
            else
                dsr[row][0] = 0.0;
        }

        // Data should be loaded. Time to grow the forest.
        cout << "Grow..." << endl;
        RandomForest forest_classifier;

        forest_classifier.grow_forest(dsr, 0, (dsr.get_row()*4) / 5, split_keys, 32, this->trees_per_forest );

        app_forests.push_back(&forest_classifier);

        cout << "Grown!" << endl;

   }

  // TEST
  // AND OUTPUT PERFORMANCE






  // Load in dataset.
  lines.clear();
  ifstream file_tr( test_filepath, ios_base::in );

  while ( getline(file_tr, line, '\n') )
    lines.push_back( line );

  // Tokenize first line.
  l1t.clear();
  l1t = Tokenize(lines[0], ",");

  // Create dataset.
  row_count = lines.size();
  col_count = l1t.size() - 1; // Ignore first (ID) column and last (?) col.
  feature_count = col_count - 1; // Class and n-1 features.

  // Dataset.
  Dataset * ds = null(Dataset);
  ds = new Dataset(row_count, col_count);

  for (int col = 0; col < col_count; ++col )
    (*ds)[0][col] = atof(l1t[col + 1].c_str());

  // store true label0
  double true_labels [row_count];

  // Convert data.
  for (unsigned int row = 1; row < row_count; ++row ) {

    // Tokenize row.
    vector<string> tokens = Tokenize(lines[row], ",");

    true_labels[row] = int(atof(tokens[1].c_str()));

    // First element is the ID. Skip.

    // Fetch the rest of the features.
    for (int col = 0; col < col_count; ++col )
      (*ds)[row][col] = atof(tokens[col + 1].c_str());
  }

  // key = instance, value = max number of votes of a class for this instance
  int votes [row_count];
  // key = instance, value = class
  double pred_labels [row_count];
  for (unsigned int row = 0; row < row_count; ++row) {
      votes[row] = 0;
      pred_labels[row] = 0;
  }

  for (int i=0; i < class_no;i++ ){

      // on classe l'instance avec les forêts entraînées

      RandomForest* classifier = app_forests[i];

      for (unsigned int row = 0; row < row_count; ++row ) {
        cout << "ici73" << endl;
        int votes1 = classifier->classify((*ds)[row]);
        cout << "ici72" << endl;
        if (votes1 > votes[row]){
              votes[row] = votes1;
              pred_labels[row] = i;
        }
      }

  }

  cout << "ici8" << endl;

  //Evaluate
  unsigned int quality = 0;

  for (unsigned int row =0; row < row_count; ++row) {

      // is the predicted class the right one?
      bool t = (true_labels[row] ==  pred_labels[row]);

      if (t) ++quality;

  }

  cout << quality / row_count << endl;

}

/**
 * This function takes into a csv made of blocks taken from an input text
 * Ignore two first columns (IDs and classes randomly placed)
 * @brief run_random_forest
 * @param text_filepath
 */

int multiclass_rs::run_random_forest (string text_filepath) {

    // Load in dataset.
    string line;
    vector<string> lines;

    ifstream file_tr( text_filepath, ios_base::in );
    while ( getline(file_tr, line, '\n') )
      lines.push_back( line );

    // Tokenize first line
    vector<string> l1t = Tokenize(lines[0], ",");

    // Create dataset.
    unsigned int row_count = lines.size();
    unsigned int col_count = l1t.size() - 2; // Ignore first (ID) column and class

    // Dataset.
    Dataset dsr_test( row_count, col_count );

    // Convert data.
    for ( unsigned int row = 0; row < row_count; ++row ) {
      // Tokenize row.
      vector<string> tokens = Tokenize(lines[row], ",");

      // First element is the ID. Skip.
      // Second element is the class. Skip

      // Fetch the features.
      for ( unsigned int col = 0; col < col_count; ++col )
        dsr_test[row][col] = atof(tokens[col + 2].c_str());
    }

    // key = instance, value = max number of votes of a class for this instance
    int votes [row_count];
    // key = instance, value = class
    int pred_labels [row_count];
    for (unsigned int row = 0; row < dsr_test.get_row(); ++row) {
        votes[row] = 0;
        pred_labels[row] = 0;
    }

    for (int i=0; i < class_no;i++ ) {

        // Classify the training data.

        RandomForest* classifier = app_forests[i];

        for ( unsigned int row = 0; row < dsr_test.get_row(); ++row ) {
          int votes1 = classifier->classify(dsr_test[row]);
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
        for (unsigned int row = 0; row < dsr_test.get_row(); ++row ) {
            if (pred_labels[row] == i)
                curr_count += votes[row];
        }
        if (curr_count > count_max) {
            count_max = curr_count;
            result = i;
        }
    }


   cout << count_max / row_count << endl;

   return result;

}
