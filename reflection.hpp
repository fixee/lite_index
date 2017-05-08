#ifndef INDEX_SERVER_COMMON_REFLECTION_HPP_
#define INDEX_SERVER_COMMON_REFLECTION_HPP_

#include <string>
#include <map>

namespace index_system {

template<class T>
class ObjectGenIf {
public:
    virtual T* operator()() = 0;
};

template<class T, class T1>
class ObjectGen : public ObjectGenIf<T> {
    virtual T* operator()() {
        return new T1();
    }
};

template<class T>
class Factory {
public:
    ~Factory() {
        for(auto it = _object_gen_map.begin(); it != _object_gen_map.end(); ++it) {
            delete it->second;
        }
    }

    static Factory& instance() {
        static Factory _inst;
        return _inst;
    }

    T* create_object(const std::string& name) {
        auto it = _object_gen_map.find(name);
        if(it == _object_gen_map.end()) {
            return NULL;
        }

        return (*(it->second))();
    }

    void release_object(const std::string& name, T* p_obj) {
        if(NULL != p_obj) delete p_obj;
    }

    static int register_object(const std::string& name, ObjectGenIf<T>* p_gen) {
        if(NULL == p_gen) return -1;
        Factory::instance()._object_gen_map[name] = p_gen;
        return 0;
    }

private:
    Factory() {}
    
    std::map<std::string, ObjectGenIf<T>* > _object_gen_map;
};

#define REGISTER_REFLECTION_FACTORY(Name,If,Cls) \
int __reflection_ ## Name ## _ ## Cls = Factory<If>::register_object(#Name, new ObjectGen<If,Cls>())

} // namespace index_system

#endif // INDEX_SERVER_COMMON_REFLECTION_HPP_
