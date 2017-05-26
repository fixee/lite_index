#ifndef FIELD_CONFIG_H_
#define FIELD_CONFIG_H_

#include <string>
#include <map>

#include "document.pb.h"

enum INDEX_FIELD_TYPE {
    INT32 = 0   ,
    UINT32      ,
    INT64       ,
    UINT64      ,
    FLOAT       ,
    DOUBLE      ,
    BOOL        ,
    STRING      ,
    FEATURE     ,

    ARR_INT32   ,
    ARR_UINT32  ,
    ARR_INT64   ,
    ARR_UINT64  ,
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
    { "keywords",               ARR_FEATURE,        true             },
    { "categories",             ARR_FEATURE,        true             },
    { "attributes",             FEATURE,            true             },
    { "isdel",                  BOOL,               false            },
    { "id",                     STRING,             false            },
    { "feedback_gmp",           DOUBLE,             false            },
    { "publish_time",           INT64,              false            },
    { "ncid",                   STRING,             false            },
    { "update_time",            INT64,              false            },
    { "product_id",             ARR_STRING,         true             },
    { "update_type",            INT32,		        false            },
    { "expire_age_hour",        INT32,		        false            },
    { "build_count",            INT32,		        false            },
    { "multi_gmp",              ARR_DOUBLE,		    false            },
    { "expire_time",            INT64,		        false            },
    { "feed_version",           INT64,		        false            },
    { "topics",                 ARR_FEATURE,		true             },
    { "build_inc",              BOOL,		        false            },
    { "gmp_update",             INT64,		        false            },
    { "ventities",              ARR_FEATURE,		true             },
    { "lr_fea_hashes",          ARR_UINT64,		    false            },
    { "title_entities",         ARR_FEATURE,		true             },
    { "quality",                DOUBLE,		        false            },
    { "newsy_score",            DOUBLE,		        false            },
    { "publisher_hash",         UINT64,		        false            },
    { "groupid_hash",           UINT64,		        false            },
    { "cp_expire_type",         UINT32,		        false            },
    { "media_level",            UINT64,		        false            },
    { "real_title_entities",    ARR_FEATURE,		true             },
    { "ve_re_te_intersection",  ARR_FEATURE,		true             }
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
