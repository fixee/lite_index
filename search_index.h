#ifndef SEARCH_INDEX_H_
#define SEARCH_INDEX_H_

#include <string>

#include "query.h"
#include "field_type.h"
#include "field_config.h"

class SearchIndex {
public:
    SearchIndex();
    ~SearchIndex();

    bool Init(std::string *err_msg = NULL);
    void Reset(size_t doc_size);

    void InsertDocument(const index_system::pb::Document& doc);

    bool IsQuerySupported(const Query& query);
    bool Trigger(const Query& query, std::vector<int>& result);

    template<class T>
    bool InitField(int slot, const field_config_t& conf);
    template<class T>
    bool InitArrField(int slot, const field_config_t& conf);

    bool TriggerItem(const QueryItem& item, boost::dynamic_bitset<>& bitset);
    bool IsQueryItemSupoorted(const QueryItem& item);
    
private:
    FieldInterface *fields_[INDEX_FIELD_SIZE];

    size_t capacity_;
    size_t doc_size_;
    bool inited_;
};

#endif // SEARCH_INDEX_H_
