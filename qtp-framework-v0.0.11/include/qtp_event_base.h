//
// Created by wen on 2016-04-12.
//

#ifndef QTP_FRAMEWORK_QTP_EVENT_BASE_H
#define QTP_FRAMEWORK_QTP_EVENT_BASE_H

#include <unordered_map>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include "qtp_timeout.h"
#include "qtp_message.h"

namespace qtp {

class QtpManager;

class QtpEventBase : public QtpTimeoutBase {
public:
  static void read_cb(bufferevent *bev, void *user_data);

  static void write_cb(bufferevent *bev, void *user_data);

  static void event_cb(bufferevent *bev, short events, void *user_data);

  static int SetTcpNoDelay(bufferevent *bev);
  static int SetKeepAlive(bufferevent *bev, int keepAlive = 1, int keepIdle = 60, int keepInterval = 5, int keepCount = 3);
  static int SendMessage(bufferevent *bev, QtpMessagePtr message);
  static int SendData(bufferevent *bev, const void* data, size_t data_len);

  QtpEventBase();

  virtual ~QtpEventBase();

  virtual int Init();

  virtual void OnRead(bufferevent *bev);

  virtual void OnWrite(bufferevent *bev);

  virtual void OnEvent(bufferevent *bev, short events);

  virtual void OnMessage(QtpMessagePtr message, bufferevent* bev);

  virtual void OnTimeout(QtpTimeout *timeout) override;

  int AddTimeout(int id, const timeval &tv);

  int DelTimeout(int id);

  QtpManager *manager() { return manager_; }

  void set_manager(QtpManager *manager) { manager_ = manager; }

private:
  QtpManager *manager_;
  std::unordered_map<int, QtpTimeout *> timeouts_;
};


} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_EVENT_BASE_H
