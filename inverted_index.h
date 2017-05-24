#ifndef INVERTED_INDEX_H_
#define INVERTED_INDEX_H_

#include <unordered_map>
#include <vector>

#include "query.h"
#include "field_type.h"

template<class T>
class InvertedIndex {
public:
    typedef T value_type;
    typedef std::vector<int> inverted_list_t;

    void clear() { index_.clear(); }

    void set_value(int pos, value_type& value) {
        index_[value].push_back(pos);
    }

    // inverted_index 支持的查询类型有 EQUAL, NOT_EQUAL, IN, NOT_IN
    bool Support(QueryType qt, const QueryData& data) {
        if (data.type != SupportQueryDataType<T>::value || 
            data.values.empty() ||
            (qt != QT_EQUAL && qt != QT_NOT_EQUAL && qt != QT_IN && qt != QT_NOT_IN)) {
            return false;
        }

        return true;
    }

    bool Trigger(QueryType qt, const QueryData& data, boost::dynamic_bitset<>& bitset) {
        if (!Support(qt, data)) return false;

        std::vector<value_type> query_values;
        value_type value;
        if (qt == QT_EQUAL || qt == QT_NOT_EQUAL) {
            GetQueryDataValue<value_type>(data.type, data.values[0], value);
            query_values.push_back(value);
        } else if (qt == QT_IN || qt == QT_NOT_IN) {
            for (int i = 0; i < data.values.size(); ++i) {
                GetQueryDataValue<value_type>(data.type, data.values[i], value);
                query_values.push_back(value);
            }
        }

        for (const value_type& val : query_values) {
            auto it = index_.find(val);
            if (it != index_.end()) {
                for(int pos : it->second) {
                    bitset.set(pos);
                }
            }
        }

        if (qt == QT_NOT_EQUAL || qt == QT_NOT_IN) {
            bitset.flip();
        }

        return true;
    }

private:
    std::unordered_map<value_type,inverted_list_t> index_;
};

#endif // INVERTED_INDEX_H_
