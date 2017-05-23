#ifndef ARRAY_FIELD_H_
#define ARRAY_FIELD_H_

#include "query.h"
#include "field_type.h"

// ArrayField 只支持使用倒排索引进行查询， 不支持filter的方式进行查询
template<class T>
class ArrayField : public FieldInterface {
public:
    typedef T value_type;
    typedef typename FieldPreProcessor<T>::value_type index_value_type;

    ArrayField() : size_(0), index_(NULL) {}
    ~ArrayField() {
        if (NULL != index_) delete index_;
    }

    void set_size(int size) { size_ = size; values_.resize(size_); }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool get_value(V pos, std::vector<value_type>& value) {
        assert(pos <= size_);
        value = values_[pos];
        return true;
    }

    template<typename V, typename std::enable_if<std::is_same<V,int>::value, V>::type = 0>
    bool set_value(V pos, std::vector<value_type>& value) {
        assert(pos <= size_);
        values_[pos] = value;

        if (NULL != index_) {
            for (value_type& val: value) {
                index_->set_value(pos, processor_.GetValue(val));
            }
        }

        return true;
    }

    bool Init(bool use_index = false) {
        if (use_index) {
            index_ = new InvertedIndex<T>(); 
        }

        return true;
    }

    virtual bool Support(QueryType qt, const QueryData& data) {
        if (data.type != SupportQueryDataType<T>::value) return false;

        if (NULL != index_ && index_->Support(qt,data)) 
            return true;

        return false;
    }

    virtual bool trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {

        // 优先使用索引触发
        if (NULL != index_ && index_->Support(qt, data)) {
            return index_->trigger(qt, data, bitset);
        }

        return false;
    }

private:
    int size_;
    std::vector<std::vector<value_type> > values_;
    InvertedIndex<index_value_type>* index_;
    FieldPreProcessor<T> processor_;
};

#endif // ARRAY_FIELD_H_
