#include <iostream>
#include <map>
#include <fstream>
#include <queue>
#include <stdlib.h>

#include "randomforest.h"

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

int main( const int argc, const char ** argv ) {

  // Load in dataset.
  string line;
  vector<string> lines;

  ifstream file( "filepath", ios_base::in );

  while ( getline(file, line, '\n') )
    lines.push_back( line );

  // Tokenize first line.
  vector<string> l1t = Tokenize(lines[0], "\t");

  // Create dataset.
  unsigned int row_count = lines.size();
  unsigned int col_count = l1t.size() - 1; // ignore ID
  unsigned int feature_count = col_count; // Class and n-1 features.

  // Dataset.
  Dataset dsr( row_count, col_count );

  // Convert data.
  for ( unsigned int row = 0; row < row_count; ++row ) {
    // Tokenize row.
    vector<string> tokens = Tokenize(lines[row], "\t");

    // First element is the ID. Skip.
    // Second element is the class

    // Fetch the features
    for ( unsigned int col = 0; col < col_count; ++col )
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
  int class_no = 2;

  // les différents classificateurs 1 vs. all
  queue<RandomForest*> forest;

  for (int i=0; i < class_no;i++ ) {

      // oversampling

      Dataset dsr_i( row_count, col_count );

      int freq_class_i = 0;
      vector<int> rows_i;

      for ( unsigned int row = 0; row < row_count; ++row ){
          if (fabs(dsr[row][0]- i) < 0.5){
              freq_class_i++;
              rows_i.push_back(row);
          }
      }

      freq_class_i /= row_count;

      for ( unsigned int row = 0; row < row_count; ++row ){
          if (fabs(dsr[row][0]- i) < 0.5){
              freq_class_i++;
              rows_i.push_back(row);
          }
      }

      // prepare the classes
      // 1 vs. all
      for ( unsigned int row = 0; row < row_count; ++row ){
        if (fabs(dsr[row][0]- i) < 0.5)
            dsr[row][0] = 1.0;
        else
            dsr[row][0] = 0.0;
      }

      // Data should be loaded. Time to grow the forest.
      cout << "Grow..." << endl;
      RandomForest forest_classifier;

      forest_classifier.grow_forest(dsr, 0, (dsr.get_row()*4) / 5, split_keys, 32, 300 );

      forest.push(&forest_classifier);

      cout << "Grown!" << endl;

      // Classify the training data.
      unsigned int tp = 0;
      unsigned int tn = 0;
      unsigned int fp = 0;
      unsigned int fn = 0;

      for ( unsigned int row = 0; row < dsr.get_row(); ++row) {
        unsigned int votes1 = forest_classifier.classify(dsr[row]);
        bool c = votes1 > dsr.get_row()-votes1;
        bool t = dsr[row][0] == 1.0;

        if ( c && t ) ++tp;
        else if ( c && !t ) ++fp;
        else if ( !c && t ) ++fn;
        else if ( !c && !t ) ++ tn;
        else cout << "????" << endl;

      }

      double accuracy = (tp + tn) * 100.0 / (tp + fp + tn + fn);
      double precision = tp * 100.0 / (tp + fp);
      double true_negative_rate = tn * 100.0 / (tn + fp);
      double recall = tp * 100.0 / (tp + fn);
      cout
        << "-----------------------------------------------------------------------\n"
        << "Training Set Classification " << i << " :\n"
        << "-----------------------------------------------------------------------\n"
        << "tp,fp,tn,fn       : " << tp << ", " << fp << ", " << tn << ", " << fn << "\n"
        << "Accuracy          : " << accuracy << "%\n"
        << "Precision         : " << precision << "%\n"
        << "True negative rate: " << true_negative_rate << "%\n"
        << "Recall            : " << recall << "%" << endl;

  }

  // Load testing data.

  // Load in dataset.
  lines.clear();
  ifstream file_tr( "data/wgss_test.csv", ios_base::in );
  while ( getline(file_tr, line, '\n') )
    lines.push_back( line );

  // Tokenize first line.
  l1t.clear();
  l1t = Tokenize(lines[0], "\t");

  // Create dataset.
  row_count = lines.size();
  col_count = l1t.size() - 1; // Ignore first (ID) column and last (?) col.
  feature_count = col_count - 1; // Class and n-1 features.

  // Dataset.
  Dataset dsr_test( row_count, col_count );

  // Convert data.
  for ( unsigned int row = 0; row < row_count; ++row ) {
    // Tokenize row.
    vector<string> tokens = Tokenize(lines[row], "\t");

    // First element is the ID. Skip.
    // Second element is the class

    // Fetch the rest of the features.
    for ( unsigned int col = 1; col < col_count; ++col )
      dsr_test[row][col] = atof(tokens[col + 1].c_str());
  }

  // key = instance, value = max number of votes of a class for this instance
  std::map<int, unsigned int> votes;
  // key = instance, value = class
  std::map<int, int> labels;
  for (unsigned int row = 0; row < dsr_test.get_row(); ++row) {
      votes[row] = 0;
      labels[row] = 0;
  }

  for (int i=0; i < class_no;i++ ){

      // prepare the classes
      // 1 vs. all
      for ( unsigned int row = 0; row < row_count; ++row ){
        if (fabs(dsr_test[row][0]- i) < 0.5)
            dsr_test[row][0] = 1.0;
        else
            dsr_test[row][0] = 0.0;
      }

      // Classify the training data.
      unsigned int tp = 0;
      unsigned int tn = 0;
      unsigned int fp = 0;
      unsigned int fn = 0;

      RandomForest* classifier = forest.front();
      forest.pop();

      for ( unsigned int row = 0; row < dsr_test.get_row(); ++row ) {
        unsigned int votes1 = classifier->classify(dsr_test[row]);
        if (votes1 > votes[row]){
              votes[row] = votes1;
              labels[row] = i;
        }

        bool c = 2*votes1 > dsr_test.get_row();
        bool t = dsr_test[row][0] == 1.0;

        if ( c && t ) ++tp;
        else if ( c && !t ) ++fp;
        else if ( !c && t ) ++fn;
        else if ( !c && !t ) ++ tn;
        else cout << "????" << endl;
      }

      double accuracy = (tp + tn) * 100.0 / (tp + fp + tn + fn);
      double precision = tp * 100.0 / (tp + fp);
      double true_negative_rate = tn * 100.0 / (tn + fp);
      double recall = tp * 100.0 / (tp + fn);
      cout
        << "-----------------------------------------------------------------------\n"
        << "Testing Set Classification:\n"
        << "-----------------------------------------------------------------------\n"
        << "tp,fp,tn,fn       : " << tp << ", " << fp << ", " << tn << ", " << fn << "\n"
        << "Accuracy          : " << accuracy << "%\n"
        << "Precision         : " << precision << "%\n"
        << "True negative rate: " << true_negative_rate << "%\n"
        << "Recall            : " << recall << "%" << endl;
  }

  return 0;

}
