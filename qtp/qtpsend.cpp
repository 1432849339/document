#include "qtpsend.h"
#include "iblog.h"
#include "ConfigSettings.h"
#include "tdf2sbepub.h"
extern std::mutex lock_;
//extern ConfigSettings cfgSettings;
//extern std::map<int, std::set<int64_t> > sub_ukey;
//extern std::set<int64_t> msp_ukey;
//extern int msp_session;
/* 
int MessageSend_Snap(Quote& send, int& key)
{
	for (auto it_data = send.snap.begin(); it_data != send.snap.end(); it_data++)
	{
		Snapshot& spt = *it_data;
		for(auto it = sub_ukey.begin(); it != sub_ukey.end(); it++)
		{
			std::set<int64_t>& tmp = it->second;
			std::set<int64_t>::iterator result = std::find(tmp.begin(), tmp.end(), spt.ukey);
			if(result == tmp.end())
				;
			else
			{
				QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
				messagereply->BeginEncode(2112);
				messagereply->SetData(&spt, sizeof(Snapshot), false);
				messagereply->Encode(QtpMessage::kEnAll);
				//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
				qtp::QtpSessionMgr::Instance().SendMessage(it->first, messagereply);
				LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to "<< it->first << " key=" << key << " "<< spt.ukey << " "<< spt.time;
			}
		}
		//std::set<int64_t>::iterator result = std::find(msp_ukey.begin(), msp_ukey.end(), spt.ukey);
		//if(result == msp_ukey.end())
		//	;
		//else
		//{
		if(msp_session > 0)
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2112);
			messagereply->SetData(&spt, sizeof(Snapshot), false);
			messagereply->Encode(QtpMessage::kEnAll);
			//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
			qtp::QtpSessionMgr::Instance().SendMessage(msp_session, messagereply);
			LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to msp "<< msp_session << " key=" << key << " "<< spt.ukey << " "<< spt.time;
		}
		//}
	}
	return 0;
}
*/
int MessageSend_Snap(Transaction& sds_tdf_trans, int& key)
{
	for(auto it = trans_sub_ukey.begin(); it != trans_sub_ukey.end(); it++)
	{
		std::set<int64_t>& tmp = it->second;
		std::set<int64_t>::iterator result = std::find(tmp.begin(), tmp.end(), sds_tdf_trans.ukey);
		if(result == tmp.end())
			;
		else
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2114);
			messagereply->SetData(&sds_tdf_trans, sizeof(Transaction), false);
			messagereply->Encode(QtpMessage::kEnAll);
			qtp::QtpSessionMgr::Instance().SendMessage(it->first, messagereply);
			LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send trans to " << it->first << " Datalen=" << messagereply->DataLen() << " optlen=" << messagereply->OptsLen() << " key=" << key << " "<< sds_tdf_trans.ukey << " "<< sds_tdf_trans.time;
		}
	}
	lock_.lock();
	if(trans_msp_session.size() > 0)
	{
		for(auto it = msp_session.begin(); it != msp_session.end(); it++)
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2114);
			messagereply->SetData(&sds_tdf_trans, sizeof(Transaction), false);
			messagereply->Encode(QtpMessage::kEnAll);
			qtp::QtpSessionMgr::Instance().SendMessage(*it, messagereply);
			//LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to msp "<< *it << " key=" << key << " "<< sds_tdf_data.ukey << " "<< sds_tdf_data.time;
		}
	}
	lock_.unlock();
	return 0;
}
/*
int MessageSend_Snap(Snapshot& sds_tdf_data, int &key)
{
	for(auto it = sub_ukey.begin(); it != sub_ukey.end(); it++)
	{
		std::set<int64_t>& tmp = it->second;
		std::set<int64_t>::iterator result = std::find(tmp.begin(), tmp.end(), sds_tdf_data.ukey);
		if(result == tmp.end())
			;
		else
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2112);
			messagereply->SetData(&sds_tdf_data, sizeof(Snapshot), false);
			messagereply->Encode(QtpMessage::kEnAll);
			qtp::QtpSessionMgr::Instance().SendMessage(it->first, messagereply);
			//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
			LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to " << it->first << " key=" << key << " "<< sds_tdf_data.ukey << " "<< sds_tdf_data.time;
		}
	}
	//std::set<int64_t>::iterator result = std::find(msp_ukey.begin(), msp_ukey.end(), sds_tdf_data.ukey);
	//if(result == msp_ukey.end())
	//	;
	//else
	lock_.lock();
	if(msp_session.size() > 0)
	{
		for(auto it = msp_session.begin(); it != msp_session.end(); it++)
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2112);
			messagereply->SetData(&sds_tdf_data, sizeof(Snapshot), false);
			messagereply->Encode(QtpMessage::kEnAll);
			//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
			qtp::QtpSessionMgr::Instance().SendMessage(*it, messagereply);
			//LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to msp "<< *it << " key=" << key << " "<< sds_tdf_data.ukey << " "<< sds_tdf_data.time;
		}
	}
	lock_.unlock();
	return 0;
}
*/
int MessageSend_qtp(Snapshot& sds_tdf_data, int &key)
{
	std::string message_json;
	tdf2json(&sds_tdf_data, message_json);
	for(auto it = fgs_snap_ukey.begin(); it != fgs_snap_ukey.end(); it++)
	{
		std::set<int64_t>& tmp = it->second;
		std::set<int64_t>::iterator result = std::find(tmp.begin(), tmp.end(), sds_tdf_data.ukey);
		if(result == tmp.end())
			;
		else
		{
			//std::string message_json;
			//tdf2json(&sds_tdf_data, message_json);
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			//messagereply->BeginEncode(3112);
			messagereply->BeginEncode(fgs::kMtPublish, 0, 5002);
			//kMoSubscribeKey
			messagereply->AddOption(fgs::kMoSubscribeKey, &sds_tdf_data.ukey, sizeof(int64_t));
			messagereply->SetData(message_json.c_str(), message_json.length(), false);
			messagereply->Encode(QtpMessage::kEnAll);
			qtp::QtpSessionMgr::Instance().SendMessage(it->first, messagereply);
			LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to fgs " << it->first << " key=" << key << " "<< sds_tdf_data.ukey << " "<< sds_tdf_data.time;
		}
	}
	lock_.lock();
	if(fgs_snap_session.size() > 0)
	{
		for(auto it = fgs_snap_session.begin(); it != fgs_snap_session.end(); it++)
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			//messagereply->BeginEncode(3112);
			messagereply->BeginEncode(fgs::kMtPublish, 0, 5002);
			messagereply->AddOption(fgs::kMoSubscribeKey, &sds_tdf_data.ukey, sizeof(int64_t));
			messagereply->SetData(message_json.c_str(), message_json.length(), false);
			messagereply->Encode(QtpMessage::kEnAll);
			qtp::QtpSessionMgr::Instance().SendMessage(*it, messagereply);
			LOG_IF(INFO, cfgSettings.LogLevel > 15) << "send to fgs " << message_json;
		}
	}
	lock_.unlock();
}

int MessageSend_Snap(Snapshot& sds_tdf_data, int &key)
{
	for(auto it = sub_ukey.begin(); it != sub_ukey.end(); it++)
	{
		std::set<int64_t>& tmp = it->second;
		std::set<int64_t>::iterator result = std::find(tmp.begin(), tmp.end(), sds_tdf_data.ukey);
		if(result == tmp.end())
			;
		else
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2112);
			messagereply->SetData(&sds_tdf_data, sizeof(Snapshot), false);
			messagereply->Encode(QtpMessage::kEnAll);
			qtp::QtpSessionMgr::Instance().SendMessage(it->first, messagereply);
			//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
			LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to sss " << it->first << " key=" << key << " "<< sds_tdf_data.ukey << " "<< sds_tdf_data.time;
		}
	}
	
	lock_.lock();
	if(msp_session.size() > 0)
	{
		for(auto it = msp_session.begin(); it != msp_session.end(); it++)
		{
			QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
			messagereply->BeginEncode(2112);
			messagereply->SetData(&sds_tdf_data, sizeof(Snapshot), false);
			messagereply->Encode(QtpMessage::kEnAll);
			//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
			qtp::QtpSessionMgr::Instance().SendMessage(*it, messagereply);
			//LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to msp "<< *it << " key=" << key << " "<< sds_tdf_data.ukey << " "<< sds_tdf_data.time;
		}
	}
	lock_.unlock();
	return 0;
}
