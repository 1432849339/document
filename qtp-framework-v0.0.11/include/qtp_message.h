//
// Created by wen on 2016-04-15.
//

#ifndef QTP_FRAMEWORK_QTP_MESSAGE_H
#define QTP_FRAMEWORK_QTP_MESSAGE_H

#include <stdint.h>
#include <map>
#include <string>
#include "qtp_util.h"

namespace qtp {

//MessageType for qtp framework
// 1 - 999
enum QtpMessageType {
  kMtTimeout = 11,
};

//OptionType for qtp framework
//1 - 999
enum QtpOptionType {
  kOtTimeoutId = 11,
};

class QtpMessage {
public:
  static const uint8_t kCurrentVersion = 0x6A;
  typedef uint16_t option_id_t;
  typedef uint16_t option_size_t;
  static const size_t kOptionHeadSize = sizeof(option_id_t) + sizeof(option_size_t);

  struct MsgHead {
    uint8_t version = 0;
    uint8_t service = 0;
    uint16_t msgtype = 0;
    uint16_t topic = 0;
    uint16_t optslen = 0;
    uint32_t datalen = 0;
  };
  static const uint32_t kMsgHeadSize = sizeof(MsgHead);

  enum EncodeOption {
    kEnNone = 0,
    kEnOption = 1,
    kEnData = 2,
    kEnHead = 4,
    kEnNormal = kEnOption | kEnData,
    kEnAll = kEnHead | kEnOption | kEnData,
  };

  struct Option {
    uint16_t id = 0;
    uint16_t len = 0;
    const void *opt = nullptr;
  };

  QtpMessage();
  virtual ~QtpMessage();

  void Clear();

  //msg head
  MsgHead Head() const;
  const void* HeadBuffer() const;
  uint8_t Version() const;
  uint8_t Service() const;
  uint16_t MsgType() const;
  uint16_t Topic() const;
  uint16_t OptsLen() const;
  uint32_t DataLen() const;

  //Decode
  int Decode(const MsgHead& head, uint32_t offset = 0);
  int Decode();

  //Encode
  int BeginEncode(uint16_t msgtype, uint8_t service = 0, uint16_t topic = 0, uint8_t version = kCurrentVersion);
  int AddOption(option_id_t id, const void *opt, option_size_t len);
  int CopyOption(const QtpMessage& message, option_id_t id);
  int RemoveOption(option_id_t id);
  int RemoveAllOptions();
  int SetData(const void* data, uint32_t data_len, bool data_free = true);
  size_t Encode(EncodeOption option = kEnNormal);
  size_t GetEncodedBufferLen() const;
  EncodeOption GetEncodeOption() const { return encode_option_; }
  bool IsEncoded() const { return encode_option_ != kEnNone; }

  //options
  int GetOption(option_id_t id, const void **opt, option_size_t* len) const;
  template<typename T> int GetOptionAsInteger(option_id_t id, T* opt) const;
  const void* GetOptionsBuffer() const;
  size_t GetOptionsLen() const;

  //data
  const void* GetData() const;
  size_t GetDataLen() const;

  //buffer
  char* AllocBuffer(size_t len);
  char* GetBuffer() const;

  //tags is options for application, it will not be encoded, and we always copy the tag
  int AddTag(option_id_t id, const void *tag, option_size_t len);
  int CopyTag(const QtpMessage& message, option_id_t id);
  int RemoveTag(option_id_t id);
  int RemoveAllTags();
  int GetTag(option_id_t id, const void **tag, option_size_t* len) const;
  template<typename T> int GetTagAsInteger(option_id_t id, T* tag) const;

private:
  DISALLOW_COPY_AND_ASSIGN(QtpMessage);
  size_t EncodeHead(char *buf);
  size_t EncodeOptions(char *buf);
  size_t EncodeData(char *buf);
  void FreeData();

  MsgHead msghead_;

  std::map<option_id_t, Option> options_;
  const void* options_p_ = nullptr;
  option_size_t  options_len_ = 0;

  const void *data_ = nullptr;
  size_t data_len_ = 0;
  bool data_free_ = false;

  char* buffer_ = nullptr;
  size_t buffer_len_ = 0;
  EncodeOption encode_option_ = kEnNone;

  std::map<uint32_t, std::string> tags_;
};
DEFINE_SHARED_PTR(QtpMessage);


template<typename T>
int QtpMessage::GetOptionAsInteger(option_id_t id, T* opt) const {
  auto it = options_.find(id);
  if (it == options_.end()) return -1;
  if (it->second.len != sizeof(T))
    return -1;
  if (opt) *opt = *(T*)it->second.opt;
  return 0;
}

template<typename T>
int QtpMessage::GetTagAsInteger(option_id_t id, T* tag) const {
  auto it = tags_.find(id);
  if (it == tags_.end()) return -1;
  if (it->second.size() != sizeof(T))
    return -1;
  if (tag) *tag = *(T*)it->second.data();
  return 0;
}

} //namespace qtp

#endif //QTP_FRAMEWORK_QTP_MESSAGE_H
