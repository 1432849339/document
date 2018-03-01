//
// Created by wen on 2016-04-12.
//

#ifndef QTP_FRAMEWORK_QTP_CLIENT_H
#define QTP_FRAMEWORK_QTP_CLIENT_H

#include "qtp_event_base.h"

namespace qtp {

class QtpClient : public QtpEventBase {
public:
  enum TimeoutType {
    kTtReConnect = 1,
  };

  QtpClient(bool auto_connect = false, uint32_t reconnect_ms = 1000);
  ~QtpClient();

  int Connect(const struct sockaddr* sa, int socklen);
  int ReConnect();
  int Close();

  virtual void OnConnected();
  virtual void OnDisconnected();

  int SendMessage(QtpMessagePtr message);
  int SendData(const void* data, size_t data_len);

  bool IsConnected() {
    return connected_;
  }

  bool IsAutoConnect() {
    return auto_connect_;
  }

  void OnEvent(bufferevent* bev, short events) override;
  void OnTimeout(QtpTimeout *timeout) override;

protected:
  bufferevent* client_;
  struct sockaddr sa_;
  int socklen_;
  char address[128];
  bool connected_;
  bool auto_connect_;
  uint32_t reconnect_ms_;
};

} // namespace qtp


#endif //QTP_FRAMEWORK_QTP_CLIENT_H
