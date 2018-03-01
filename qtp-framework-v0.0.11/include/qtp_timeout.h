//
// Created by wen on 2016-05-20.
//

#ifndef QTP_FRAMEWORK_QTP_TIMEOUT_H
#define QTP_FRAMEWORK_QTP_TIMEOUT_H

#include <unordered_map>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

namespace qtp {

class QtpTimeout;
class QtpTimeoutBase {
public:
  virtual void OnTimeout(QtpTimeout *timeout) = 0;
};

class QtpTimeout {
public:
  static void timeout_cb(evutil_socket_t fd, short what, void *user_data);

  QtpTimeout(event_base *ev_base, QtpTimeoutBase *to_base, int id, const timeval &tv);

  ~QtpTimeout();

  QtpTimeoutBase *base() { return base_; }

  event *ev() { return ev_; }

  int id() { return id_; }

  timeval tv() { return tv_; }

private:
  QtpTimeoutBase *base_ = nullptr;
  event *ev_ = nullptr;
  int id_ = 0;
  timeval tv_;
};

} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_TIMEOUT_H
