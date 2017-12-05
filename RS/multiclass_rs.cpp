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
    unsigned int col_count = l1t.size() - 1; // ignore ID
    unsigned int feature_count = col_count - 1; // ignore class and ID

    // Dataset.
    Dataset dsr( row_count, col_count );

    // Convert data.

    for ( unsigned int row = 0; row < row_count; ++row ) {
        // Tokenize row.
        vector<string> tokens = Tokenize(lines[row], ",");

        // First element is the ID. Skip.
        // Class is the second row of the csv, first row of dsr
        // for now we don't migrate the classes
        // Fetch the features

        for ( unsigned int col = 1; col < col_count; ++col )
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

    for (int i=0; i < class_no;i++ ) {

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

       // Data should be loaded. Time to grow the forest.
       cout << "Grow..." << endl;
       RandomForest forest_classifier;

       forest_classifier.grow_forest(dsr_i, 0, (dsr_i.get_row()*4) / 5, split_keys, 32, 300 );

       app_forests->push_back(&forest_classifier);

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
  Dataset dsr_test( row_count, col_count );

  // store true labels
  vector<int> true_labels;

  // Convert data.
  for ( unsigned int row = 0; row < row_count; ++row ) {
    // Tokenize row.
    vector<string> tokens = Tokenize(lines[row], ",");

    true_labels[row] = atof(tokens[1].c_str());

    // First element is the ID. Skip.
    // Second element is the class, we'll fetch 'em later

    // Fetch the rest of the features.
    for ( unsigned int col = 1; col < col_count; ++col )
      dsr_test[row][col] = atof(tokens[col + 1].c_str());
  }

  // key = instance, value = max number of votes of a class for this instance
  std::map<int, unsigned int> votes;
  // key = instance, value = class
  std::map<int, int> pred_labels;
  for (unsigned int row = 0; row < dsr_test.get_row(); ++row) {
      votes[row] = 0;
      pred_labels[row] = 0;
  }

  for (int i=0; i < class_no;i++ ){

      // on classe l'instance avec les forêts entraînées

      RandomForest* classifier = (*app_forests)[i];

      for ( unsigned int row = 0; row < dsr_test.get_row(); ++row ) {
        unsigned int votes1 = classifier->classify(dsr_test[row]);
        if (votes1 > votes[row]){
              votes[row] = votes1;
              pred_labels[row] = i;
        }
      }

  }

  //Evaluate
  unsigned int quality = 0;

  for (unsigned int row =0; row < dsr_test.get_row(); ++row) {

      // is the predicted class the right one?
      bool t = (true_labels[row] ==  pred_labels[row]);

      if (t) ++quality;

  }

  cout << quality / dsr_test.get_row() << endl;

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
    std::map<int, unsigned int> votes;
    // key = instance, value = class
    std::map<int, int> pred_labels;
    for (unsigned int row = 0; row < dsr_test.get_row(); ++row) {
        votes[row] = 0;
        pred_labels[row] = 0;
    }

    for (int i=0; i < class_no;i++ ) {

        // Classify the training data.

        RandomForest* classifier = (*app_forests)[i];

        for ( unsigned int row = 0; row < dsr_test.get_row(); ++row ) {
          unsigned int votes1 = classifier->classify(dsr_test[row]);
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
        for ( unsigned int row = 0; row < dsr_test.get_row(); ++row ) {
            if (votes[row] == i)
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
