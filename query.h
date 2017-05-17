#ifndef QUERY_H_
#define QUERY_H_

#include <string>
#include <vector>

typedef std::string string_t;

enum QueryDataType {
    QDT_INT32     = 0 ,
    QDT_INT64     ,
    QDT_FLOAT     ,
    QDT_DOUBLE    ,
    QDT_BOOL      ,

    QDT_MAX
}; 

union QueryDataValue {
    int32_t      int32_value;
    int64_t      int64_value;
    float        float_value;
    double       double_value;
    bool         bool_value;
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
    string_t     name;
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
bool GetQueryDataValue(QueryDataType qt, QueryDataValue qd_value, T& value) {
    switch (qt) {
        case QDT_INT32:
            value = qd_value.int32_value;
            return true;
        case QDT_INT64:
            value = qd_value.int64_value;
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
