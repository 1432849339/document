//
// Created by wen on 2016-05-06.
//

#ifndef QTP_FRAMEWORK_QTP_LOG_H
#define QTP_FRAMEWORK_QTP_LOG_H

#include <iostream>
#include "spdlog/spdlog.h"
#include "qtp_util.h"
#include "singleton.h"

namespace qtp {

class QtpLog : public Singleton<QtpLog> {
public:
  typedef spdlog::level::level_enum level_enum;
  static std::shared_ptr<spdlog::logger> s_qtp_log;
  static std::shared_ptr<spdlog::logger> s_app_log;
  static int s_verbose;

  static level_enum kTrace;
  static level_enum kDebug;
  static level_enum kInfo;
  static level_enum kNotice;
  static level_enum kWarn;
  static level_enum kError;
  static level_enum kCritical;
  static level_enum kAlert;
  static level_enum kEmerg;

  static void SetVerbose(int v);
  static void SetLevel(level_enum lev);
  static void SetPatten(const std::string& pattern);
  static void SetAsyncMode(size_t queue_size, bool block = true);
  static int Create(const std::string &app_name, const std::string &log_dir = "logs");

  ~QtpLog();
private:
  friend class Singleton<QtpLog>;
  QtpLog();
};

class LogMessageVoidify {
  public:
  LogMessageVoidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(spdlog::details::line_logger&) { }
};

} //namespace qtp

#define QTPLOG_TRY  try
#define QTPLOG_CATCH catch (const spdlog::spdlog_ex& ex) \
  { std::cout << "Log failed: " << ex.what() << std::endl; }

#ifdef QTP_FRAMEWORK_EXPORT
#define VLOG(v)  (v) > qtp::QtpLog::s_verbose ? void(0) : qtp::LogMessageVoidify() & qtp::QtpLog::s_qtp_log->info()
#define LOG_IF(exp)  (!(exp)) ? void(0) : qtp::LogMessageVoidify() & qtp::QtpLog::s_qtp_log->info()
#define LOG(level) qtp::QtpLog::s_qtp_log->level()
#else //#ifdef QTP_FRAMEWORK_EXPORT
#define VLOG(v)  (v) > qtp::QtpLog::s_verbose ? void(0) : qtp::LogMessageVoidify() & qtp::QtpLog::s_app_log->info()
#define LOG_IF(exp)  (!(exp)) ? void(0) : qtp::LogMessageVoidify() & qtp::QtpLog::s_qtp_log->info()
#define LOG(level) qtp::QtpLog::s_app_log->level()
#endif //#ifdef QTP_FRAMEWORK_EXPORT

#endif //QTP_FRAMEWORK_QTP_LOG_H
