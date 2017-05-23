#ifndef FIELD_H_
#define FIELD_H_

#include <type_traits>
#include <vector>
#include <boost/dynamic_bitset.hpp>

#include "query.h"
#include "field_type.h"
#include "inverted_index.h"
#include "filter.h"

template<class T>
class CommField : public FieldInterface {
public:
    // 用于存储的数据类型
    typedef T value_type;
    // 用于建立索引的数据类型
    typedef typename FieldPreProcessor<T>::value_type index_value_type;

    CommField() : size_(0), index_(NULL) {}
    ~CommField() {
        for (FieldFilter<index_value_type>* p : filters_) {
            if (NULL != p) delete p;
        }
        if (NULL != index_) delete index_;
    }

    void set_size(int size) { size_ = size; values_.resize(size_); }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool get_value(V pos, value_type& value) {
        assert(pos <= size_);
        value = values_[pos];
        return true;
    }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool set_value(V pos, value_type& value) {
        assert(pos <= size_);
        values_[pos] = value;

        if (NULL != index_) {
            index_->set_value(pos, processor_.GetValue(value));
        }
        return true;
    }

    bool Init(bool use_index = false) {
        filters_.resize(QT_MAX, NULL);
        Factory<QueryType, FieldFilter<index_value_type>>& inst =   \
            Factory<QueryType, FieldFilter<index_value_type>>::instance();
        for (int i = 0; i < QT_MAX; ++i) {
            filters_[i] = inst.create_object(static_cast<QueryType>(i));
        }

        if (use_index) {
            index_ = new InvertedIndex<T>(); 
        }
    }

    virtual int Support(QueryType qt, const QueryData& data) {
        if (data.type != SupportQueryDataType<T>::value) return 0;

        if (NULL != index_ && index_->Support(qt,data)) 
            return 2;

        if (NULL != filters_[qt] && filters_[qt]->Rebuild(data.type, data.values))
            return 1;

        return 0;
    }

    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {
        // 优先使用索引触发
        if (NULL != index_ && index_->Support(qt, data)) {
            return index_->trigger(qt, data, bitset);
        }

        // 使用filter 触发
        FieldFilter<index_value_type>* p_filter = filters_[qt];
        if (p_filter != NULL && p_filter->Rebuild(data.type, data.values)) {
            for (int i = 0; i < bitset.size(); ++i) {
                if (p_filter->filter(processor_.GetValue(this->values_[i]))) {
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
    InvertedIndex<index_value_type>* index_;
    std::vector<FieldFilter<index_value_type>*> filters_;

    FieldPreProcessor<T> processor_;
};

#endif // FIELD_H_
