#ifndef FIELD_H_
#define FIELD_H_

#include <type_traits>
#include <vector>
#include <boost/dynamic_bitset.hpp>

#include "query.h"
#include "field_type.h"
#include "inverted_index.h"
#include "filter.h"

template<QueryDataType QDT>
class CommField : public FieldInterface {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    CommField() : size_(0), index_(NULL) {}

    ~CommField() {
        for (FieldFilter<QDT>* p : filters_) {
            if (NULL != p) delete p;
        }

        if (NULL != index_) delete index_;
    }

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

        if (NULL != index_) {
            index_->set_value(pos, value);
        }
        return true;
    }


    bool Init(bool use_index = false) {
        filters_.resize(QT_MAX, NULL);
        Factory<QueryType, FieldFilter<QDT>>& inst = Factory<QueryType, FieldFilter<QDT>>::instance();
        for (int i = 0; i < QT_MAX; ++i) {
            filters_[i] = inst.create_object(static_cast<QueryType>(i));
        }

        if (use_index) {
            index_ = new InvertedIndex<QDT>(); 
        }
    }

    virtual bool Support(QueryType qt, const QueryData& data) {
        if (data.type != QDT) return false;

        if (NULL != index_ && index_->Support(qt,data)) 
            return true;

        if (NULL != filters_[qt] && filters_[qt]->Rebuild(data.values))
            return true;

        return false;
    }

    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {
        // 优先使用索引触发
        if (NULL != index_ && index_->Support(qt, data)) {
            return index_->trigger(qt, data, bitset);
        }

        // 使用filter 触发
        FieldFilter<QDT>* p_filter = filters_[qt];
        if (p_filter != NULL && p_filter->Rebuild(data.values)) {
            for (int i = 0; i < bitset.size(); ++i) {
                if (p_filter->filter(this->values_[i])) {
                    bitset.set(i);
                }
            }
            return true;
        }

        return false;
    }

private:
    int size_;
    std::vector<value_type> values_;
    InvertedIndex<QDT>* index_;
    std::vector<FieldFilter<QDT>*> filters_;
};

#endif // FIELD_H_
