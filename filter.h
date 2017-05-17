#ifndef FILTER_H_
#define FILTER_H_

#include <unordered_set>

#include "query.h"
#include "field_type.h"
#include "reflection.hpp"

template<QueryDataType QDT>
class FieldFilter {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;
    virtual ~FieldFilter() {}

    virtual bool Rebuild(const std::vector<QueryDataValue>& data) = 0;
    virtual bool filter(value_type& value) = 0;
};

template<QueryDataType QDT>
class RangeFilter : public FieldFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool Rebuild(const std::vector<QueryDataValue>& data) {
        if (data.size() < 2) {
            return false;
        }

        GetQueryDataValue<value_type>(QDT, data[0], begin_);
        GetQueryDataValue<value_type>(QDT, data[1], end_);

        return true;
    }

    virtual bool filter(value_type& value) {
        return (value > begin_ && value < end_);
    }

private:
    value_type begin_;
    value_type end_;
};

template<QueryDataType QDT>
class NormalFilter : public FieldFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool Rebuild(const std::vector<QueryDataValue>& data) {
        if (data.size() < 1) {
            return false;
        }

        GetQueryDataValue<value_type>(QDT, data[0], value_);
        return true;
    }

    virtual bool filter(value_type& value) = 0;

protected:
    value_type value_;
};

template<QueryDataType QDT>
class SetFilter : public FieldFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool Rebuild(const std::vector<QueryDataValue>& data) {
        if (data.empty()) {
            return false;
        }

        value_type temp;
        for (auto& data_val : data) {
            GetQueryDataValue<value_type>(QDT, data_val, temp);
            set_value_.insert(temp);
        }

        return true;
    }

    virtual bool filter(value_type& value) = 0;

protected:
    std::unordered_set<value_type> set_value_;
};

template<QueryDataType QDT>
class EqualFilter : public NormalFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool filter(value_type& value) {
        return value == this->value_;
    }
};

template<QueryDataType QDT>
class NotEqualFilter : public NormalFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool filter(value_type& value) {
        return value != this->value_;
    }
};

template<QueryDataType QDT>
class AndFilter : public NormalFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool filter(value_type& value) {
        return value & this->value_;
    }
};


template<QueryDataType QDT>
class InFilter : public SetFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool filter(value_type& value) {
        return this->set_value_.find(value) != this->set_value_.end();
    }
};

template<QueryDataType QDT>
class NotInFilter : public SetFilter<QDT> {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    virtual bool filter(value_type& value) {
        return this->set_value_.find(value) == this->set_value_.end();
    }
};

// 注册支持的 QDT_INT32 支持的Filter类型
int _QDT_INT32_reflection = []{
    typedef FieldFilter<QDT_INT32> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<QDT_INT32>>());
    Factory<QueryType,If>::register_object(QT_EQUAL, new ObjectGen<If, EqualFilter<QDT_INT32>>());
    Factory<QueryType,If>::register_object(QT_NOT_EQUAL, new ObjectGen<If, NotEqualFilter<QDT_INT32>>());
    Factory<QueryType,If>::register_object(QT_AND, new ObjectGen<If, AndFilter<QDT_INT32>>());
    Factory<QueryType,If>::register_object(QT_IN, new ObjectGen<If, InFilter<QDT_INT32>>());
    Factory<QueryType,If>::register_object(QT_NOT_IN, new ObjectGen<If, NotInFilter<QDT_INT32>>());

    return 0;
}();

// 注册支持的 QDT_INT64 支持的Filter类型
int _QDT_INT64_reflection = []{
    typedef FieldFilter<QDT_INT64> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<QDT_INT64>>());
    Factory<QueryType,If>::register_object(QT_EQUAL, new ObjectGen<If, EqualFilter<QDT_INT64>>());
    Factory<QueryType,If>::register_object(QT_NOT_EQUAL, new ObjectGen<If, NotEqualFilter<QDT_INT64>>());
    Factory<QueryType,If>::register_object(QT_AND, new ObjectGen<If, AndFilter<QDT_INT64>>());
    Factory<QueryType,If>::register_object(QT_IN, new ObjectGen<If, InFilter<QDT_INT64>>());
    Factory<QueryType,If>::register_object(QT_NOT_IN, new ObjectGen<If, NotInFilter<QDT_INT64>>());

    return 0;
}();

// 注册支持的 QDT_FLOAT 支持的Filter类型
int _QDT_FLOAT_reflection = []{
    typedef FieldFilter<QDT_FLOAT> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<QDT_FLOAT>>());

    return 0;
}();

// 注册支持的 QDT_DOUBLE 支持的Filter类型
int _QDT_DOUBLE_reflection = []{
    typedef FieldFilter<QDT_DOUBLE> If;

    Factory<QueryType,If>::register_object(QT_RANGE, new ObjectGen<If, RangeFilter<QDT_DOUBLE>>());

    return 0;
}();

// 注册支持的 QDT_INT64 支持的Filter类型
int _QDT_BOOL_reflection = []{
    typedef FieldFilter<QDT_BOOL> If;

    Factory<QueryType,If>::register_object(QT_EQUAL, new ObjectGen<If, EqualFilter<QDT_BOOL>>());
    Factory<QueryType,If>::register_object(QT_NOT_EQUAL, new ObjectGen<If, NotEqualFilter<QDT_BOOL>>());

    return 0;
}();
#endif // FILTER_H_
