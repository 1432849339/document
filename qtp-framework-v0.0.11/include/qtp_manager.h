//
// Created by wen on 2016-04-12.
//

#ifndef QTP_FRAMEWORK_QTP_EVENT_MANAGER_H
#define QTP_FRAMEWORK_QTP_EVENT_MANAGER_H

#include <unordered_set>
#include <event2/event.h>
#include <event2/thread.h>
#include "singleton.h"
#include "qtp_util.h"

namespace qtp {

class QtpEventBase;

class QtpManager {
public:
  static QtpManager& Main() {
    static QtpManager mgr;
    return mgr;
  }
  QtpManager();
  ~QtpManager();

  int Init(bool debug = false);
  int MakeNotifiable();
  int AddMember(QtpEventBase*);
  int Start();
  int Stop();

  event_base* base() { return event_base_; }
private:
  DISALLOW_COPY_AND_ASSIGN(QtpManager);

  event_base *event_base_;
  std::unordered_set<QtpEventBase*> members_;
};

} // namespace qtp

#endif //QTP_FRAMEWORK_QTP_EVENT_MANAGER_H
