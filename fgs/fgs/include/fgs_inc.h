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
#ifndef FGS_FGS_INC_H
#define FGS_FGS_INC_H

namespace fgs{

static const int fgs_service_id = 0;

enum MessageOption{
  // (0, 59900) for all application service event

  // [59900, 60000] for fgs system event option
  kMoItemSize = 59901,  // event data single struct size.
  kMoItemCnt = 59902,   // event data struct counts.
  kMoInstanceID = 59903,  // service instance id.
  kMoSessionID = 59904,   // access session id.
  kMoSubscribeKey = 59905,  // subscribe key.

  // [60000, above] for qtp_base system event option
};

enum MessageType{
  // [0, 1000] for fgs system event
  kMtFgsStdAns = 100,    // fgs system std ans event.
  kMtLogin = 101,        // fgs system login event.
  kMtLoginAns = 102,     // fgs system login ans event.
  kMtLogout = 103,     // fgs system logout event.
  kMtLogoutAns = 104,  // fgs system logout ans event.

  kMtSubscribe  = 105,   // fgs system subscribe event.
  kMtUnsubscribe = 106,  // fgs system unsubscribe event.
  kMtPublish = 107,      // fgs system publish event.

  kMtComboSubscribe = 108, // fgs system combo subscribe event
  kMtComboUnsubscribe = 109, // fgs system combo unsubscribe event
  kMtComboPublish = 110, // fgs system combo publish event

  // [1000, 60000] for all application service event.

  // [60000, above] for qtp_base system event
};

enum MessageRetCode{
  // fgs std return code.
  kRetOK = 0,  // return ok
  kRetStdError = -1, // un define error code.
  kRetUserNoLogin = -2, // user not logged in fgs
  kRetServiceNotOnline = -3, // service not online
  kRetMessageNotCorrect = -4, // the message package not correct(not found instance_id)
  kRetSubscribeNotCorrect = -5, // the subscribe message not correct
  kRetUnsubscribeNotCorrect = -6, // the unsubscribe message not correct
  kRetComboSubscribeNotCorrect = -7, // the combo subscribe message not correct
  kRetComboUnsubscribeNotCorrect = -8, // the combo unsubscribe message not correct
};

enum MessageTopic{
  // (0, 60000) for all application service topic
  // fgs service_id = 0
  kTopicServiceClose = 1,

};

}//!namespace fgs

#endif //FGS_FGS_INC_H
