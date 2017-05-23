#ifndef FILTER_H_
#define FILTER_H_

#include <unordered_set>

#include "query.h"
#include "field_type.h"
#include "reflection.hpp"

template<class T>
class FieldFilter {
public:
    typedef T value_type;
    virtual ~FieldFilter() {}

    virtual bool Rebuild(QueryDataType qdt, const std::vector<QueryDataValue>& data) = 0;
    virtual bool filter(value_type& value) = 0;
};

template<class T>
class RangeFilter : public FieldFilter<T> {
public:
    typedef T value_type;
    virtual bool Rebuild(QueryDataType qdt, const std::vector<QueryDataValue>& data) {
        if (data.size() < 2) {
            return false;
        }

        GetQueryDataValue<value_type>(qdt, data[0], begin_);
        GetQueryDataValue<value_type>(qdt, data[1], end_);

        return true;
    }

    virtual bool filter(value_type& value) {
        return (value > begin_ && value < end_);
    }

private:
    value_type begin_;
    value_type end_;
};

template<class T>
class NormalFilter : public FieldFilter<T> {
public:
    typedef T value_type;
    virtual bool Rebuild(QueryDataType qdt, const std::vector<QueryDataValue>& data) {
        if (data.size() < 1) {
            return false;
        }

        GetQueryDataValue<value_type>(qdt, data[0], value_);
        return true;
    }

    virtual bool filter(value_type& value) = 0;

protected:
    value_type value_;
};

template<class T>
class SetFilter : public FieldFilter<T> {
public:
    typedef T value_type;
    virtual bool Rebuild(QueryDataType qdt, const std::vector<QueryDataValue>& data) {
        if (data.empty()) {
            return false;
        }

        value_type temp;
        for (auto& data_val : data) {
            GetQueryDataValue<value_type>(qdt, data_val, temp);
            set_value_.insert(temp);
        }

        return true;
    }

    virtual bool filter(value_type& value) = 0;

protected:
    std::unordered_set<value_type> set_value_;
};

template<class T>
class EqualFilter : public NormalFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(value_type& value) {
        return value == this->value_;
    }
};

template<class T>
class NotEqualFilter : public NormalFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(value_type& value) {
        return value != this->value_;
    }
};

template<class T>
class AndFilter : public NormalFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(value_type& value) {
        return value & this->value_;
    }
};


template<class T>
class InFilter : public SetFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(value_type& value) {
        return this->set_value_.find(value) != this->set_value_.end();
    }
};

template<class T>
class NotInFilter : public SetFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(value_type& value) {
        return this->set_value_.find(value) == this->set_value_.end();
    }
};

// 注册支持的 int32 支持的Filter类型
int int32_reflection = []{
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
#endif // FILTER_H_
