#include "filter.h"

// 注册支持的 int32 支持的Filter类型
int int32_reflection = [] {
    typedef FieldFilter<int32_t> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<int32_t>>());
    Factory<QueryType,If>::register_object(QT_EQUAL, new ObjectGen<If, EqualFilter<int32_t>>());
    Factory<QueryType,If>::register_object(QT_NOT_EQUAL, new ObjectGen<If, NotEqualFilter<int32_t>>());
    Factory<QueryType,If>::register_object(QT_AND, new ObjectGen<If, AndFilter<int32_t>>());
    Factory<QueryType,If>::register_object(QT_IN, new ObjectGen<If, InFilter<int32_t>>());
    Factory<QueryType,If>::register_object(QT_NOT_IN, new ObjectGen<If, NotInFilter<int32_t>>());

    return 0;
}();

// 注册支持的 uint64_t 支持的Filter类型
int int64_reflection = []{
    typedef FieldFilter<uint64_t> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<uint64_t>>());
    Factory<QueryType,If>::register_object(QT_EQUAL, new ObjectGen<If, EqualFilter<uint64_t>>());
    Factory<QueryType,If>::register_object(QT_NOT_EQUAL, new ObjectGen<If, NotEqualFilter<uint64_t>>());
    Factory<QueryType,If>::register_object(QT_AND, new ObjectGen<If, AndFilter<uint64_t>>());
    Factory<QueryType,If>::register_object(QT_IN, new ObjectGen<If, InFilter<uint64_t>>());
    Factory<QueryType,If>::register_object(QT_NOT_IN, new ObjectGen<If, NotInFilter<uint64_t>>());

    return 0;
}();

// 注册支持的 float 支持的Filter类型
int float_reflection = []{
    typedef FieldFilter<float> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<float>>());

    return 0;
}();

// 注册支持的 double 支持的Filter类型
int double_reflection = []{
    typedef FieldFilter<double> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<double>>());

    return 0;
}();

// 注册支持的 bool 支持的Filter类型
int bool_reflection = []{
    typedef FieldFilter<bool> If;

    Factory<QueryType,If>::register_object(QT_EQUAL, new ObjectGen<If, EqualFilter<bool>>());
    Factory<QueryType,If>::register_object(QT_NOT_EQUAL, new ObjectGen<If, NotEqualFilter<bool>>());

    return 0;
}();
