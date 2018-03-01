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
#ifndef FGS_ZOO_WATCHER_H
#define FGS_ZOO_WATCHER_H
#include <memory>
#include <string>
#include <atomic>

namespace fgs{

class ZooKeeper;
typedef std::shared_ptr<ZooKeeper> ZooKeeperPtr;

// host: (ip:port)
// timeout: second
// log_level: enum {ERROR=1,WARN=2,INFO=3,DEBUG=4}
ZooKeeperPtr CreateZooKeeper(const std::string& host, int timeout = 30, uint8_t log_level = 1);

// class ZooWatcher
class ZooWatcher{
 public:
  explicit ZooWatcher(ZooKeeperPtr zookeeper);
  virtual ~ZooWatcher();

  // watch loop
  void join();
  void detach();
  void stop();

  bool is_connected() const;
  bool is_expired() const;
  bool is_watching() const;
 protected:
  void WatchHandle(int type, int state, const std::string& path);
  //
  virtual void OnConnected();
  virtual void OnConnecting();
  virtual void OnSessionExpired();

  virtual void OnCreated(const std::string& path);
  virtual void OnDeleted(const std::string& path);
  virtual void OnChanged(const std::string& path);
  virtual void OnChildChanged(const std::string& path);
  virtual void OnNotWatching(const std::string& path);

  virtual void OnStop();
 protected:
  ZooKeeperPtr zookeeper_ptr_;
 private:
  bool is_connected_;
  bool is_expired_;
  std::atomic<bool> is_watching_;
};

}//!namespace fgs

#endif //FGS_ZOO_WATCHER_H
