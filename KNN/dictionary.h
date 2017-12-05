#ifndef DICTIONARY_H
#define DICTIONARY_H
#include<vector>
#include<distances.h>
#include<map>
#include<iterator>
using namespace std;


class Dictionary
{
public:
    Dictionary(Distances mydistance_,vector<long> Query_doc_);
    void Push(const vector<long>& current_doc)const;
    void SortMap();
    ~Dictionary();

private:
    map<vector<long>, long> mapOfdocs;
    Distances& mydistance;
    vector<long>& Query_doc;
};

#endif // DICTIONARY_H
