#ifndef FIELD_TYPE_H_
#define FIELD_TYPE_H_

#include <string>
#include <boost/dynamic_bitset.hpp>

#include "query.h"
#include "document.pb.h"

// BEGIN 定义一些特殊的字段类型
// 支持普通的类型有: int32_t, uint32_t, int64_t, uint64_t, float, double, bool
typedef struct feature_t {
    uint64_t    name;
    double      score;
} feature_t;
// END 定义一些特殊的字段类型

// BEGIN 定义field pre processor
template<class T>
class FieldPreProcessor {
public:
    typedef T value_type;
    const value_type& operator()(const T& val) {
        return val;
    }
};

template<>
class FieldPreProcessor<feature_t> {
public:
    typedef uint64_t value_type;
    const value_type& operator()(const feature_t& val) {
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
class SupportQueryDataType<uint32_t> {
public:
    static const QueryDataType value = QDT_UINT32;
};

template<>
class SupportQueryDataType<int64_t> {
public:
    static const QueryDataType value = QDT_INT64;
};

template<>
class SupportQueryDataType<uint64_t> {
public:
    static const QueryDataType value = QDT_UINT64;
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
    static const QueryDataType value = QDT_UINT64;
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

    virtual bool Trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) = 0;

    virtual bool SetDocument(int pos, const index_system::pb::Document& doc) = 0;

    virtual void set_size(size_t size) = 0;

    virtual void clear() = 0;
};

#endif // FIELD_TYPE_H_
