#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <boost/dynamic_bitset.hpp>
#include <fstream>

#include "reflection.hpp"
#include "query.h"
#include "field.h"
#include "document.pb.h"
#include "array_field.h"
#include "field_config.h"
#include "search_index.h"

using namespace std;

int main(int argc, char** argv) {

    SearchIndex engine;
    std::string err_msg;
    bool ret = engine.Init(&err_msg);
    if (!ret) {
        std::cout << "search_index inti error, err_msg:";
        std::cout << err_msg << endl;
        return 1;
    }

    // 大括号限制生命周期， 及时释放内存
    {
        std::ifstream ifs("data/786113550_27", std::ios::in | std::ios::binary);
        std::string data;
        data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        std::cout << data.size() << std::endl;

        index_system::pb::Documents docs;
        ret = docs.ParseFromString(data);
        std::cout << "parse ret:" << ret << std::endl;
        std::cout << "document size:" << docs.documents_size() << std::endl;

        engine.Reset(docs.documents_size());
        for (int i = 0; i < docs.documents_size(); ++i) {
            engine.InsertDocument(docs.documents(i));
        }
    }
    std::cout << "search_index load succ!" << std::endl;

    //c_str_t str_value;
    //str_value.data = const_cast<char*>("14");
    //str_value.size = 2;
    //QueryDataValue pid;
    //pid.string_value = str_value;

    std::vector<std::string> pid_list = { "14" };

    QueryItem item;
    item.name = "product_id";
    item.type = QT_EQUAL;
    item.data.type = QDT_STRING;
    item.data.value = MakeQueryDataValue<std::string>(pid_list);

    Query query;
    query.should.push_back(item);

    std::vector<int> result;
    ret = engine.Trigger(query, result);
    if (!ret) {
        std::cout << "trigger not supported!" << std::endl;
        return 1;
    }

    std::cout << "trigger succ, result_size:" << result.size() << std::endl;

    return 0;
}
