#ifndef ARRAY_FIELD_H_
#define ARRAY_FIELD_H_

#include "query.h"
#include "field_type.h"

// ArrayField 只支持使用倒排索引进行查询， 不支持filter的方式进行查询
template<QueryDataType QDT>
class ArrayField : public FieldInterface {
public:
    typedef typename FieldDataType<QDT>::value_type value_type;

    ArrayField() : size_(0), index_(NULL) {}
    ~ArrayField() {
        if (NULL != index_) delete index_;
    }

    void set_size(int size) { size_ = size; values_.resize(size_); }

    template<typename T, typename std::enable_if<std::is_same<T,int>::value, T>::type = 0>
    bool get_value(T pos, std::vector<value_type>& value) {
        assert(pos <= size_);
        value = values_[pos];
        return true;
    }

    template<typename T, typename std::enable_if<std::is_same<T,int>::value, T>::type = 0>
    bool set_value(T pos, std::vector<value_type>& value) {
        // 使用 if 进行判断 或者 assert 进行判断？？？
        // assert 起作用的地方是在运行时 ??? 
        assert(pos <= size_);
        values_[pos] = value;

        if (NULL != index_) {
            for (value_type& val: value) {
                index_->set_value(pos, val);
            }
        }

        return true;
    }

    bool Init(bool use_index = false) {
        if (use_index) {
            index_ = new InvertedIndex<QDT>(); 
        }

        return true;
    }

    virtual bool Support(QueryType qt, const QueryData& data) {
        if (data.type != QDT) return false;

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
    InvertedIndex<QDT>* index_;
};

#endif // ARRAY_FIELD_H_
