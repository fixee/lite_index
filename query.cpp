#include "query.h"

template<>
bool GetQueryDataValue(const QueryDataValue& qd_value, std::vector<std::string>& value) {
    int size = qd_value.size;
    char *p = qd_value.data;

    query_str_size_type str_size;
    while (size > 0) {
        if (size < sizeof(query_str_size_type)) 
            return false;

        str_size = *(query_str_size_type*)p;
        p += sizeof(query_str_size_type);
        size -= sizeof(query_str_size_type);

        if (size < str_size) 
            return false;

        value.push_back(std::string(p, str_size));
        p += str_size;
        size -= str_size;
    }
    return true;
}

template<>
QueryDataValue MakeQueryDataValue(std::vector<std::string>& vec) {
    QueryDataValue value;
    value.size = 0;
    for (auto& str : vec) {
        value.size += sizeof(query_str_size_type) + str.size();
    }

    value.data = new char[value.size];
    char *p = value.data;
    for (auto& str : vec) {
        *(query_str_size_type*)p = str.size();
        p += sizeof(query_str_size_type);
        memcpy(p, str.c_str(), str.size());
        p += str.size();
    }

    return value;
}
