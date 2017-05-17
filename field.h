#ifndef FIELD_H_
#define FIELD_H_

#include <type_traits>
#include <vector>
#include <boost/dynamic_bitset.hpp>

#include "query.h"
#include "field_type.h"
#include "filter.h"

class FieldInterface {
public:
    virtual bool Init() = 0;
    virtual bool Support(QueryType qt, const QueryData& data) = 0;
    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) = 0;
};

template<QueryDataType QDT>
class FieldValue {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    void set_size(int size) { size_ = size; values_.resize(size_); }

    template<typename T, typename std::enable_if<std::is_same<T,int>::value, T>::type = 0>
    bool get_value(T pos, value_type& value) {
        assert(pos <= size_);
        value = values_[pos];
        return true;
    }

    template<typename T, typename std::enable_if<std::is_same<T,int>::value, T>::type = 0>
    bool set_value(T pos, value_type& value) {
        // 使用 if 进行判断 或者 assert 进行判断？？？
        // assert 起作用的地方是在运行时 ??? 
        assert(pos <= size_);
        values_[pos] = value;
        return true;
    }

protected:
    int size_;
    std::vector<value_type> values_;
};

template<QueryDataType QDT>
class CommField : public FieldValue<QDT>, public FieldInterface {
public:
    CommField() {}
    ~CommField() {
        for (FieldFilter<QDT>* p : filters_) {
            if (NULL != p) delete p;
        }
    }

    virtual bool Init() {
        filters_.resize(QT_MAX, NULL);
        Factory<QueryType, FieldFilter<QDT>>& inst = Factory<QueryType, FieldFilter<QDT>>::instance();
        for (int i = 0; i < QT_MAX; ++i) {
            filters_[i] = inst.create_object(static_cast<QueryType>(i));
        }
    }

    virtual bool Support(QueryType qt, const QueryData& data) {
        if (data.type != QDT || NULL == filters_[qt] || 
            !filters_[qt]->Rebuild(data.values)) {
            return false;
        }

        return true;
    }

    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {
        FieldFilter<QDT>* p_filter = filters_[qt];
        if (p_filter == NULL || !p_filter->Rebuild(data.values)) 
            return false;

        for (int i = 0; i < bitset.size(); ++i) {
            if (p_filter->filter(this->values_[i])) {
                bitset.set(i);
            }
        }

        return true;
    }

private:
    std::vector<FieldFilter<QDT>*> filters_;
};

#endif // FIELD_H_
