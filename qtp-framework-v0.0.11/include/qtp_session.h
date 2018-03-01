//
// Created by wen on 2016-04-27.
//

#ifndef QTP_FRAMEWORK_QTP_SESSION_H
#define QTP_FRAMEWORK_QTP_SESSION_H

#include <unordered_map>
#include "singleton.h"
#include "qtp_message.h"

struct bufferevent;

namespace qtp {

class QtpMessage;
typedef uint64_t session_id_t;
typedef void* Session;

class QtpSessionMgr : public Singleton<QtpSessionMgr> {
public:
  static const int kMaxUserSession = 1000;
  session_id_t AddSession(bufferevent*);
  int AddSession(session_id_t session_id, bufferevent*);
  session_id_t GetSessionID(bufferevent*);
  bufferevent* GetSession(session_id_t session_id);
  int RemoveSession(bufferevent* bev);
  int SendMessage(session_id_t session_id, QtpMessagePtr message);
  int SendData(session_id_t session_id, const void* data, size_t data_len);
  int BroadcastMessage(QtpMessagePtr message);
  int BroadcastData(const void* data, size_t data_len);
private:
  session_id_t session_id_ = kMaxUserSession;
  std::unordered_map<session_id_t, bufferevent*> sessions_;
  std::unordered_map<bufferevent*, session_id_t> session_ids_;
};

} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_SESSION_H
