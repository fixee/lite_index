#ifndef FIELD_TYPE_H_
#define FIELD_TYPE_H_

#include <string>

#include "query.h"

// BEGIN 定义一些特殊的字段类型
// 支持普通的类型有: int32_t, int64_t, float, double, bool
typedef struct feature_t {
    std::string name;
    double      score;
} feature_t;
// END 定义一些特殊的字段类型

// BEGIN 定义field pre processor
template<class T>
class FieldPreProcessor {
public:
    typedef T value_type;
    value_type& GetValue(T& val) {
        return val;
    }
};

template<>
class FieldPreProcessor<feature_t> {
public:
    typedef std::string value_type;
    value_type& GetValue(feature_t& val) {
        return val.name;
    }
};
// END 定义field pre processor

// BEGIN 定义各种类型支持的查询数据类型
template<class T>
class SupportQueryDataType {};

template<>
class SupportQueryDataType<int32_t> {
public:
    static const QueryDataType value = QDT_INT32;
};

template<>
class SupportQueryDataType<int64_t> {
public:
    static const QueryDataType value = QDT_INT64;
};

template<>
class SupportQueryDataType<float> {
public:
    static const QueryDataType value = QDT_FLOAT;
};

template<>
class SupportQueryDataType<double> {
public:
    static const QueryDataType value = QDT_DOUBLE;
};

template<>
class SupportQueryDataType<bool> {
public:
    static const QueryDataType value = QDT_BOOL;
};

template<>
class SupportQueryDataType<std::string> {
public:
    static const QueryDataType value = QDT_STRING;
};

template<>
class SupportQueryDataType<feature_t> {
public:
    static const QueryDataType value = QDT_STRING;
};
// END 定义各种类型支持的查询数据类型

class FieldInterface {
public:
    virtual bool Init(bool use_index = false) = 0;

    /* desc: check if the query is supported.
     * return val: 0, not supported;
     *             1, supported by filter;
     *             2, supported by index;
     */
    virtual int Support(QueryType qt, const QueryData& data) = 0;

    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) = 0;
};

#endif // FIELD_TYPE_H_
