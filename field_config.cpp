#include "field_config.h"
#include "field_type.h"

feature_t pb_feature_to_feature_t(const index_system::pb::Feature& feature) {
    feature_t fea;
    fea.name = feature.name();
    fea.value = feature.value();
    return fea;
}

// BEGIN 与定义的各种get 函数
#define __DOC_ATTR(N)   doc.N
#define __DOC_ATTR_SIZE(N)  doc.N ##_size

// 定义一个获取一般属性的宏
#define DEFINE_NORMAL_GETTER(T, N)                      \
T get_ ## N(const index_system::pb::Document& doc) {    \
    return __DOC_ATTR(N)();                             \
}                                                       \
static bool __get_ ## N ## _func =                      \
    GetterFactory<T>::Instance().set(#N, &get_##N); 

#define DEFINE_REPEATED_GETTER(T,N)                                  \
std::vector<T> get_ ## N(const index_system::pb::Document& doc) {    \
    std::vector<T> ret;                                              \
    for (int i = 0; i < __DOC_ATTR_SIZE(N)(); ++i) {                 \
        ret.push_back(__DOC_ATTR(N)(i));                             \
    }                                                                \
    return ret;                                                      \
}                                                                    \
static bool __get_ ## N ## _func =                                   \
    GetterFactory<std::vector<T>>::Instance().set(#N, &get_##N); 

// 定义一个获取doc feature 属性的宏
#define DEFINE_NORMAL_FEATURE(N)                                    \
feature_t get_##N(const index_system::pb::Document& doc) {          \
    feature_t fea;                                                  \
    fea.name = __DOC_ATTR(N)().name();                              \
    fea.value = __DOC_ATTR(N)().value();                            \
    return fea;                                                     \
}                                                                   \
static bool __get_##N##_func =                                      \
    GetterFactory<feature_t>::Instance().set(#N, &get_##N);    

#define DEFINE_REPEATED_FEATURE(N)                                          \
std::vector<feature_t> get_##N(const index_system::pb::Document& doc) {     \
    std::vector<feature_t> ret;                                             \
    for (int i = 0; i < __DOC_ATTR_SIZE(N)(); ++i) {                        \
        ret.push_back(pb_feature_to_feature_t(__DOC_ATTR(N)(i)));           \
    }                                                                       \
    return ret;                                                             \
}                                                                           \
static bool __get_##N##_func =                                              \
    GetterFactory<std::vector<feature_t>>::Instance().set(#N, &get_##N);    

// 获取文章的各种属性
DEFINE_NORMAL_GETTER(   bool,               isdel)
DEFINE_NORMAL_GETTER(   std::string,        id)
DEFINE_NORMAL_GETTER(   double,             feedback_gmp)
DEFINE_NORMAL_GETTER(   int64_t,            publish_time)
DEFINE_NORMAL_GETTER(   std::string,        ncid)
DEFINE_NORMAL_GETTER(   int64_t,            update_time)
DEFINE_REPEATED_GETTER( std::string,        product_id)
DEFINE_NORMAL_GETTER(   int32_t,            update_type)
DEFINE_NORMAL_GETTER(   int32_t,            expire_age_hour)
DEFINE_NORMAL_GETTER(   int32_t,            build_count)
DEFINE_REPEATED_GETTER( double,             multi_gmp)
DEFINE_NORMAL_GETTER(   int64_t,            expire_time)
DEFINE_NORMAL_GETTER(   int64_t,            feed_version)
DEFINE_NORMAL_GETTER(   bool,               build_inc)
DEFINE_NORMAL_GETTER(   int64_t,            gmp_update)
DEFINE_REPEATED_GETTER( uint64_t,           lr_fea_hashes)
DEFINE_NORMAL_GETTER(   double,             quality)
DEFINE_NORMAL_GETTER(   double,             newsy_score)
DEFINE_NORMAL_GETTER(   uint64_t,           publisher_hash)
DEFINE_NORMAL_GETTER(   uint64_t,           groupid_hash)
DEFINE_NORMAL_GETTER(   uint32_t,           cp_expire_type)
DEFINE_NORMAL_GETTER(   uint64_t,           media_level)

DEFINE_REPEATED_FEATURE(                    keywords)
DEFINE_REPEATED_FEATURE(                    categories)
DEFINE_NORMAL_FEATURE(                      attributes)
DEFINE_REPEATED_FEATURE(                    topics)
DEFINE_REPEATED_FEATURE(                    ventities)
DEFINE_REPEATED_FEATURE(                    title_entities)
DEFINE_REPEATED_FEATURE(                    real_title_entities)
DEFINE_REPEATED_FEATURE(                    ve_re_te_intersection)
