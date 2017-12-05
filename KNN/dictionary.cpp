#include "dictionary.h"

Dictionary::Dictionary(Distances mydistance_, vector<long> Query_doc_){
    mydistance=mydistance;
    Query_doc=Query_doc_;

}
void Dictionary::Push(const vector<long>& current_doc) const
{
    mapOfdocs[current_doc]=mydistance(current_doc,Query_doc);
}
void Dictionary::SortMap()
{

}
