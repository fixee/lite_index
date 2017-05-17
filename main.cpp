#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <boost/dynamic_bitset.hpp>

#include "reflection.hpp"
#include "query.h"
#include "field.h"

using namespace std;

int main(int argc, char** argv) {

    CommField<QDT_INT32> field;
    field.set_size(1024);
    field.Init();

    for (int i = 0; i < 1024; ++i) {
        field.set_value(i, i);
    }

    QueryData query_data;
    query_data.type = QDT_INT32;

    QueryDataValue start;
    start.int32_value = 1;
    query_data.values.push_back(start);

    QueryDataValue end;
    end.int32_value = 12;
    query_data.values.push_back(end);

    QueryType query_type = QT_AND;

    bool support = field.Support(query_type, query_data);
    cout << "support:" << support << endl;

    boost::dynamic_bitset<> bitset(1024);
    field.trigger(query_type, query_data, bitset);
    cout << "count:" << bitset.count() << endl;
    cout << "size:" << bitset.size() << endl;

    //for (size_t index = bitset.find_first(); 
    //     index != boost::dynamic_bitset<>::npos; 
    //     index = bitset.find_next(index)) {
    //    cout << index << endl;
    //}
    
    return 0;
}
