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

#endif // FILTER_H_
