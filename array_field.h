#ifndef ARRAY_FIELD_H_
#define ARRAY_FIELD_H_

#include "query.h"
#include "field_type.h"

// ArrayField 只支持使用倒排索引进行查询， 不支持filter的方式进行查询
template<class T>
class ArrayField : public FieldInterface {
public:
    // 用于存储的数据类型
    typedef T value_type;
    // 用于建立索引的数据类型
    typedef typename FieldPreProcessor<T>::value_type index_value_type;
    // 从文章中获取值的方法
    typedef typename GetterFactory<std::vector<T>>::GetterFunc GetterFunc;

    ArrayField(GetterFunc func) : index_(NULL), getter_func_(func) {}
    ~ArrayField() {
        if (NULL != index_) delete index_;
    }

    void set_size(size_t size) { values_.resize(size); }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool get_value(V pos, std::vector<value_type>& value) {
        value = values_[pos];
        return true;
    }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool set_value(V pos, std::vector<value_type>& value) {
        values_[pos] = value;

        if (NULL != index_) {
            for (const value_type& val: value) {
                index_->set_value(pos, processor_(val));
            }
        }

        return true;
    }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool set_value(V pos, std::vector<value_type>&& value) {
        values_[pos] = value;

        if (NULL != index_) {
            for (const value_type& val: value) {
                index_->set_value(pos, processor_(val));
            }
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
        if (use_index) {
            index_ = new InvertedIndex<index_value_type>(); 
        }

        return true;
    }

    virtual int Support(QueryType qt, const QueryData& data) {
        if (data.type != SupportQueryDataType<T>::value) return 0;

        if (NULL != index_ && index_->Support(qt,data)) 
            return 2;

        return 0;
    }

    virtual bool Trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {
        // 优先使用索引触发
        if (NULL != index_ && index_->Support(qt, data)) {
            return index_->Trigger(qt, data, bitset);
        }

        return false;
    }

private:
    std::vector<std::vector<value_type> > values_;
    InvertedIndex<index_value_type>* index_;
    FieldPreProcessor<T> processor_;
    GetterFunc getter_func_;
};

#endif // ARRAY_FIELD_H_
