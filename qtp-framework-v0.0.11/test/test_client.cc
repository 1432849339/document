//
// Created by wen on 2016-04-12.
//

#ifndef _WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <string.h>
#include "qtp_manager.h"
#include "qtp_server.h"
#include "qtp_message.h"
#include "qtp_client.h"
#include "qtp_session.h"
#include "qtp_version.h"
#include "qtp_framework.h"
#include "qtp_log.h"
#include "test/LoginQry.h"
#include "test/LoginAns.h"
#include "test/MessageHeader.h"

#define BUF_MAX 2048

static const int kServiceID = 120;
static const int kTestSessionClientId = 100;

enum Option {
  kOpX = 8,
};

using namespace qtp;
using namespace test;

const char* server_ip = "127.0.0.1";
int server_port = 2015;


class TestClient : public QtpClient {

public:
  TestClient() : QtpClient(true) { }

  void OnConnected() {
    LOG(info) << "TestClient::OnConnected";
    QtpSessionMgr::Instance().AddSession(kTestSessionClientId, client_);

    char *data = new char[BUF_MAX];
    LoginQry qry;
    MessageHeader hdr;
    try {
      hdr.wrap(data, 0, 0, BUF_MAX)
          .blockLength(LoginQry::sbeBlockLength())
          .templateId(LoginQry::sbeTemplateId())
          .schemaId(LoginQry::sbeSchemaId())
          .version(LoginQry::sbeSchemaVersion());
      qry.wrapForEncode(data, hdr.encodedLength(), BUF_MAX).capMgrId(1234).capUnitId(5678).tradeUnitId(2);
      qry.trader().id(8888).type(1);
    } catch (std::exception &e) {
      printf("not enought buffer\n");
      return;
    }

    QtpMessagePtr message = std::make_shared<QtpMessage>();
    message->BeginEncode(LoginQry::sbeTemplateId(), kServiceID);
    int x = 8888;
    message->AddOption(kOpX, &x, sizeof(x));
    message->GetOptionAsInteger(kOpX, &x);
    message->SetData(data, hdr.encodedLength() + qry.encodedLength(), true); //data由QtpMessage释放
#if 1 //注意Encode方式的不同
    message->Encode(QtpMessage::kEnOption); //只encode选项区
    SendMessage(message);
#else
    message->Encode(QtpMessage::kEnAll);
    message->SetData(message->GetBuffer(), message->GetEncodedBufferLen());
#endif
    size_t len = message->GetEncodedBufferLen();
    printf("write: %lu\n", len);

    timeval tv = {10, 0};
    AddTimeout(2, tv);
  }

  void OnMessage(QtpMessagePtr message, bufferevent *bev) override {
    switch (message->MsgType()) {
      case 20000:
        printf("message: %d\n", message->MsgType());
        break;
      case 10002: {
        MessageHeader hdr;
        LoginAns ans;
        try {
          hdr.wrap((char *) message->GetData(), 0, 0, message->DataLen());
          ans.wrapForDecode((char *) message->GetData(), hdr.encodedLength(), hdr.blockLength(), hdr.version(),
                            message->DataLen());
        } catch (std::exception &e) {
          printf("wrong message\n");
          return;
        }
        printf("capMgr:%d, capUnit:%d, tradeUnit:%d, trader:%d\n", ans.capMgrId(), ans.capUnitId(), ans.tradeUnitId(),
               ans.traderId());
        break;
      } //case
    } //switch
  }

};

DECLARE_VERSION(test_client, 0, 0, 1)
DEFINE_VERSION(test_client)

int main(int argc, char* argv[]) {

  QtpLog::SetLevel(QtpLog::kTrace);

  GET_VERSION_STR(QTP_FRAMEWORK);

  if (!CHECK_LIB_VERSION(QTP_FRAMEWORK))
    LOG(emerg) << "headers and so is not match";

  if (argc > 1)
    server_ip = argv[1];

  struct sockaddr_in sin;

  QtpManager::Main().Init();

  TestClient* client = new TestClient();
  if (client->Init() != 0) {
    printf("client init error!\n");
    return -1;
  }

  QtpManager::Main().AddMember(client);

  ::memset(&sin, 0, sizeof(sin));
  if (::evutil_inet_pton(AF_INET, server_ip, &sin.sin_addr) == 0) {
    printf("wrong server ip: %s\n", server_ip);
    return -1;
  }
  sin.sin_family = AF_INET;
  sin.sin_port = htons(server_port);
  client->Connect((sockaddr*)&sin, sizeof(sin));

  QtpManager::Main().Start();
  printf("done\n");
  return 0;
}