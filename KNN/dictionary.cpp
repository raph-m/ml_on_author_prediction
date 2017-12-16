#include "dictionary.h"
#include"distances.h"
#include<map>
#include<utility>
using namespace std;

Dictionary::Dictionary(Distances mydistance_, vector<double> Query_doc_){
    mydistance= mydistance_;
    Query_doc=Query_doc_;

}

void Dictionary::Push(const vector<double> current_doc)
{
   // mapOfdocs[current_doc]=mydistance(current_doc,Query_doc);
    mapOfdocs.insert(pair< vector <double>, double> (current_doc,mydistance(current_doc,Query_doc)));
}

void Dictionary::SortMap()
{

}
