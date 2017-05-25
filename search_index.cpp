#include "search_index.h"

#include <boost/dynamic_bitset.hpp>

#include "field.h"
#include "array_field.h"
#include "field_config.h"

SearchIndex::SearchIndex() : capacity_(0), doc_size_(0), inited_(false) {
}

SearchIndex::~SearchIndex() {
}

template<class T>
bool SearchIndex::InitField(int slot, const field_config_t& conf) {
    if (slot >= INDEX_FIELD_SIZE) 
        return false;

    typename GetterFactory<T>::GetterFunc func;
    if(!GetterFactory<T>::Instance().get(conf.convert_func, func))
        return false;

    FieldInterface* p = new CommField<T>(func);
    if (!p->Init(conf.index)) {
        delete p;
        return false;
    } else {
        fields_[slot] = p;
        return true;
    }
}

template<class T>
bool SearchIndex::InitArrField(int slot, const field_config_t& conf) {
    if (slot >= INDEX_FIELD_SIZE) 
        return false;

    typename GetterFactory<std::vector<T>>::GetterFunc func;
    if(!GetterFactory<std::vector<T>>::Instance().get(conf.convert_func, func))
        return false;

    FieldInterface* p = new ArrayField<T>(func);
    if (!p->Init(conf.index)) {
        delete p;
        return false;
    } else {
        fields_[slot] = p;
        return true;
    }
}

bool SearchIndex::Init() {
    for (int i = 0; i < INDEX_FIELD_SIZE; ++i) {
        const field_config_t& conf = INDEX_FIELDS[i];
        switch (conf.type) {
        case INT32:
            if (!InitField<int32_t>(i, conf)) return false;
            break;
        case INT64:
            if (!InitField<int64_t>(i, conf)) return false;
            break;
        case FLOAT :
            if (!InitField<float>(i, conf)) return false;
            break;
        case DOUBLE :
            if (!InitField<double>(i, conf)) return false;
            break;
        case BOOL :
            if (!InitField<bool>(i, conf)) return false;
            break;
        case STRING :
            if (!InitField<std::string>(i, conf)) return false;
            break;
        case FEATURE :
            if (!InitField<feature_t>(i, conf)) return false;
            break;
        case ARR_INT32:
            if (!InitArrField<int32_t>(i, conf)) return false;
            break;
        case ARR_INT64:
            if (!InitArrField<int64_t>(i, conf)) return false;
            break;
        case ARR_FLOAT :
            if (!InitArrField<float>(i, conf)) return false;
            break;
        case ARR_DOUBLE :
            if (!InitArrField<double>(i, conf)) return false;
            break;
        case ARR_BOOL :
            if (!InitArrField<bool>(i, conf)) return false;
            break;
        case ARR_STRING :
            if (!InitArrField<std::string>(i, conf)) return false;
            break;
        case ARR_FEATURE :
            if (!InitArrField<feature_t>(i, conf)) return false;
            break;
        default:
            return false;
        }
    }

    return true;
}

void SearchIndex::Reset(size_t doc_size) {
    for (FieldInterface* p : fields_) {
        p->clear();
        p->set_size(doc_size);
    }

    doc_size_ = 0;
    capacity_ = doc_size;
}

void SearchIndex::InsertDocument(const index_system::pb::Document& doc) {
    for (FieldInterface* p : fields_) {
        p->SetDocument(doc_size_, doc);
    }
    doc_size_++;
    return;
}

bool SearchIndex::IsQuerySupported(const Query& query) {
    for (const auto& item : query.should) {
        if (!IsQueryItemSupoorted(item))
            return false;
    }

    for (const auto& item : query.must) {
        if (!IsQueryItemSupoorted(item))
            return false;
    }

    for (const auto& item : query.must_not) {
        if (!IsQueryItemSupoorted(item))
            return false;
    }

    return true;
}

bool SearchIndex::IsQueryItemSupoorted(const QueryItem& item) {
    auto it = FIELD_SLOT_MAP.find(item.name);
    if (it != FIELD_SLOT_MAP.end()) {
        return fields_[it->second]->Support(item.type, item.data);
    } else {
        return false;
    }
}

bool SearchIndex::Trigger(const Query& query, std::vector<int>& result) {
    //if (!IsQueryItemSupoorted(query)) return false;

    boost::dynamic_bitset<> working_set(doc_size_);
    boost::dynamic_bitset<> should_set(doc_size_);
    boost::dynamic_bitset<> must_set(doc_size_);
    boost::dynamic_bitset<> must_not_set(doc_size_);

    for (const auto& item: query.should) {
        working_set.clear();
        if (!TriggerItem(item, working_set)) 
            return false;
        should_set |= working_set;
    }

    must_set.set();
    for (const auto& item: query.must) {
        working_set.clear();
        if (!TriggerItem(item, working_set)) 
            return false;
        must_set &= working_set;
    }

    for (const auto& item: query.must_not) {
        working_set.clear();
        if (!TriggerItem(item, working_set)) 
            return false;
        must_not_set |= working_set;
    }

    should_set &= must_set;
    should_set &= must_not_set.flip();

    // bitset to pos
    for (size_t index = should_set.find_first(); 
         index != boost::dynamic_bitset<>::npos; 
         index = should_set.find_next(index)) {
        result.push_back(index);
    }

    return false;
}

bool SearchIndex::TriggerItem(const QueryItem& item, boost::dynamic_bitset<>& bitset) {
    auto it = FIELD_SLOT_MAP.find(item.name);
    if (it == FIELD_SLOT_MAP.end()) return false;

    return fields_[it->second]->Trigger(item.type, item.data, bitset);
}
