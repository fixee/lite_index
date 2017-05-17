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


#endif // FIELD_TYPE_H_
