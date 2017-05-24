#ifndef FIELD_CONFIG_H_
#define FIELD_CONFIG_H_

#include <string>
#include <map>

typedef struct field_config_t {
    std::string name;
    std::string type;
    std::string convert_func;
} field_config_t;

template<class T>
class GetterFactory {
public:
    typedef T (*GetterFunc)(const index_system::pb::Document&);

    static GetterFactory& Instance() {
        static GetterFactory _inst;
        return _inst;
    }

    bool set(const std::string& name, GetterFunc func) {
        map_[name] = func;
        return true;
    }

    bool get(const std::string& name, GetterFunc &func) {
        auto it = map_.find(name);
        if (it == map_.end()) return false;
        func = it->second;
        return true;
    }

    ~GetterFactory() {}

private:
    GetterFactory() {}
    GetterFactory(GetterFactory&) = delete;
    
    std::map<std::string, GetterFunc> map_;   
};

static const field_config_t fields[] = {
    { "doc_id", "int32", "get_doc_id" }
};

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

#endif // FIELD_CONFIG_H_
