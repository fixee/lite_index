#ifndef FILTER_H_
#define FILTER_H_

#include <unordered_set>

#include "query.h"
#include "reflection.hpp"

template<class T>
class FieldFilter {
public:
    typedef T value_type;
    virtual ~FieldFilter() {}

    virtual bool Rebuild(QueryDataType qdt, const QueryDataValue& data) = 0;
    virtual bool filter(const value_type& value) = 0;
};

template<class T>
class RangeFilter : public FieldFilter<T> {
public:
    typedef T value_type;
    virtual bool Rebuild(QueryDataType qdt, const QueryDataValue& data) {
        std::vector<value_type> vec;
        if (!GetQueryDataValue<value_type>(data, vec) || vec.size() < 2)
            return false;

        begin_ = vec[0];
        end_ = vec[1];
        return true;
    }

    virtual bool filter(const value_type& value) {
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
    virtual bool Rebuild(QueryDataType qdt, const QueryDataValue& data) {
        std::vector<value_type> vec;
        if (!GetQueryDataValue<value_type>(data, vec) || vec.empty() )
            return false;

        value_ = vec[0];
        return true;
    }

    virtual bool filter(const value_type& value) = 0;

protected:
    value_type value_;
};

template<class T>
class SetFilter : public FieldFilter<T> {
public:
    typedef T value_type;
    virtual bool Rebuild(QueryDataType qdt, const QueryDataValue& data) {
        std::vector<value_type> vec;
        if (!GetQueryDataValue<value_type>(data, vec) || vec.empty() )
            return false;

        for (auto& val : vec) {
            set_value_.insert(val);
        }

        return true;
    }

    virtual bool filter(const value_type& value) = 0;

protected:
    std::unordered_set<value_type> set_value_;
};

template<class T>
class EqualFilter : public NormalFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(const value_type& value) {
        return value == this->value_;
    }
};

template<class T>
class NotEqualFilter : public NormalFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(const value_type& value) {
        return value != this->value_;
    }
};

template<class T>
class AndFilter : public NormalFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(const value_type& value) {
        return value & this->value_;
    }
};


template<class T>
class InFilter : public SetFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(const value_type& value) {
        return this->set_value_.find(value) != this->set_value_.end();
    }
};

template<class T>
class NotInFilter : public SetFilter<T> {
public:
    typedef T value_type;
    virtual bool filter(const value_type& value) {
        return this->set_value_.find(value) == this->set_value_.end();
    }
};

#endif // FILTER_H_
