#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <boost/dynamic_bitset.hpp>
#include <fstream>

#include "reflection.hpp"
#include "query.h"
#include "field.h"
#include "array_field.h"
#include "document.pb.h"
#include "field_config.h"

using namespace std;

int main(int argc, char** argv) {
    //// test field
    //CommField<int32_t> field;
    //field.set_size(1024);
    //field.Init(true);

    //for (int i = 0; i < 1024; ++i) {
    //    field.set_value(i, i);
    //}

    //QueryData query_data;
    //query_data.type = QDT_INT32;

    //QueryDataValue start;
    //start.int32_value = 1;
    //query_data.values.push_back(start);

    //QueryDataValue end;
    //end.int32_value = 12;
    //query_data.values.push_back(end);

    //QueryType query_type = QT_IN;

    //bool support = field.Support(query_type, query_data);
    //cout << "support:" << support << endl;

    //boost::dynamic_bitset<> bitset(1024);
    //field.trigger(query_type, query_data, bitset);
    //cout << "count:" << bitset.count() << endl;
    //cout << "size:" << bitset.size() << endl;

    //for (size_t index = bitset.find_first(); 
    //     index != boost::dynamic_bitset<>::npos; 
    //     index = bitset.find_next(index)) {
    //    cout << index << endl;
    //}
    
    // test proto_buffer
    using namespace index_system::pb;

    // read data
    std::ifstream ifs("data/786113550_27", std::ios::in | std::ios::binary);
    std::string data;
    data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    std::cout << data.size() << std::endl;

    Documents docs;
    bool ret = docs.ParseFromString(data);
    std::cout << "parse ret:" << ret << std::endl;
    std::cout << "document size:" << docs.documents_size() << std::endl;

    //typedef typename GetterFactory<int64_t>::GetterFunc int64_getter;
    typedef GetterFactory<int64_t>::GetterFunc int64_getter;
    typedef GetterFactory<std::string>::GetterFunc string_getter;

    int64_getter publish_time_getter;
    if (!GetterFactory<int64_t>::Instance().get("get_publish_time", publish_time_getter)) {
        std::cout << "field getter error: get_publish_time" << std::endl;
        return 1;
    }

    string_getter doc_id_getter;
    if (!GetterFactory<std::string>::Instance().get("get_doc_id", doc_id_getter)) {
        std::cout << "field getter error: get_doc_id" << std::endl;
        return 1;
    }

    for (int i = 0; i < docs.documents_size(); ++i) {
        const index_system::pb::Document& doc = docs.documents(i);
        std::cout << "doc_id:" << doc_id_getter(doc) << "    ";
        std::cout << "publish_time:" << publish_time_getter(doc) << std::endl;
    }

    return 0;
}
