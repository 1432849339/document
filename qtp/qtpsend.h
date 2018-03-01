#ifndef _SDS_QTP_SEND_H_
#define _SDS_QTP_SEND_H_
#include <algorithm>
#include <set>
#include "quote.h"
#include "qtp_session.h"
#include "qtp_manager.h"
#include "qtp_server.h"
#include "qtp_queue.h"
#include "qtp_stage.h"
#include "qtp_version.h"
#include "qtp_framework.h"
#include "iblog.h"
#include "ConfigSettings.h"
#include "fgs_inc.h"
using namespace qtp;

extern ConfigSettings cfgSettings;
extern std::map<int, std::set<int64_t> > sub_ukey;
extern std::map<int, std::set<int64_t> > trans_sub_ukey;
extern std::map<int, std::set<int64_t> > fgs_snap_ukey;
extern std::set<int64_t> msp_session;
extern std::set<int64_t> trans_msp_session;
extern std::set<int64_t> fgs_snap_session;

//int MessageSend_Snap(Quote& send, int& key);
int MessageSend_Snap(Snapshot& sds_tdf_data, int& key);
int MessageSend_Snap(Transaction& sds_tdf_trans, int& key);
int MessageSend_qtp(Snapshot& sds_tdf_data, int &key);
 
inline int MessageSend_Snap(Quote& send, int& key)
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
		if(msp_session.size() > 0)
		{
			for(auto it = msp_session.begin(); it != msp_session.end(); it++)
			{
				QtpMessagePtr messagereply = std::make_shared<QtpMessage>();
				messagereply->BeginEncode(2112);
				messagereply->SetData(&spt, sizeof(Snapshot), false);
				messagereply->Encode(QtpMessage::kEnAll);
				//qtp::QtpSessionMgr::Instance().BroadcastMessage(messagereply);
				qtp::QtpSessionMgr::Instance().SendMessage(*it, messagereply);
				LOG_IF(INFO, cfgSettings.LogLevel > 9) << "send to msp "<< *it << " key=" << key << " "<< spt.ukey << " "<< spt.time;
			}
		}
		//}
	}
	return 0;
}


#endif
