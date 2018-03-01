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

#include <stdint.h>
#include <inttypes.h>
#include <thread>
#include "qtp_session.h"
#include "qtp_manager.h"
#include "qtp_server.h"
#include "qtp_queue.h"
#include "qtp_stage.h"
#include "qtp_log.h"
#include "qtp_version.h"
#include "qtp_framework.h"
#include "test/MessageHeader.h"
#include "test/LoginQry.h"
#include "test/LoginAns.h"

#define PORT 2015
#define BUF_MAX 2048

static const int kServiceID = 120;
static const uint32_t kStageAID = 108;
static const uint32_t kStageBID = 109;

enum Option {
  kOpX = 8,
};

enum Tag {
  kTagSession = 1,
};

using namespace qtp;
using namespace test;

class TestServer : public QtpServer {
public:
  void OnMessage(QtpMessagePtr message, bufferevent* bev) override {

    session_id_t session_id = QtpSessionMgr::Instance().GetSessionID(bev);

    message->AddTag(kTagSession, &session_id, sizeof(session_id));

    QtpQueueMgr::Instance().SendMessage(kStageAID, message);
  }

  void OnAccept(bufferevent *bev, evutil_socket_t fd, struct sockaddr *sa, int socklen) {
    LOG(info) << "TestServer::OnAccept!";
    QtpSessionMgr::Instance().AddSession(bev);

    timeval tv = {1, 0};
    AddTimeout(1, tv);
  }

  void OnClose(bufferevent* bev) override {
    LOG(info) << "TestServer::OnClose!";
    QtpSessionMgr::Instance().RemoveSession(bev);
    DelTimeout(1);
  }

  void OnTimeout(QtpTimeout *timeout) override {
    LOG(info)  << "TestServer on timeout id: " << timeout->id();
  }

};

class StageA : public QtpStage {
public:
  int OnEvent(QtpMessagePtr message) {
    printf("StageA OnEvent: %hu\n", message->MsgType());
    timeval tv = {1,0};
    AddTimeout(2, tv);

    switch(message->MsgType()) {
      case kMtTimeout: {
        int id = 0;
        if (message->GetTagAsInteger(kOtTimeoutId, &id) != 0)
          return -1;
        printf("StageA timeoutid: %d\n", id);
        DelTimeout(2);
        break;
      }
      case 10001: {
        void *x = nullptr;
        QtpMessage::option_size_t x_len = 0;
        message->GetOption(kOpX, (const void **) &x, &x_len);
        printf("Option #kOpX is %d\n", *(int *) x);

        int x_value = 0;
        message->GetOptionAsInteger(kOpX, &x_value);
        printf("Option #kOpX is %d\n", x_value);

        session_id_t client_session = 0;
        message->GetTagAsInteger(kTagSession, &client_session);
        printf("client session id: %" PRIu64 "\n", client_session);

        MessageHeader hdr;
        hdr.wrap((char *) message->GetData(), 0, 0, message->DataLen());
        LoginQry qry;
        qry.wrapForDecode((char *) message->GetData(), hdr.encodedLength(), hdr.blockLength(), hdr.version(),
                          message->DataLen());
        printf("capMgr:%d, capUnit:%d, tradeUnit:%d, trader:%d\n", qry.capMgrId(), qry.capUnitId(), qry.tradeUnitId(),
               qry.trader().id());
      }
      default:
        break;
    }
    QtpQueueMgr::Instance().SendMessage(kStageBID, message);
    return 0;
  }

};

class StageB : public QtpStage {
public:
  int OnStart() override {
    timeval tv = {0,100};
    AddTimeout(3, tv);
    return 0;
  }

  int OnEvent(QtpMessagePtr message) override {
    printf("StageB OnEvent: %hu\n", message->MsgType());

    switch(message->MsgType()) {
      case kMtTimeout: {
        int id = 0;
        if (message->GetTagAsInteger(kOtTimeoutId, &id) != 0)
          return -1;
        printf("StageB timeoutid: %d\n", id);
        auto message =  std::make_shared<QtpMessage>();
        message->BeginEncode(20000);
        message->Encode();
        QtpSessionMgr::Instance().BroadcastMessage(message);
        break;
      }
      case 10001: {

        session_id_t client_session = 0;
        message->GetTagAsInteger(kTagSession, &client_session);
        printf("client session id: %" PRIu64 "\n", client_session);

        //response
        char* data = new char[BUF_MAX];
        LoginAns ans;
        MessageHeader hdr;
        try {
          hdr.wrap(data, 0, 0, BUF_MAX)
              .blockLength(LoginQry::sbeBlockLength())
              .templateId(LoginQry::sbeTemplateId())
              .schemaId(LoginQry::sbeSchemaId())
              .version(LoginQry::sbeSchemaVersion());
          ans.wrapForEncode(data, hdr.encodedLength(), BUF_MAX).capMgrId(1234).capUnitId(5678).tradeUnitId(2);
          ans.traderId(8888);
        } catch (std::exception &e) {
          printf("not enought buffer\n");
          break;
        }

        auto response = std::make_shared<QtpMessage>();
        response->BeginEncode(LoginAns::sbeTemplateId(), kServiceID);
        response->SetData(data, hdr.encodedLength() + ans.encodedLength());
        response->Encode(QtpMessage::kEnAll);

        QtpSessionMgr::Instance().SendMessage(client_session, response);
      }
      default:
        break;
    }

    return 0;
  }
};

int main(int argc, char* argv[]) {

  QTPLOG_TRY {

    LOG(info) << "server start";

    LOG(info) << GET_VERSION_NUM(QTP_FRAMEWORK);
    LOG(info) << GET_VERSION_STR(QTP_FRAMEWORK);

    if (!CHECK_LIB_VERSION(QTP_FRAMEWORK))
      LOG(emerg) << "headers and so is not match";

    QtpLog::SetAsyncMode(65536, true);
    QtpLog::SetVerbose(100);
    QtpLog::SetLevel(QtpLog::kTrace);
    //QtpLog::Create("test_server");

    LOG(info) << "info";
    LOG(error) << "error";
    LOG(debug) << "debug";
    LOG(trace) << "trace";
    LOG(emerg) << "emerg";
    VLOG(0) << "v: " << 0;
    VLOG(100) << "v: " << 100;
    VLOG(1000) << "v: " << 100;

    QtpManager::Main().Init(true);
    QtpManager::Main().MakeNotifiable(); //多线程支持

    StageA stageA;
    stageA.set_queue(QtpQueueMgr::Instance().CeateQueue(kStageAID));
    std::thread tha([&stageA]() { stageA.Run(); });

    StageB stageB;
    stageB.set_queue(QtpQueueMgr::Instance().CeateQueue(kStageBID));
    std::thread thb([&stageB]() { stageB.Run(); });

    TestServer *server = new TestServer();
    QtpManager::Main().AddMember(server);

    struct sockaddr_in sin = {0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    if (server->Bind((sockaddr *) &sin, sizeof(sin)) != 0) {
      LOG(error) << "hi";
      return -1;
    }

    QtpManager::Main().Start();

    tha.join();
    thb.join();

  } QTPLOG_CATCH

  return 0;
}