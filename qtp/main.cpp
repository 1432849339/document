#include "isonbase.h"
#include "iblog.h"
#include "context.h"
#include "stage.h"
#include "actor.h"
#include "event.h"
#include "ibprotocol.h"
#include "socket.h"
#include "message.h"
#include "shmapi_dll.h"
#include "shmput.h"
#include "ConfigSettings.h"
#include "ukey.h"
#include "quoteread.h"
#include "qtp_session.h"
#include "qtp_message.h"
#include "qtp_manager.h"
#include "qtp_server.h"
#include "qtp_queue.h"
#include "qtp_stage.h"
#include "qtp_version.h"
#include "qtp_framework.h"
#include <thread>
#include <time.h>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fgs_inc.h"
#include "zoo_data.h"
#include "zoo_election.h"
#include "zoo_watcher.h"
using namespace ison::base;
using namespace fgs;
//g_socket相关
int g_pub_hwm = 5000000;
ison::base::Socket *g_socket;//全局Socket，用于接受KMDS接口PUSH的数据
Context ctx;
ConfigSettings cfgSettings;
chronos::SecurityMaster ud;
int ColationTime = 93000000;
int sys_status = 1;
extern std::map<int, Quote> send_map;
std::map<int, std::set<int64_t>> sub_ukey;
std::map<int, std::set<int64_t>> trans_sub_ukey;
std::map<int, std::set<int64_t>> fgs_snap_ukey;
std::set<int64_t> fgs_snap_session;
std::set<int64_t> msp_session;
std::set<int64_t> trans_msp_session;
std::mutex lock_;
extern int sendtime;
extern int prod_time[20];
ZooElectionPtr zoo_watcher = nullptr;


class TestServer : public QtpServer {
public:
	void OnMessage(QtpMessagePtr message, bufferevent* bev) override
	{
		std::string msg;
		if (message->GetDataLen() > 0)
		{
			msg.append((char*)message->GetData(), message->GetDataLen());
			//LOG(INFO) << "recive msg type=" << message->MsgType() << " " << msg;
		}
		int session = QtpSessionMgr::Instance().GetSessionID(bev);
		switch (message->MsgType())
		{
		case 65:
		{
			int count = *(int*)msg.c_str();
			for (int i = 0; i < count; i++)
			{
				//lock.
				lock_.lock();
				sub_ukey[session].insert(*((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i)));
				//msp_ukey.insert(*((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i)));
				lock_.unlock();
				LOG(INFO) << "65 recive " << *((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i));
				
			}
			break;
		}
		case 66:
		{
			int count = *(int*)msg.c_str();
			for (int i = 0; i < count; i++)
			{
				//lock.
				lock_.lock();
				sub_ukey[session].erase(*((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i)));
				//msp_ukey.erase(*((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i)));
				lock_.unlock();
				LOG(INFO) << "66 erase " << *((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i));
			}
			break;
		}
		case 67:
		{
			lock_.lock();
			msp_session.insert(session);	
			lock_.unlock();
			LOG(INFO) << "msp 67 msp_session add " << session;
			break;
		}
		case 75:
		{
			int count = *(int*)msg.c_str();
			for (int i = 0; i < count; i++)
			{
				//lock.
				lock_.lock();
				trans_sub_ukey[session].insert(*((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i)));
				lock_.unlock();
				LOG(INFO) << "75 recive " << *((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i));
				
			}
			break;
		}
		case 76:
		{
			int count = *(int*)msg.c_str();
			for (int i = 0; i < count; i++)
			{
				//lock.
				lock_.lock();
				trans_sub_ukey[session].erase(*((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i)));
				lock_.unlock();
				LOG(INFO) << "76 erase " << *((int64_t*)(msg.c_str() + sizeof(int) + sizeof(int64_t)*i));
			}
			break;
		}
		case 77:
		{
			lock_.lock();
			trans_msp_session.insert(session);	
			lock_.unlock();
			LOG(INFO) << "msp 77 trans_msp_session add " << session;
			break;
		}
		case fgs::kMtSubscribe: 
        {   
			//void *x = nullptr;
        	//QtpMessage::option_size_t x_len = 0;
        	//message->GetOption(59905, (const void **) &x, &x_len);
        	//printf("Option #59905 is %d\n", *(int *) x);
			//int64_t y = 0;
			//message->GetOptionAsInteger(59905, &y);
			message->Topic();
			if(message->Topic() == 5002)
			{
            	int count = msg.length()/sizeof(int64_t);
				if(count == 1)
				{
					if(*(int64_t*)(msg.c_str())	== 0)
					{
						lock_.lock();
						fgs_snap_session.insert(session);
						fgs_snap_ukey.erase(session);
						lock_.unlock();
						LOG(INFO) << "msp 105 fgs_snap_session add " << session;
						break;
						
					}
				}
            	for (int i = 0; i < count; i++)
            	{   
            	    //lock.
            	    lock_.lock();
            	    fgs_snap_ukey[session].insert(*((int64_t*)(msg.c_str() + sizeof(int64_t)*i)));
            	    lock_.unlock();
            	    LOG(INFO) << "105 insert " << *((int64_t*)(msg.c_str() + sizeof(int64_t)*i));
            	}
				
			}  
          	LOG(INFO) << " 105 5002=" << message->Topic();
            break;
        }
		case fgs::kMtUnsubscribe: 
        {   
			//int64_t y = 0;
			//message->GetOptionAsInteger(59905, &y);
			if(message->Topic() == 5002)
			{
				int count = msg.length()/sizeof(int64_t);
				if(count == 1)
				{
					if(*(int64_t*)(msg.c_str())	== 0)
					{
						lock_.lock();
						fgs_snap_session.erase(session);
						fgs_snap_ukey.erase(session);
						lock_.unlock();
						LOG(INFO) << "msp 106 fgs_snap_session delete " << session;
						break;
					}
				}
            	for (int i = 0; i < count; i++)
            	{   
            	    //lock.
            	    lock_.lock();
            	    fgs_snap_ukey[session].erase(*((int64_t*)(msg.c_str() + sizeof(int64_t)*i)));
            	    lock_.unlock();
            	    LOG(INFO) << "106 erase " << *((int64_t*)(msg.c_str() + sizeof(int64_t)*i));
            	}
			}   
          	LOG(INFO) << " 106 5002=" << message->Topic();
            break;
        }   
		case 255:
		{
			LOG(INFO) << "255";
		}
		default:
		{
			LOG(INFO) << "message->MsgType()=" << message->MsgType();
			break;
		}
		}
	}


	void OnAccept(bufferevent *bev, evutil_socket_t fd, struct sockaddr *sa, int socklen) {
		LOG(INFO) << "TestServer::OnAccept!";
		QtpSessionMgr::Instance().AddSession(bev);
		int session = QtpSessionMgr::Instance().GetSessionID(bev);
		LOG(INFO) << "OnAccept " << session;
	}

	void OnClose(bufferevent* bev) override {
		LOG(INFO) << "TestServer::OnClose!";
		lock_.lock();
		sub_ukey.erase(QtpSessionMgr::Instance().GetSessionID(bev));
		msp_session.erase(QtpSessionMgr::Instance().GetSessionID(bev));
		trans_sub_ukey.erase(QtpSessionMgr::Instance().GetSessionID(bev));
		trans_msp_session.erase(QtpSessionMgr::Instance().GetSessionID(bev));
		fgs_snap_ukey.erase(QtpSessionMgr::Instance().GetSessionID(bev));
		lock_.unlock();
		QtpSessionMgr::Instance().RemoveSession(bev);
	}
};

void InitShm(const char* name, int shmid)
{
	int retcode = 0;
	retcode = load_shmdll((char *)cfgSettings.ShmDir.c_str());
	if (retcode != 0)
	{
		LOG(ERROR) << "load_shmdll error retcode=" << retcode;
		exit(retcode);
	}
	retcode = shmInit(shmid, (char*)name);
	if (retcode != 0){
		LOG(ERROR) << "shmInit error retcode=" << retcode;
		exit(retcode);
	}
	LOG(INFO) << "InitShm sucessful！";
}

void Zooker_register()
{
	//ZooKeeperPtr zookeeper = CreateZooKeeper("172.24.13.23:4180", 10);
	ZooKeeperPtr zookeeper = CreateZooKeeper(cfgSettings.ZookeeperUrl, 10);
	if (!zookeeper){
		LOG(ERROR) << "zookeeper create failed.";
		exit(-1);
	}
	ZooData zoo_data;
	zoo_data.service_id = 50;
	zoo_data.instance_id = 1;
	zoo_data.port = cfgSettings.FgsPort;
	zoo_data.lb = kZtMasterSlave;
	strcpy(zoo_data.address, cfgSettings.FgsUrl.c_str());
	zoo_watcher = std::make_shared<ZooElection>(zookeeper, zoo_data);
	zoo_watcher->detach();
	LOG(INFO) << "zookeeper init";
}

int main(int argc, char * argv[])
{
	iblog_init(argv[0], "log");
	iblog_v(1002);//自定义VLOG(v)时，v值小于此处设置值的语句才有输
	iblog_stderrthreshold(2);
	iblog_logbufsecs(0);
	std::string strConfigFile = "./sds20chronos.ini";
	if (argc >1)
	{
		strConfigFile = argv[1];
		LOG(INFO) << "read configfile " << strConfigFile;
	}
	bool bLoadSettings = cfgSettings.LoadSettings(strConfigFile);
	if (!bLoadSettings)
	{
		LOG(INFO) << "LoadSettings faild,please check ", strConfigFile;
		return -2;
	}
	LOG(INFO) << "Read cofigure success!";
	//记录系统启动
	LOG(INFO) << argv[0] << " g_sds2tgw_pub=" << cfgSettings.PubUrl << " g_log_level=" << cfgSettings.LogLevel;


	std::string dir = cfgSettings.UkeyDir;
	if(dir == "")
	{
		std::set<int> marketset;
		marketset.insert(0);
		//int32_t ret = ud.UKOpen(marketset, "tcp://172.24.54.1:3306", "ukdbr", "ukdbr", "ukdb09", gettrday3());
		int32_t ret = ud.UKOpen(marketset, cfgSettings.UkeyUrl, cfgSettings.UkeyUser, cfgSettings.UkeyPass, cfgSettings.UkeyDateBase, 0);
		if (0 != ret)
		{
			LOG(INFO) << "UKOpen faild!";
			return -1;
		}
	}
	else
	{
		if (0 != ud.UKOpen(cfgSettings.UkeyDir))
		{
			LOG(INFO) << "UKOpen faild!";
			return -1;
		}
	}
	//if (0 != ud.UKOpen(secu, cont, mark))
	
	LOG(INFO) << "UKOpen sucess!";
	ColationTime = cfgSettings.ColationTime;

	InitShm("sds20chronos", 9901);

	if(cfgSettings.PlayType == 1 || cfgSettings.PlayType == 0)
	{
		g_socket = new Socket(ctx, SocketType::publish);
		g_socket->Bind(cfgSettings.PubUrl.c_str());
		g_socket->SetSndhwm(g_pub_hwm);
	}
	std::vector<int> date;
	date.push_back(cfgSettings.PlayDate);

	LOG(INFO) << "CreateHServer start";
	if (!CHECK_LIB_VERSION(QTP_FRAMEWORK))
		LOG(ERROR) << "headers and so is not match";
	//QtpLog::SetAsyncMode(65536, true);
	//QtpLog::SetVerbose(100);
	//QtpLog::SetLevel(QtpLog::kTrace);
	QtpManager::Main().Init(true);
	QtpManager::Main().MakeNotifiable();

	if(cfgSettings.PlayType == 4 || cfgSettings.PlayType == 2 || cfgSettings.PlayType == 0)
	{
		TestServer *server = new TestServer();
		QtpManager::Main().AddMember(server);
		struct sockaddr_in sin = { 0 };
		sin.sin_family = AF_INET;
		sin.sin_port = htons(cfgSettings.QtpPort);
		sin.sin_addr.s_addr = inet_addr(cfgSettings.QtpUrl.c_str());

		if (server->Bind((sockaddr *)&sin, sizeof(sin)) != 0) {
			LOG(ERROR) << "hi";
			return -1;
		}
		//std::thread thd([&]() { QtpManager::Main().Start();});
		//thd.detach();
	}
	if(cfgSettings.PlayType == 4 || cfgSettings.PlayType == 3)
	{
		TestServer *server = new TestServer();
		QtpManager::Main().AddMember(server);
		struct sockaddr_in sin = { 0 };
		sin.sin_family = AF_INET;
		sin.sin_port = htons(cfgSettings.FgsPort);
		sin.sin_addr.s_addr = inet_addr(cfgSettings.FgsUrl.c_str());
		if (server->Bind((sockaddr *)&sin, sizeof(sin)) != 0) {
			LOG(ERROR) << "hi";
			return -1;
		}
	}
	std::thread thd([&]() { QtpManager::Main().Start();});
	thd.detach();

	
	Zooker_register();
	
	std::thread sds2tgw_thread[10];
	
	if(cfgSettings.SourceModel == 1 || cfgSettings.SourceModel == 2)
	{
		while(1)
		{
			if(cfgSettings.LVTBlock != "")
			{
				sds2tgw_thread[0] = std::thread(mount_level2);
				//sds2tgw_thread[0].detach();
			}
			if(cfgSettings.CFEBlock != "")
			{
				sds2tgw_thread[1] = std::thread(mount_cfe);
				//sds2tgw_thread[1].detach();	
			}
			if(cfgSettings.SPIBlock != "")
			{
				sds2tgw_thread[2] = std::thread(mount_future);
				//sds2tgw_thread[2].detach();
			}
			if(cfgSettings.IDXBlock != "")
			{
				sds2tgw_thread[3] = std::thread(mount_index);
				//sds2tgw_thread[3].detach();
			}
			if(cfgSettings.FGDBlock != "")
			{
				sds2tgw_thread[4] = std::thread(mount_fgd);
				//sds2tgw_thread[4].detach();	
			}
			if(cfgSettings.FUSBlock != "")
			{
				sds2tgw_thread[5] = std::thread(mount_fus);
				//sds2tgw_thread[5].detach();
			}
			if(cfgSettings.LHKBlock != "")
			{
				sds2tgw_thread[6] = std::thread(mount_level2_hk);
				//sds2tgw_thread[6].detach();
			}
			if(cfgSettings.IHKBlock != "")
			{
				sds2tgw_thread[7] = std::thread(mount_index_hk);
				//sds2tgw_thread[7].detach();
			}
			if(cfgSettings.TRDBlock != "")
			{
				sds2tgw_thread[8] = std::thread(mount_trans);
				//sds2tgw_thread[7].detach();
			}
			for(int i = 0; i < 9; i++)
			{
				if(sds2tgw_thread[i].joinable())
					sds2tgw_thread[i].join();
			}
			LOG(INFO) << "shmput over!";
			ColationTime = cfgSettings.ColationTime;
			sleep(2);
			sys_status = 1;
		};
	}
	else if(cfgSettings.SourceModel == 3)
	{
		std::thread thd_custom = std::thread(std::bind(&SendQueue, cfgSettings.PlaySpeed));
		for(int i = 0; i < date.size(); )
		{
			if(cfgSettings.LVTBlock != "")
			{
				sds2tgw_thread[0] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.LVTBlock));
				//sds2tgw_thread[0].detach();
			}
			if(cfgSettings.CFEBlock != "")
			{
				sds2tgw_thread[1] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.CFEBlock));	
				//sds2tgw_thread[1] = std::thread(std::bind(&read_file_, cfgSettings.FileDir, date[i], cfgSettings.CFEBlock));	
				//sds2tgw_thread[1].detach();	
			}
			if(cfgSettings.SPIBlock != "")
			{
				sds2tgw_thread[2] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.SPIBlock));	
				//sds2tgw_thread[2].detach();
			}
			if(cfgSettings.IDXBlock != "")
			{
				sds2tgw_thread[3] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.IDXBlock));	
				//sds2tgw_thread[3].detach();
			}
			if(cfgSettings.FGDBlock != "")
			{
				sds2tgw_thread[4] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.FGDBlock));	
				//sds2tgw_thread[4].detach();	
			}
			if(cfgSettings.FUSBlock != "")
			{
				sds2tgw_thread[5] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.FUSBlock));	
				//sds2tgw_thread[5].detach();
			}
			if(cfgSettings.LHKBlock != "")
			{
				sds2tgw_thread[6] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.LHKBlock));	
				//sds2tgw_thread[6].detach();
			}
			if(cfgSettings.IHKBlock != "")
			{
				sds2tgw_thread[7] = std::thread(std::bind(&read_file, cfgSettings.FileDir, date[i], cfgSettings.IHKBlock));	
			}
			for(int i = 0; i < 7; i++)
			{
				if(sds2tgw_thread[i].joinable())
					sds2tgw_thread[i].join();
			}
			while(!send_map.empty())
				std::this_thread::sleep_for(std::chrono::seconds(1));
			std::this_thread::sleep_for(std::chrono::seconds(1));
			for(int i = 0; i < 20; i++)
			{
				LOG(INFO) << prod_time[i];
			}
			LOG(INFO) <<"sendtime="<<sendtime;
			sendtime = 93000000;
			LOG(INFO) <<"sendtime="<<sendtime;
		}
		thd_custom.join();
	}
	
	//sds2tgw_thread[4] = std::thread(mount_trans);
	//sds2tgw_thread[5] = std::thread(mount_order);
	//sds2tgw_thread[6] = std::thread(mount_orderqueue);
	//sds2tgw_thread[7] = std::thread(mount_brokerqueue);
	
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
	time_t tt = time(NULL);
	tm * ptm_ = localtime(&tt);
	ptm_->tm_hour = cfgSettings.EndHour;
	ptm_->tm_min = cfgSettings.EndMin;
	ptm_->tm_sec = cfgSettings.EndSec;
	LOG(INFO) << "Main will sleep to " << ptm_->tm_hour << ptm_->tm_min << ptm_->tm_sec;
	using std::chrono::system_clock;
	std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm_)));
}
