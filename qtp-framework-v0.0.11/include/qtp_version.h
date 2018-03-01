//
// Created by wen on 2016-05-11.
//

#ifndef QTP_FRAMEWORK_QTP_VERSION_H
#define QTP_FRAMEWORK_QTP_VERSION_H

//major < 100, minor < 100, release < 10000
#define DECLARE_VERSION(name, major, minor, release) \
static const char* name##_version_str() { return #major"."#minor"."#release; }\
static const int s_##name##_major_version = major; \
static const int s_##name##_minor_version = minor; \
static const int s_##name##_release_version = release; \
int name##_major_version(); \
int name##_minor_version(); \
int name##_release_version(); \
int name##_version(); \
const char* name##_lib_version_str();

#define DEFINE_VERSION(name) \
int name##_major_version() { return s_##name##_major_version; } \
int name##_minor_version() { return s_##name##_minor_version; } \
int name##_release_version() { return s_##name##_release_version; } \
int name##_version() { \
  return s_##name##_release_version + s_##name##_minor_version * 10000 + s_##name##_major_version * 1000000; \
} \
const char* name##_lib_version_str() { \
return name##_version_str(); \
}

#define GET_MAJOR_VERSION(name) name##_major_version()

#define GET_MINOR_VERSION(name) name##_minor_version()

#define GET_RELEASE_VERSION(name) name##_release_version()

#define GET_VERSION_NUM(name) name##_version()

#define GET_VERSION_STR(name) name##_version_str()

//chech the lib and headers
#define CHECK_LIB_VERSION(name) \
(name##_major_version() == s_##name##_major_version && \
name##_minor_version() == s_##name##_minor_version && \
name##_release_version() == s_##name##_release_version && \
!strcmp(name##_version_str(), name##_lib_version_str()) )

#endif //QTP_FRAMEWORK_QTP_VERSION_H
