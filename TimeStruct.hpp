#ifndef  __TIME_STRUCT_H__
#define __TIME_STRUCT_H__

#include <ctime>
#include <chrono>
#include "include/gflags/gflags.h"

DECLARE_int32(update_hour);
DECLARE_int32(update_minu);
DECLARE_int32(date_distance);


struct GetTrday
{
public:
	GetTrday() : tt(0)
	{
		pstm = nullptr;
	}

	int operator()()
	{
		tt = time(nullptr);
		pstm = localtime(&tt);
		if (pstm->tm_wday == 6)//星期6
		{
			tt -= 1 * 24 * 60 * 60;
			pstm = localtime(&tt);
		}
		else if (pstm->tm_wday == 0)//星期天
		{
			tt -= 2 * 24 * 60 * 60;
			pstm = localtime(&tt);
		}
		else if (pstm->tm_wday == 1)//周一
		{
			if (pstm->tm_hour < FLAGS_update_hour)
			{
				tt -= 3 * 24 * 60 * 60;
				pstm = localtime(&tt);
			}
			else if (pstm->tm_hour == FLAGS_update_hour && pstm->tm_min < FLAGS_update_minu)
			{
				tt -= 3 * 24 * 60 * 60;
				pstm = localtime(&tt);
			}
			else
			{
			}
		}
		else//周二到周五
		{
			if (pstm->tm_hour < FLAGS_update_hour)//判断是否,到达更新时间,没到的交易日为上一天
			{
				tt -= 1 * 24 * 60 * 60;
				pstm = localtime(&tt);
			}
			else if (pstm->tm_hour == FLAGS_update_hour && pstm->tm_min < FLAGS_update_minu)
			{
				tt -= 1 * 24 * 60 * 60;
				pstm = localtime(&tt);
			}
			else
			{
			}
		}
		const int year = pstm->tm_year + 1900;
		const int mon = pstm->tm_mon + 1;
		const int day = pstm->tm_mday;
		return year * 10000 + mon * 100 + day;
	}
	int operator-(int day)
	{
		(*this)();
		tt -= day * 24 * 60 * 60;
		pstm = localtime(&tt);
		const int year = pstm->tm_year + 1900;
		const int mon = pstm->tm_mon + 1;
		day = pstm->tm_mday;
		return year * 10000 + mon * 100 + day;
	}
	int operator+(int day)
	{
		(*this)();
		tt += day * 24 * 60 * 60;
		pstm = localtime(&tt);
		const int year = pstm->tm_year + 1900;
		const int mon = pstm->tm_mon + 1;
		day = pstm->tm_mday;
		return year * 10000 + mon * 100 + day;
	}

	static int64_t GetSecond()
	{
		return time(nullptr);
	}

	static int64_t GetMsTime(int ymd, int hmsu)
	{
		struct tm timeinfo = { 0 };
		timeinfo.tm_year = ymd / 10000 - 1900;
		timeinfo.tm_mon = (ymd % 10000) / 100 - 1;
		timeinfo.tm_mday = ymd % 100;
		timeinfo.tm_hour = hmsu / 10000;
		timeinfo.tm_min = (hmsu % 10000) / 100;
		timeinfo.tm_sec = ((hmsu % 10000) % 100);
		return mktime(&timeinfo);
	}
private:
	struct tm*	pstm = nullptr;
	time_t		tt;
};

#endif
