/*
 * AccessControler.h
 * 访问权限控制类
 * Created on: 2010-8-25
 * Author: leo
 */

#ifndef _ACCESSCONTROLER_H

#define _ACCESSCONTROLER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include "configure.h"
#include "AccessControlerUtils.h"
#include "log.h"

using namespace std;

typedef vector<string> IpList;

typedef map<string, IpList> VisitorIpMap;	//配置文件用户对应的合法IP列表

typedef struct AllowInfo {
	long amount;
	long freq;
	vector<string> apiList;
} AllowInfo;

typedef map<string, AllowInfo> VisitorDstserviceAllowInfoMap;	//配置文件用户对应的合法访问次数、访问频率和API名称

typedef deque<time_t> TimeArray;

typedef struct VisitorHistory {
	bool banFlag;
	long curAmount;
	long freq;
	time_t firstVisitTime;
	time_t lastVisitTime;
	TimeArray timeArray;
} VisitHistory;

typedef map<string, VisitorHistory> VisitorHistoryMap;	//用户动态信息

class AccessControler {
public:
	//最大访问次数时间周期
	static long period4Amount;
	//最大访问频率时间周期
	static long period4Freq;
	//访问者IP列表，存放访问者对应的合法IP地址列表，从配置文件读取
	static VisitorIpMap visitorIpMap;
	//访问者服务端允许访问列表，存放访问者服务端对应的合法访问次数、访问频率和API名称，从配置文件读取
	static VisitorDstserviceAllowInfoMap visitorDstserviceAllowInfoMap;
	//访问者访问时间历史记录，动态统计
	static VisitorHistoryMap visitorHistoryMap;

	//初始化配置信息和访问历史信息：0-初始化成功，其他-失败
	static int initialize();

	//载入 访问控制相关配置：0-重新载入成功，其他-失败
	static int loadVisitorConfigure();

	//检查是否禁止访问：0-允许访问，其他-禁止访问
	static int check(const char* visitor, const char* dstService,
			const char* apiName, const char* visitorIp, string &reason);

	//手动禁止访问：0-禁止成功，其他-失败
	static int ban(const char* visitor, const char* dstService);

	//手动解禁：0-解禁成功，其他-失败
	static int unban(const char* visitor, const char* dstService);

	//将用户动态信息dump到缓存中
	static int dumpVisitorHistory(char *buf, int len);

    //讲访问表dump到缓存中
    static int dumpAccessTable(char* buf, int len);

    //
    static int dumpAccessTableToFile(fstream& out);

	//从缓存load用户动态信息
	//param buf:dump出来的用户动态信息
	static int loadVisitorHistory(char *buf);

	//打印VisitorIpMap
	static void showVisitorIpMap();

	//打印VisitorDstserviceAllowInfoMap
	static void showVisitorDstserviceAllowInfoMap();

	//打印VisitorHistoryMap
	static void showVisitorHistoryMap();
};

#endif  /* _ACCESSCONTROLER_H */

