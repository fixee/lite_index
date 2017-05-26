#include <dirent.h>

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
        DIR *p_dir;
        struct dirent *p_dirent;
        std::vector<std::string> file_names;

        p_dir = opendir("data");
        if(NULL != p_dir) {
            while((p_dirent = readdir(p_dir)) != NULL) {
                if(strcmp( p_dirent->d_name, ".") == 0) { continue; }
                if(strcmp( p_dirent->d_name, "..") == 0) { continue; }

                file_names.push_back(std::string("data/").append(p_dirent->d_name));
            }

            closedir(p_dir);
        }

        size_t total_doc_size = 0;
        for (auto &file_name : file_names) {
            std::string data;
            std::ifstream ifs(file_name.c_str(), std::ios::in | std::ios::binary);
            data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            index_system::pb::Documents docs;
            ret = docs.ParseFromString(data);
            total_doc_size += docs.documents_size();
        }

        engine.Reset(total_doc_size);
        std::cout << "total_doc_size:" << total_doc_size << std::endl;

        size_t load_doc_size = 0;
        for (auto &file_name : file_names) {
            std::string data;
            std::ifstream ifs(file_name.c_str(), std::ios::in | std::ios::binary);
            data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            index_system::pb::Documents docs;
            ret = docs.ParseFromString(data);
            for (int i = 0; i < docs.documents_size(); ++i) {
                engine.InsertDocument(docs.documents(i));
            }

            load_doc_size += docs.documents_size();
            std::cout << "load file_name:" << file_name << "    ";
            std::cout << "loaded doc size:" << load_doc_size << std::endl;
        }
    }

    std::cout << "search_index load succ!" << std::endl;
    sleep(1000000);

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
