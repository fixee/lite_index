#ifndef QUERY_H_
#define QUERY_H_

#include <string.h>

#include <string>
#include <vector>
#include <type_traits>

enum QueryDataType {
    QDT_INT32     = 0 ,
    QDT_UINT32    ,
    QDT_INT64     ,
    QDT_UINT64    ,
    QDT_FLOAT     ,
    QDT_DOUBLE    ,
    QDT_BOOL      ,
    QDT_STRING    ,

    QDT_MAX
}; 

typedef struct QueryDataValue {
    int   size;
    char* data;
} QueryDataValue;

struct QueryData {
    QueryDataType       type;
    QueryDataValue      value;
};

enum QueryType { 
    QT_RANGE        = 0,
    QT_EQUAL           ,
    QT_NOT_EQUAL       ,
    QT_AND             ,
    QT_IN              ,
    QT_NOT_IN          ,

    QT_MAX
};

struct QueryItem {
    std::string   name;
    QueryType     type;
    QueryData     data;
};

enum BoolOperationType {
    BOT_AND     = 0,
    BOT_OR         ,
    BOT_NOT        
};

struct Query {
    BoolOperationType bot;
    std::vector<QueryItem>  query_items;
    std::vector<Query>      sub_queries;
};

typedef uint32_t query_str_size_type;
template<typename T>
bool GetQueryDataValue(const QueryDataValue& qd_value, std::vector<T>& value) {
    int size = qd_value.size;
    char *p = qd_value.data;

    while (size > 0) {
        if (size >= sizeof(T)) {
            value.push_back(*(T*)p);
            size -= sizeof(T);
            p += sizeof(T);
        } else {
            return false;
        }
    }
    return true;
}

template<class T>
QueryDataValue MakeQueryDataValue(std::vector<T>& vec) {
    QueryDataValue value;
    value.size = vec.size() * sizeof(T);
    value.data = new char[value.size];
    memcpy(value.data, &vec[0], value.size);
    return value;
}

#endif // QUERY_H_
