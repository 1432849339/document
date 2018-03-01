//
// Created by wen on 2016-04-26.
//

#ifndef QTP_FRAMEWORK_QTP_QUEUE_H
#define QTP_FRAMEWORK_QTP_QUEUE_H

#include <unordered_map>
#include "lock_queue.h"
#include "qtp_message.h"
#include "singleton.h"

namespace qtp {

typedef LockQueue<QtpMessagePtr> QtpQueue;

class QtpQueueMgr : public Singleton<QtpQueueMgr> {
public:
  ~QtpQueueMgr();
  QtpQueue* CeateQueue(uint32_t id);
  int RemoveQueue(uint32_t id);
  int SendMessage(uint32_t id, QtpMessagePtr msg);
private:
  QtpQueueMgr() = default;
  friend class Singleton<QtpQueueMgr>;
  std::unordered_map<uint32_t, QtpQueue*> queues_;
};

}

#endif //QTP_FRAMEWORK_QTP_QUEUE_H
