/*
 * AccessControler.cpp
 * 访问权限控制类
 * Created on: 2010-8-25
 * Author: leo
 */

#include "AccessControler.h"

//初始化AccessControler类的静态成员
long AccessControler::period4Amount = 0;
long AccessControler::period4Freq = 0;
VisitorIpMap AccessControler::visitorIpMap;
VisitorDstserviceAllowInfoMap AccessControler::visitorDstserviceAllowInfoMap;
VisitorHistoryMap AccessControler::visitorHistoryMap;

//初始化配置信息和访问历史信息：0-初始化成功，其他-失败
//param buf:dump出来的用户动态信息
int AccessControler::initialize(){
	int flag;
	flag = AccessControler::loadVisitorConfigure();
	if(0 != flag){
		LOG_ERROR("[AccessControler::initialize] loadVisitorConfigure() failed !");
		return -1;
	}

	return 0;
}

//载入 访问控制相关配置：0-重新载入成功，其他-失败
int AccessControler::loadVisitorConfigure() {
	AccessControler::visitorIpMap.clear();
	AccessControler::visitorDstserviceAllowInfoMap.clear();

	//加载配置文件
	utils::Configure configure;
	int loadFlag = configure.load("visit_ctl.conf");
	if (0 != loadFlag) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] Load visit_ctrl_conf configure file error !");
		return -1;
	}

	int rc;

	//初始化最大访问次数时间周期
	rc = configure.get_single("visit_ctl_config", "period_for_amount", AccessControler::period4Amount);
	if (0 != rc) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'period_for_amount' error !");
		return -1;
	}
	if (0 >= AccessControler::period4Amount) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] Period4Amount must be greater than 0 !");
		return -1;
	}

	//初始化最大访问频率时间周期
	rc = configure.get_single("visit_ctl_config", "period_for_freq", AccessControler::period4Freq);
	if (0 != rc) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'period_for_freq' error !");
		return -1;
	}
	if (0 >= AccessControler::period4Freq) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] period4Freq must be greater than 0 !");
		return -1;
	}

	//初始化访问者对应的IP地址列表
	int count;
	rc = configure.count1_array2("visit_ctl_config", "visitor_ip_map", count);
	if (0 != rc) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'count' wrong, initing 'VisitorIpMap' error !");
		return -1;
	}
	for (int i = 0; i < count; i++) {
		string visitor;
		string ip;

		rc = configure.get_array2("visit_ctl_config", "visitor_ip_map", i, 0,
				visitor);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'visitor' error during the %d loop, init 'VisitorIpMap' failed !", i);
			return -1;
		}
		rc = configure.get_array2("visit_ctl_config", "visitor_ip_map", i, 1,
				ip);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'ip' error during the %d loop, init 'VisitorIpMap' failed !", i);
			return -1;
		}

		VisitorIpMap::iterator iter1 = visitorIpMap.find(visitor);
		if (iter1 != visitorIpMap.end()) {
			//合并重复的允许访问IP配置
			IpList iv = iter1->second;
			iv.push_back(ip);
			visitorIpMap.erase(visitor);
			visitorIpMap.insert(pair<string, IpList> (visitor, iv));
		} else {
			//插入允许访问IP
			IpList iv;
			iv.push_back(ip);
			visitorIpMap.insert(pair<string, IpList> (visitor, iv));
		}
	}

	//初始化“访问者服务端”对应的允许访问信息
	rc = configure.count1_array2("visit_ctl_config",
			"visitor_dstservice_allow_info_map", count);
	if (0 != rc) {
		LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'count' error, init 'VisitorDstserviceAllowInfoMap' failed !");
		return -1;
	}
    for (int i = 0; i < count; i++) {
		string visitor;
		string dstService;
		long amount;
		long freq;
		string strApiList;
		rc = configure.get_array2("visit_ctl_config",
				"visitor_dstservice_allow_info_map", i, 0, visitor);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'visitor' error during the %d loop, init 'VisitorDstserviceAllowInfoMap' failed !", i);
			return -1;
		}
		rc = configure.get_array2("visit_ctl_config",
				"visitor_dstservice_allow_info_map", i, 1, dstService);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'dstservice' error during the %d loop, init 'VisitorDstserviceAllowInfoMap' failed !", i);
			return -1;
		}
		rc = configure.get_array2("visit_ctl_config",
				"visitor_dstservice_allow_info_map", i, 2, amount);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'amount' error during the %d loop, init 'VisitorDstserviceAllowInfoMap' failed !", i);
			return -1;
		}
		rc = configure.get_array2("visit_ctl_config",
				"visitor_dstservice_allow_info_map", i, 3, freq);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'freq' error during the %d loop, init 'VisitorDstserviceAllowInfoMap' failed !", i);
			return -1;
		}
		rc = configure.get_array2("visit_ctl_config",
				"visitor_dstservice_allow_info_map", i, 4, strApiList);
		if (0 != rc) {
			LOG_ERROR("[AccessControler::loadVisitorConfigure] Read 'strApiList' error during the %d loop, init 'VisitorDstserviceAllowInfoMap' failed !", i);
			return -1;
		}

		//获取允许访问apiName
		string visitorDstservice = visitor + "&" + dstService;
		vector<string> apiList = splitStrByDelim(strApiList, ",");
		AllowInfo allowInfo;
		allowInfo.amount = amount;
		allowInfo.freq = freq;
		allowInfo.apiList = apiList;
		map<string, AllowInfo>::iterator iter2 =
				visitorDstserviceAllowInfoMap.find(visitorDstservice);
		if (iter2 != visitorDstserviceAllowInfoMap.end()) 
		{
			LOG_ERROR("[AccessControler::loadVisitorConfigure] '%s'--'%s' configured multiple times, please modify the configure file !", 
				visitor.c_str(), dstService.c_str());
			return -1;
		} 
		else 
		{
			//插入允许访问信息
			visitorDstserviceAllowInfoMap.insert(pair<string, AllowInfo> (visitorDstservice, allowInfo));
		}
	}

	return 0;
}


//检查是否禁止访问：0-允许访问，其他-禁止访问
int AccessControler::check(const char* visitor, const char* dstService,
		const char* apiName, const char* visitorIp, string &reason) {

	if (NULL == visitor || NULL == dstService || NULL == apiName || NULL
			== visitorIp) {
		LOG_WARN("[AccessControler::check] Parameter can not be null !");
		return -1;
	}

	//检查访问者是否在visitorIpMap中配置过;
	VisitorIpMap::iterator iter1 = visitorIpMap.find(visitor);
	if (iter1 == visitorIpMap.end()) {
		//没有该访问者的配置项
		LOG_WARN("[AccessControler::check] No configuration of '%s' !", visitor);
		reason = "No configuration of " + string(visitor);
		return -1;
	} else {
		bool findFlag = false;
		for (int i = 0; i < (int)iter1->second.size(); i++) {
			if (visitorIp == iter1->second[i]) {
				findFlag = true;
			}
		}
		if (!findFlag) {
			//没有该访问者对应IP的配置项
			LOG_WARN("[AccessControler::check] No 'IP' configuration of '%s' !", visitor);
			reason = "No 'IP' configuration of " + string(visitor);
			return -1;
		}
	}

	//检查是否有“访问者服务端”对应的允许信息
	string vst = visitor;
	string dst = dstService;
	VisitorDstserviceAllowInfoMap::iterator iter2 =
			visitorDstserviceAllowInfoMap.find(vst + "&" + dst);
	if (iter2 == visitorDstserviceAllowInfoMap.end()) {
		LOG_WARN("[AccessControler::check] No allowInfo configuration of '%s'--'%s' !", visitor, dstService);
		reason = "No allowInfo configuration of " + vst + "--" + dst + "!";
		return -1;
	}

	//检查访问apiName是否配置过
	AllowInfo allowInfo = iter2->second;
	vector<string> apiList = allowInfo.apiList;
	bool findFlag = false;
	for (int i = 0; i < (int)apiList.size(); i++) {
		if (apiName==apiList[i] || "*"==apiList[i]) {
			findFlag = true;
		}
	}
	if (!findFlag) {
		LOG_WARN("[AccessControler::check] No apiName:'%s' configuration of '%s'--'%s' !", apiName, visitor, dstService);
		reason = "No apiName:" + string(apiName) + " configuration of " + vst + "--" + dst + "!";
		return -1;
	}

	//第一次访问，初始化访问历史信息
	time_t now = time(0);
	VisitorHistoryMap::iterator iter3 = visitorHistoryMap.find(vst + "&" + dst);
	if (iter3 == visitorHistoryMap.end()) {
		VisitorHistory visitorHistory;
		visitorHistory.banFlag = false;
		visitorHistory.curAmount = 0L;
		visitorHistory.firstVisitTime = now;
		visitorHistory.lastVisitTime = now;
		visitorHistory.freq = 0;
		visitorHistory.timeArray = TimeArray();
		visitorHistoryMap.insert(pair<string, VisitorHistory> (vst + "&" + dst, visitorHistory));
		iter3 = visitorHistoryMap.find(vst + "&" + dst);
	}

	if(iter3->second.timeArray.size() != AccessControler::period4Freq)
	{
		iter3->second.timeArray.resize(AccessControler::period4Freq,0);
		iter3->second.freq = 0 ;
	}
	//检查是否被禁止访问
	if (iter3->second.banFlag == true) {
		LOG_WARN("[AccessControler::check] '%s' is banned to visit '%s' !", visitor, dstService);
		reason = vst + " is banned to visit " + dst + " !";
		return -1;
	}

	//检查访问次数是否超出配置文件限制
	//如果现在距第一次访问时间超过最大时间，则将第一次访问时间置为now，当前访问次数清0
	if((now - iter3->second.firstVisitTime) > AccessControler::period4Amount){
		iter3->second.firstVisitTime = now;
		iter3->second.curAmount = 0L;
	}

	
	if (iter3->second.curAmount> allowInfo.amount) {
		LOG_WARN("[AccessControler::check] '%s' visits '%s' overtake the max amount !", visitor, dstService);
		reason = vst + " visits " + dst + " overtake the max amount !";
		return -1;
	}
	++iter3->second.curAmount;
	//检查访频率是否超出配置文件限制


	time_t span = now - iter3->second.lastVisitTime;
	if(span != 0 )
		iter3->second.lastVisitTime = now;
	if(span >=AccessControler::period4Freq)
	{
		iter3->second.freq = 0;
		iter3->second.timeArray.assign(AccessControler::period4Freq,0);
	}
	else if(span > 0 && span < AccessControler::period4Freq)
	{
		//first step:delete the last span elements ;

		for(int i= 0 ; i < span;++i)
		{ 
			iter3->second.freq -=iter3->second.timeArray.back();
			iter3->second.timeArray.pop_back();
//			iter3->second.freq -= iter3->second.timeArray[AccessControler::period4Freq - 1- i];
		}

		//second step:move the first AccessControler::period4Freq - span elements backforward

		/*for(int  i =  AccessControler::period4Freq - span - 1; i>=0; --i)
		{
			iter3->second.timeArray[i + span] = iter3->second.timeArray[i];
		}*/
		
		//thirdstep: assign the first span elements to zero.
		for(int i = 0 ;i < span ; ++i)
		{
			//iter3->second.timeArray[i] = 0;
			iter3->second.timeArray.push_front(0);
		}

	}
	
	if (iter3->second.freq > allowInfo.freq) {
		LOG_WARN("[AccessControler::check] '%s' visits '%s' overtake the max freqence !", visitor, dstService);
		reason = vst + " visits " + dst + " overtake the max frequence !";
		return -1;  
	}
	
	++iter3->second.freq;
	++iter3->second.timeArray.front();
	
	return 0;
}
//手动禁止访问：0-禁止成功，其他-失败
int AccessControler::ban(const char* visitor, const char* dstService) 
{

	if (NULL == visitor || NULL == dstService) 
	{
		LOG_WARN("[AccessControler::ban] Parameter can not be null !");
		return -1;
	}

	//检查访问者是否在visitorIpMap中配置过;
	VisitorIpMap::iterator iter1 = visitorIpMap.find(visitor);
	if (iter1 == visitorIpMap.end()) {
		//没有该访问者的配置项
		LOG_WARN("[AccessControler::ban] No configuration of '%s' !", visitor);
		return -1;
	} 
//		else {
//			bool findFlag = false;
//			for (int i = 0; i < (int)iter1->second.size(); i++) {
//				if (visitorIp == iter1->second[i]) {
//					findFlag = true;
//				}
//			}
//			if (!findFlag) {
//				//没有该访问者对应IP的配置项
//				LOG_WARN("[AccessControler::ban] No 'IP' configuration of '%s' !", visitor);
//				return -1;
//			}
//		}

	//检查是否有“访问者服务端”对应的允许访问信息
	string vst = visitor;
	string dst = dstService;
	VisitorDstserviceAllowInfoMap::iterator iter2 =
			visitorDstserviceAllowInfoMap.find(vst + "&" + dst);
	if (iter2 == visitorDstserviceAllowInfoMap.end()) {
		LOG_WARN("[AccessControler::ban] No allowInfo configuration of '%s'--'%s' !", visitor, dstService);
		return -1;
	}

//		//检查访问apiName是否配置过
//		AllowInfo allowInfo = iter2->second;
//		vector<string> apiList = allowInfo.apiList;
//		bool findFlag = false;
//		for (int i = 0; i < (int)apiList.size(); i++) {
//			if (apiName==apiList[i]  || "*"==apiList[i]) {
//				findFlag = true;
//			}
//		}
//		if (!findFlag) {
//			LOG_WARN("[AccessControler::ban] No apiName:'%s' configuration of '%s'--'%s' !", apiName, visitor, dstService);
//			return -1;
//		}

	//设置禁止访问
	VisitorHistoryMap::iterator iter3 = visitorHistoryMap.find(vst + "&" + dst);
	if (iter3 != visitorHistoryMap.end()) {
		iter3->second.banFlag = true;
	}
	else{
		VisitorHistory visitorHistory;
		visitorHistory.banFlag = true;
		visitorHistory.curAmount = 0L;
		visitorHistory.firstVisitTime = 0L;
		visitorHistory.timeArray = TimeArray();
		visitorHistoryMap.insert(pair<string, VisitorHistory> (vst + "&" + dst, visitorHistory));
	}

	return 0;
}

//手动解禁：0-解禁成功，其他-失败
int AccessControler::unban(const char* visitor, const char* dstService) 
{

	if (NULL == visitor || NULL == dstService) {
		LOG_WARN("[AccessControler::unban] Parameter can not be null !");
		return -1;
	}

	//检查访问者是否在visitorIpMap中配置过;
	VisitorIpMap::iterator iter1 = visitorIpMap.find(visitor);
	if (iter1 == visitorIpMap.end()) {
		//没有该访问者的配置项
		LOG_WARN("[AccessControler::unban] No configuration of '%s' !", visitor);
		return -1;
	} 
//		else {
//			bool findFlag = false;
//			for (int i = 0; i < (int)iter1->second.size(); i++) {
//				if (visitorIp == iter1->second[i]) {
//					findFlag = true;
//				}
//			}
//			if (!findFlag) {
//				//没有该访问者对应IP的配置项
//				LOG_WARN("[AccessControler::unban] No 'IP' configuration of '%s' !", visitor);
//				return -1;
//			}
//		}

	//检查是否有“访问者服务端”对应的允许访问信息
	string vst = visitor;
	string dst = dstService;
	VisitorDstserviceAllowInfoMap::iterator iter2 =
			visitorDstserviceAllowInfoMap.find(vst + "&" + dst);
	if (iter2 == visitorDstserviceAllowInfoMap.end()) {
		LOG_WARN("[AccessControler::unban] No allowInfo configuration of '%s'--'%s' !", visitor, dstService);
		return -1;
	}

//		//检查访问apiName是否配置过
//		AllowInfo allowInfo = iter2->second;
//		vector<string> apiList = allowInfo.apiList;
//		bool findFlag = false;
//		for (int i = 0; i < (int)apiList.size(); i++) {
//			if (apiName==apiList[i]  || "*"==apiList[i]) {
//				findFlag = true;
//			}
//		}
//		if (!findFlag) {
//			LOG_WARN("[AccessControler::unban] No apiName:'%s' configuration of '%s'--'%s' !", apiName, visitor, dstService);
//			return -1;
//		}

	//设置允许访问
	VisitorHistoryMap::iterator iter3 = visitorHistoryMap.find(vst + "&" + dst);
	if (iter3 != visitorHistoryMap.end()) {
		iter3->second.banFlag = false;
	}

	return 0;
}

//将用户动态信息dump到缓存中
int AccessControler::dumpVisitorHistory(char *buf, int len){
	if(NULL == buf){
		LOG_INFO("Visitor dump information can not be NULL !");
	}

	stringstream logstr;

	VisitorHistoryMap::iterator iter;
	for(iter=visitorHistoryMap.begin(); iter!=visitorHistoryMap.end(); iter++){
		logstr<<"visitorDstservice:"<<iter->first<<"|";
		logstr<<"banFlag:"<<iter->second.banFlag<<"|";
		logstr<<"curAmount:"<<iter->second.curAmount<<"|";
		logstr<<"firstVisitTime:"<<iter->second.firstVisitTime<<endl;
	}

	strncpy(buf, logstr.str().c_str(), len);
	return 0;
}

//讲访问表dump到缓存中
int AccessControler::dumpAccessTable(char* buf, int len){
    stringstream logstr("");

    logstr << "VisitorIpMap：" << endl;
    logstr << "------------------------------------------" << endl;
    VisitorIpMap::iterator iter = visitorIpMap.begin();
    while (iter != visitorIpMap.end()) {
        string visitor = iter->first;
        vector<string> ipList = iter->second;
        logstr << "visitor:  " << visitor << endl;
        logstr << "ipList:  ";
        for (int i = 0; i < (int)ipList.size(); i++) {
            logstr << ipList[i] << "  ";
        }
        logstr << endl;
        iter++;
        logstr << "------------------------------------------" << endl;
    }

    logstr << "VisitorDstserviceAllowInfoMap：" << endl;
    logstr << "------------------------------------------" << endl;
    VisitorDstserviceAllowInfoMap::iterator iter2 =
        visitorDstserviceAllowInfoMap.begin();
    while (iter2 != visitorDstserviceAllowInfoMap.end()) {
        string vstDst = iter2->first;
        long amount = iter2->second.amount;
        long freq = iter2->second.freq;
        vector<string> apiList = iter2->second.apiList;
        logstr << "vstDst:  " << vstDst << endl;
        logstr << "amount:  " << amount << endl;
        logstr << "freq:  " << freq << endl;
        logstr << "apiList:  ";
        for (int i = 0; i < (int)apiList.size(); i++) {
            logstr << apiList[i] << "  ";
        }
        logstr << endl;
        iter2++;
        logstr << "------------------------------------------" << endl;
    }

    strncpy(buf, logstr.str().c_str(), len);
    return 0;
}

int AccessControler::dumpAccessTableToFile(fstream& out)
{
    VisitorIpMap::iterator iter = visitorIpMap.begin();
    while (iter != visitorIpMap.end()) {
        string visitor = iter->first;
        vector<string> ipList = iter->second;
        for (int i = 0; i < (int)ipList.size(); i++) {
            out << visitor << " ";
            out << ipList[i] << std::endl;
        }
        iter++;
    }

    out << std::endl;

    VisitorDstserviceAllowInfoMap::iterator iter2 =
        visitorDstserviceAllowInfoMap.begin();
    while (iter2 != visitorDstserviceAllowInfoMap.end()) {
        string vstDst = iter2->first;
        std::replace(vstDst.begin(), vstDst.end(), '&', ' ');
        long amount = iter2->second.amount;
        long freq = iter2->second.freq;
        vector<string> apiList = iter2->second.apiList;
        out << vstDst << " ";
        out << amount << " ";
        out << freq << " ";
        out << std::endl;

        iter2++;
    }

    out << std::endl;

    iter2 = visitorDstserviceAllowInfoMap.begin();
    while (iter2 != visitorDstserviceAllowInfoMap.end()) {
        string vstDst = iter2->first;
        std::replace(vstDst.begin(), vstDst.end(), '&', ' ');
        vector<string> apiList = iter2->second.apiList;
        for (int i = 0; i < (int)apiList.size(); i++) {
            out << vstDst << " ";
            out << apiList[i] << " " << std::endl;
        }

        iter2++;
    }

    return 0;
}

//从缓存load用户动态信息
int AccessControler::loadVisitorHistory(char *buf){
	if(NULL == buf || buf == ""){
		LOG_INFO("Visitor dump information can not be NULL or empty!");
		return -1;
	}

	visitorHistoryMap.clear();

	stringstream logstr(buf);
	string tmpStr;
	while(std::getline(logstr, tmpStr)){
		vector<string> tmpVec = splitStrByDelim(tmpStr, "|");

		//获取“访问者服务端”
		string visitorDstservice = splitStrByDelim(tmpVec[0], ":")[1];
		VisitorHistory visitorHistory;
		//获取禁止标志
		visitorHistory.banFlag = (bool)atoi(splitStrByDelim(tmpVec[1], ":")[1].c_str());
		//获取最大访问次数
		visitorHistory.curAmount = atol(splitStrByDelim(tmpVec[2], ":")[1].c_str());
		//获取第一次访问时间
		visitorHistory.firstVisitTime = atol(splitStrByDelim(tmpVec[3], ":")[1].c_str());
		//访问时间清单清空，暂时不重新load
		visitorHistory.timeArray = TimeArray();
		visitorHistory.freq = 0;
		visitorHistory.lastVisitTime = time(0);
		visitorHistoryMap.insert(pair<string, VisitorHistory>(visitorDstservice, visitorHistory));

	}
	
	LOG_INFO("Visitor load dumped information Success.......!");
	showVisitorHistoryMap();

	return 0;

}

//打印VisitorIpMap
void AccessControler::showVisitorIpMap() 
{
	stringstream logstr("");
	logstr << "VisitorIpMap：" << endl;
	logstr << "------------------------------------------" << endl;
	VisitorIpMap::iterator iter = visitorIpMap.begin();
	while (iter != visitorIpMap.end()) {
		string visitor = iter->first;
		vector<string> ipList = iter->second;
		logstr << "visitor:  " << visitor << endl;
		logstr << "ipList:  ";
		for (int i = 0; i < (int)ipList.size(); i++) {
			logstr << ipList[i] << "  ";
		}
		logstr << endl;
		iter++;
		logstr << "------------------------------------------" << endl;
	}
	LOG_INFO("[AccessControler::showVisitorIpMap] Info----:%s",
			logstr.str().c_str());
}

//打印VisitorDstserviceAllowInfoMap
void AccessControler::showVisitorDstserviceAllowInfoMap() 
{
	stringstream logstr("");

	logstr << "VisitorDstserviceAllowInfoMap：" << endl;
	logstr << "------------------------------------------" << endl;
	VisitorDstserviceAllowInfoMap::iterator iter =
			visitorDstserviceAllowInfoMap.begin();
	while (iter != visitorDstserviceAllowInfoMap.end()) {
		string vstDst = iter->first;
		long amount = iter->second.amount;
		long freq = iter->second.freq;
		vector<string> apiList = iter->second.apiList;
		logstr << "vstDst:  " << vstDst << endl;
		logstr << "amount:  " << amount << endl;
		logstr << "freq:  " << freq << endl;
		logstr << "apiList:  ";
		for (int i = 0; i < (int)apiList.size(); i++) {
			cout << apiList[i] << "  ";
		}
		logstr << endl;
		iter++;
		logstr << "------------------------------------------" << endl;
	}

	LOG_INFO("[AccessControler::showVisitorDstserviceAllowInfoMap] Info----:%s",
		logstr.str().c_str());
}

//打印VisitorHistoryMap
void AccessControler::showVisitorHistoryMap() 
{
	stringstream logstr("");
	logstr << "VisitorHistoryMap：" << endl;
	logstr << "------------------------------------------" << endl;
	VisitorHistoryMap::iterator iter = visitorHistoryMap.begin();
	while (iter != visitorHistoryMap.end()) {
		string vstDst = iter->first;
		bool banFlag = iter->second.banFlag;
		long curAmount = iter->second.curAmount;
		long firstVisitTime = iter->second.firstVisitTime;
		logstr << "vstDst:  " << vstDst << endl;
		logstr << "banFlag:  " << banFlag << endl;
		logstr << "curAmount:  " << curAmount << endl;
		logstr << "firstVisitTime:  " << firstVisitTime << endl;
		iter++;
		logstr << "------------------------------------------" << endl;
	}
	
	LOG_INFO("[AccessControler::showVisitorHistoryMap] Info----:%s",
		logstr.str().c_str());
}

