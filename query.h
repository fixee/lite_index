#ifndef QUERY_H_
#define QUERY_H_

#include <string>
#include <vector>

typedef struct c_str_t {
    char *data;
    size_t size;
} c_str_t;

typedef c_str_t string_t;

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

union QueryDataValue {
    int32_t      int32_value;
    uint32_t     uint32_value;
    int64_t      int64_value;
    uint64_t     uint64_value;
    float        float_value;
    double       double_value;
    bool         bool_value;
    c_str_t      string_value;
};

struct QueryData {
    QueryDataType                 type;
    std::vector<QueryDataValue> values;
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

struct Query
{
    std::vector<QueryItem> should;
    std::vector<QueryItem> must;
    std::vector<QueryItem> must_not;
};

template<typename T>
bool GetQueryDataValue(QueryDataType qdt, QueryDataValue qd_value, T& value) {
    switch (qdt) {
        case QDT_INT32:
            value = qd_value.int32_value;
            return true;
        case QDT_UINT32:
            value = qd_value.uint32_value;
            return true;
        case QDT_INT64:
            value = qd_value.int64_value;
            return true;
        case QDT_UINT64:
            value = qd_value.uint64_value;
            return true;
        case QDT_FLOAT:
            value = qd_value.float_value;
            return true;
        case QDT_DOUBLE:
            value = qd_value.double_value;
            return true;
        case QDT_BOOL:
            value = qd_value.bool_value;
            return true;
        default:
            return false;
    }
}

#endif // QUERY_H_
