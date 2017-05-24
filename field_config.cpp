#include "field_config.h"

//const std::map<std::string, int> FIELD_SLOT_MAP = [] {
//    std::map<std::string, int> ret;
//    for (int i = 0; i < INDEX_FIELD_SIZE; ++i) {
//        ret.insert(std::pair<std::string,int>(INDEX_FIELDS[i].name, i));
//    }
//    return ret;
//}();

//// BEGIN 与定义的各种get 函数

// 获取文章的doc_id
std::string get_doc_id(const index_system::pb::Document& doc) {
    return doc.id();
}

// 获取文章 publish_time
int64_t get_publish_time(const index_system::pb::Document& doc) {
    return doc.publish_time();
}

// 注册factory 函数
static bool __get_doc_id_func = GetterFactory<std::string>::Instance().set("get_doc_id", &get_doc_id);
static bool __get_publish_time = GetterFactory<int64_t>::Instance().set("get_publish_time", &get_publish_time);
