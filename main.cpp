#include <iostream>
#include <string>
#include <vector>

#include "def.pb.h"
#include "reflection.hpp"

typedef struct c_str_t {
    char *data;
    size_t size;
} c_str_t;

typedef int32_t     D_INT32     ;
typedef uint32_t    D_UINT32    ;
typedef int64_t     D_INT64     ;
typedef uint64_t    D_UINT64    ;
typedef float       D_FLOAT     ;
typedef double      D_DOUBLE    ;
typedef bool        D_BOOL      ;
typedef c_str_t     D_STRING    ;

typedef std::vector<int> InvertedIndex;

template<class T>
class Field {

private:
    size_t size_;
    size_t capacity_;
};

class Searcher {
    
private:
    Field<D_STRING> name_;
    Field<D_INT32> age_;
};

class Document {
public:
    Document(DataBase* db, int doc_id);

private:
    DataBase *db;
    int doc_id;
};

struct field_config {
    std::string name;
    std::string type;
    std::string pb_field;
};

std::vector<field_config> vec = {
    { "name",       "string",       "name"      },
    { "age",        "int32",        "age"       },
    { "gender",     "bool",         "gender"    },
    { "score",      "float",        "score"     },
    { "keywords",   "feature",      "keywords"  }
};

c_str_t field_getter_name(const Document& doc) {
    c_str_t name;
    name.size = doc.name().size();
    name.data = new char[name.size];
    memcpy(name.data, doc.name().c_str(), name.size);

    return name;
}

D_INT32 field_getter_age(const Document& doc) {
    return doc.age;
}

int main(int argc, char** argv) {
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i].type << std::endl;
    }

    return 0;
}
