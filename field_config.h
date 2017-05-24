#ifndef FIELD_CONFIG_H_
#define FIELD_CONFIG_H_

#include <string>
#include <map>

#include "document.pb.h"

enum INDEX_FIELD_TYPE {
    INT32 = 0   ,
    INT64       ,
    FLOAT       ,
    DOUBLE      ,
    BOOL        ,
    STRING      ,
    FEATURE     ,

    ARR_INT32   ,
    ARR_INT64   ,
    ARR_FLOAT   ,
    ARR_DOUBLE  ,
    ARR_BOOL    ,
    ARR_STRING  ,
    ARR_FEATURE ,

    INDEX_FIELD_TYPE_MAX
};

typedef struct field_config_t {
    std::string         name;
    INDEX_FIELD_TYPE    type;
    std::string         convert_func;
    bool                index;
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

static const field_config_t INDEX_FIELDS[] = {
    { "doc_id",         STRING,         "get_doc_id",       false            },
    { "publish_time",   INT64,          "get_publish_time", false            }
};

static const size_t INDEX_FIELD_SIZE = sizeof(INDEX_FIELDS) / sizeof(INDEX_FIELDS[0]);

static const std::map<std::string, int> FIELD_SLOT_MAP = [] {
    std::map<std::string, int> ret;
    for (int i = 0; i < INDEX_FIELD_SIZE; ++i) {
        ret.insert(std::pair<std::string,int>(INDEX_FIELDS[i].name, i));
    }
    return ret;
}();

#endif // FIELD_CONFIG_H_
