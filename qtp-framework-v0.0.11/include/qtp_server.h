//
// Created by wen on 2016-04-12.
//

#ifndef QTP_FRAMEWORK_QTP_SERVER_H
#define QTP_FRAMEWORK_QTP_SERVER_H

#include "qtp_event_base.h"

namespace qtp {

class QtpServer : public QtpEventBase {
public:
  QtpServer();

  ~QtpServer();

  int Bind(const struct sockaddr *sa, int socklen, int backlog = -1);
  int Close(bufferevent *bev);

  virtual void OnAccept(bufferevent *bev, evutil_socket_t fd, struct sockaddr *sa, int socklen);
  virtual void OnClose(bufferevent *bev);
  //virtual void OnRead(bufferevent* bev) override;
  //virtual void OnWrite(bufferevent* bev) override;
  void OnEvent(bufferevent* bev, short events) override;

private:
  evconnlistener *listener_;
};

} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_SERVER_H
