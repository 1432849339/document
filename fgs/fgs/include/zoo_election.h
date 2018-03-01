/*
 * Copyright (C) 2017 LeHigh Hongking - All Rights Reserved.
 *
 * You may not use, distribute and modify this code for any
 * purpose unless you receive an official authorization from
 * Shenzhen LeHigh Hongking Technologies Co., Ltd.
 *
 * You should have received a copy of the license with this
 * file. If not, please write to: admin@hongkingsystem.cn,
 * or visit: http://hongkingsystem.cn
 */
#ifndef FGS_ZOO_ELECTION_H
#define FGS_ZOO_ELECTION_H
#include "zoo_data.h"
#include "zoo_watcher.h"
#ifdef QTPBASE
#include "qtp_queue.h"
using namespace qtp;
#endif

typedef std::function<void (const fgs::ZooData&)> callback_fn;

namespace fgs{

class ZooElection : public ZooWatcher{
 public:
  explicit ZooElection(ZooKeeperPtr zk, const ZooData& zk_data, const std::string& zk_dir = "");
  virtual ~ZooElection();

  void OnConnected() override;
  void OnCreated(const std::string& path) override;
  void OnDeleted(const std::string& path) override;
  void OnChildChanged(const std::string& path) override;
  // watcher stop call.
  void OnStop() override;

  // register server callback. when register successful call.
  int RegisterCallback(const callback_fn& fn);
#ifdef QTPBASE
  int SetQtpQueue(QtpQueuePtr queue);
#endif

  bool is_master() const;
 private:
  int CreateInstanceNode(const std::string& path);
  int RobMasterNode(const std::string& path);
 private:
  std::string zk_dir_;
  ZooData zk_data_;
  bool is_master_;
  bool is_instance_;
  callback_fn callback_;
#ifdef QTPBASE
  QtpQueuePtr queue_{nullptr};
#endif
};
typedef std::shared_ptr<ZooElection> ZooElectionPtr;

}//!namespace fgs

#endif //FGS_ZOO_ELECTION_H
