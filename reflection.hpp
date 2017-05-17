#ifndef INDEX_SERVER_COMMON_REFLECTION_HPP_
#define INDEX_SERVER_COMMON_REFLECTION_HPP_

#include <string>
#include <map>

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

template<class K, class V>
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

    V* create_object(const K& name) {
        auto it = _object_gen_map.find(name);
        if(it == _object_gen_map.end()) {
            return NULL;
        }

        return (*(it->second))();
    }

    void release_object(const K& name, V* p_obj) {
        if(NULL != p_obj) delete p_obj;
    }

    static int register_object(const K& name, ObjectGenIf<V>* p_gen) {
        if(NULL == p_gen) return -1;
        Factory::instance()._object_gen_map[name] = p_gen;
        return 0;
    }

private:
    Factory() {}
    
    std::map<K, ObjectGenIf<V>* > _object_gen_map;
};

#endif // INDEX_SERVER_COMMON_REFLECTION_HPP_
