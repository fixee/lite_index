#ifndef FIELD_TYPE_H_
#define FIELD_TYPE_H_

#include "query.h"

template<QueryDataType QDT>
struct FieldDataType {};

template<>
struct FieldDataType<QDT_INT32>{
    typedef int32_t value_type;
};

template<>
struct FieldDataType<QDT_INT64>{
    typedef int64_t value_type;
};

template<>
struct FieldDataType<QDT_FLOAT>{
    typedef float value_type;
};

template<>
struct FieldDataType<QDT_DOUBLE>{
    typedef double value_type;
};

template<>
struct FieldDataType<QDT_BOOL>{
    typedef bool value_type;
};

class FieldInterface {
public:
    virtual bool Init(bool use_index = false) = 0;
    virtual bool Support(QueryType qt, const QueryData& data) = 0;
    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) = 0;
};

#endif // FIELD_TYPE_H_
