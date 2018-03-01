//
// Created by wen on 2016-04-26.
//

#ifndef QTP_FRAMEWORK_QTP_STAGE_H
#define QTP_FRAMEWORK_QTP_STAGE_H

#include "qtp_timeout.h"
#include "qtp_queue.h"
#include "spin_lock.h"

namespace qtp {

class QtpStage : public QtpTimeoutBase {
public:
  QtpStage(QtpQueue* q = nullptr);
  virtual ~QtpStage();

  virtual int OnStart() { return 0; }
  virtual int OnEvent(QtpMessagePtr message) = 0;
  virtual int OnStop() { return 0; }

  void Run(); //it's for thread

  virtual void OnTimeout(QtpTimeout *timeout) override;
  int AddTimeout(int id, const timeval &tv);
  int DelTimeout(int id);

  void set_queue(QtpQueue* queue) { queue_ = queue; }
  QtpQueue* queue() { return queue_; }

private:
  QtpQueue* queue_;

  SpinLock timeouts_lock_;
  std::unordered_map<int, QtpTimeout *> timeouts_;
};

} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_STAGE_H
