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
#ifndef FGS_ZOO_DATA_H
#define FGS_ZOO_DATA_H
#include <memory>

namespace fgs{

enum ZooLoadBalance{
  kZtNone = 0,        // none mode.(route by instance_id)
  kZtMasterSlave = 1, // master-slave mode
};

struct ZooData{
  int64_t service_id;
  int64_t instance_id;
  int32_t lb; // load balance algorithm
  int32_t port;
  char address[32];
};
typedef std::shared_ptr<ZooData> ZooDataPtr;

enum ZooMessageType{
  kMtGeodeCreated = 65101,
};

}//!namespace fgs


#endif //FGS_ZOO_DATA_H
