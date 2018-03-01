//
// Created by wen on 2016-05-25.
//

#ifndef QTP_FRAMEWORK_QTP_TIME_H
#define QTP_FRAMEWORK_QTP_TIME_H
#include <stdint.h>
#include <time.h>
#include <string>
#include <chrono>
#include "qtp_util.h"

#ifdef _MSC_VER
#define localtime_r(t, tm) localtime_s(tm, t)
#endif

namespace qtp {

class QtpTime {
public:

  static void FromNow(int32_t* date, int32_t* time) {
    QtpTime now;
    now.FromNow();
    if (date) *date = now.date();
    if (time) *time = now.time();
  }

  void FromNow() {
    time_t now = ::time(NULL);
    FromTime(now);
  }

  void FromTime(time_t now) {
    struct tm tm_now = {0};
    localtime_r(&now, &tm_now);
    FromLocalTime(tm_now);
  }

  time_t ToTime() {
    struct tm tm ={0};
    ToLocalTime(&tm);
    return mktime(&tm);
  }

  void FromLocalTime(const struct tm& tm_now) {
    date_ = (tm_now.tm_year + 1900)*10000 + (tm_now.tm_mon + 1)*100 + tm_now.tm_mday;
    time_ = tm_now.tm_hour * 10000000 + tm_now.tm_min * 100000 + tm_now.tm_sec*1000;
  }

  void ToLocalTime(struct tm* tm) {
    tm->tm_year = year() - 1900;
    tm->tm_mon = month() - 1;
    tm->tm_mday = day();
    tm->tm_hour = hour();
    tm->tm_min = minute();
    tm->tm_sec = second();
  }

  std::string DateString() {
    char buf[32] = {0};
    snprintf(buf, 32, "%04d-%02d-%02d", year(), month(), day());
    return std::move(std::string(buf));
  }

  std::string TimeString(bool has_ms = false) {
    char buf[32] = {0};
    if (has_ms)
      snprintf(buf, 32, "%02d:%02d:%02d.%03d", hour(), minute(), second(), microsecond());
    else
      snprintf(buf, 32, "%02d:%02d:%02d", hour(), minute(), second());
    return std::move(std::string(buf));
  }

  std::string DateTimeString(char delimiter=' ', bool has_ms = false) {
    return DateString() + delimiter + TimeString(has_ms);
  }

  int32_t year() { return date_ / 10000; } const
  int32_t month() { return (date_ % 10000) / 100; } const
  int32_t day() { return date_ % 100; } const
  int32_t hour() { return time_ / 10000000; } const
  int32_t minute() { return (time_ % 10000000) / 100000; } const
  int32_t second() { return (time_ % 100000) / 1000; } const
  int32_t microsecond() { return time_ % 1000; } const

  int32_t date() { return date_; } const
  void set_date(int32_t date) { date_ = date; }
  int32_t time() { return time_; } const
  void set_time(int32_t time) { time_ = time; }

private:
  int32_t date_ = 0;
  int32_t time_ = 0;
};

} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_TIME_H