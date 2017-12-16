#ifndef DICTIONARY_H
#define DICTIONARY_H
#include<vector>
#include "distances.h"
#include<map>
#include<iterator>
using namespace std;


class Dictionary
{
public:
    Dictionary(Distances mydistance_,vector<double> Query_doc_);
    void Push(const vector<double> current_doc);
    void SortMap();
    ~Dictionary();

private:
    map<vector<double>, double> mapOfdocs;
    Distances mydistance;
    vector<double> Query_doc;
};

#endif // DICTIONARY_H
