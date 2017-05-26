#ifndef FIELD_H_
#define FIELD_H_

#include <type_traits>
#include <vector>
#include <boost/dynamic_bitset.hpp>

#include "query.h"
#include "field_type.h"
#include "inverted_index.h"
#include "filter.h"
#include "field_config.h"
#include "document.pb.h"

template<class T>
class CommField : public FieldInterface {
public:
    // 用于存储的数据类型
    typedef T value_type;
    // 用于建立索引的数据类型
    typedef typename FieldPreProcessor<T>::value_type index_value_type;
    // 从文章中获取值的方法
    typedef typename GetterFactory<T>::GetterFunc GetterFunc;

    CommField(GetterFunc func) : index_(NULL), getter_func_(func) {}
    ~CommField() {
        for (FieldFilter<index_value_type>* p : filters_) {
            if (NULL != p) delete p;
        }
        if (NULL != index_) delete index_;
    }

    virtual void set_size(size_t size) { values_.resize(size); }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool get_value(V pos, value_type& value) {
        value = values_[pos];
        return true;
    }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool set_value(V pos, const value_type& value) {
        values_[pos] = value;

        if (NULL != index_) {
            index_->set_value(pos, processor_(value));
        }
        return true;
    }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool set_value(V pos, const value_type&& value) {
        values_[pos] = value;

        if (NULL != index_) {
            index_->set_value(pos, processor_(value));
        }
        return true;
    }

    virtual bool SetDocument(int pos, const index_system::pb::Document& doc) {
        return set_value(pos, getter_func_(doc));
    }

    virtual void clear() {
        values_.clear();

        if (NULL != index_) {
            index_->clear();
        }
    }

    bool Init(bool use_index = false) {
        filters_.resize(QT_MAX, NULL);
        Factory<QueryType, FieldFilter<index_value_type>>& inst =   \
            Factory<QueryType, FieldFilter<index_value_type>>::instance();
        for (int i = 0; i < QT_MAX; ++i) {
            filters_[i] = inst.create_object(static_cast<QueryType>(i));
        }

        if (use_index) {
            index_ = new InvertedIndex<index_value_type>(); 
        }

        return true;
    }

    virtual int Support(QueryType qt, const QueryData& data) {
        if (data.type != SupportQueryDataType<T>::value) return 0;

        if (NULL != index_ && index_->Support(qt,data)) 
            return 2;

        if (NULL != filters_[qt] && filters_[qt]->Rebuild(data.type, data.value))
            return 1;

        return 0;
    }

    virtual bool Trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {
        if (NULL != index_ && index_->Support(qt, data))
            return index_->Trigger(qt, data, bitset);

        FieldFilter<index_value_type>* p_filter = filters_[qt];
        if (p_filter == NULL || !p_filter->Rebuild(data.type, data.value))
            return false;

        for (int i = 0; i < values_.size(); ++i) {
            if (p_filter->filter(processor_(values_[i]))) {
                bitset.set(i);
            }
        }
        return true;
    }

private:
    std::vector<value_type> values_;
    InvertedIndex<index_value_type>* index_;
    std::vector<FieldFilter<index_value_type>*> filters_;

    FieldPreProcessor<T> processor_;
    GetterFunc getter_func_;
};

#endif // FIELD_H_
