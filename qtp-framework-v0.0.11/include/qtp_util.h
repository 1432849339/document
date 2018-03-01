//
// Created by wen on 2016-04-12.
//

#ifndef QTP_FRAMEWORK_QTP_UTIL_H
#define QTP_FRAMEWORK_QTP_UTIL_H

#include <memory>
#include <vector>
#include <string>

#ifdef _MSC_VER
# ifndef snprintf
#   define snprintf _snprintf
# endif
#endif

namespace qtp {

#define DEFINE_SHARED_PTR(TypeName) \
  typedef std::shared_ptr<TypeName> TypeName##Ptr

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);                \
  void operator=(const TypeName&)

template<typename T>
void UpdateOrInsert(T& m, const typename T::key_type& k, const typename T::mapped_type& v) {
  auto it = m.find(k);
  if (it != m.end()) m[k] = v;
  else m.insert(std::make_pair(k,v));
}

template<typename T>
typename T::mapped_type FindOrDefault(T& m, const typename T::key_type& k, typename T::mapped_type default_v) {
  auto it = m.find(k);
  if (it != m.end()) return it->second;
  else return default_v;
}

template<typename T>
typename T::mapped_type FindOrMakeShared(T& m, const typename T::key_type& k) {
  typename T::mapped_type v;
  auto it = m.find(k);
  if (it != m.end()) v = it->second;
  else {
    v = std::make_shared<typename T::mapped_type::element_type>();
    m.insert(std::make_pair(k, v));
  }
  return v;
}

int mkdir_p(const char* path);
int split(const std::string& s, const std::string& delim, std::vector< std::string >* ret);
int get_clean_path(const std::string& path, std::string* clean_path);
int get_full_path(const std::string& dir, const std::string& path, std::string* full_path);

} // namespace qtp

#endif //QTP_FRAMEWORK_QTP_UTIL_H
