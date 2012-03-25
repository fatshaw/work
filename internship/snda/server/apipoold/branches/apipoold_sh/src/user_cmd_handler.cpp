#include "user_cmd_handler.h"
#include "apipoold_configure.h"

#include "apipoold_handler.h"

#include "AccessControler.h"
#include "phpservice_handler.h"

#include "dispatcher.h"
#include "options.h"

#include "log.h"
#include <sstream>
#include <fstream>

#include <string>
#include <vector>

using namespace std;

extern char * configureFile;

void UserCmdBan(const char* visitor, const char* dstService,
			char *result_buf, int result_buf_len, 
			int& result_len);
void UserCmdUnBan(const char* visitor, const char* dstService,
			char *result_buf, int result_buf_len, 
			int& result_len);
void UserCmdShowReqlist(const char *req_name,char *result_buf, int result_buf_len, int& result_len);
void UserCmdShowAccessInfo(char *result_buf, int result_buf_len, int& result_len);
void UserCmdShowAccessTable(char *result_buf, int result_buf_len, int& result_len);
// 重新加载配置
void UserCmdReload(string sub_item, char *result_buf, int result_buf_len, int& result_len);

void UserCmdHandler(const char *cmd, int cmd_len, char *result_buf, int result_buf_len, int& result_len)
{
	LOG_INFO("UserCmdHandler, cmd:[%s]", string(cmd, cmd_len).c_str());

	vector<string> cmd_vector;

	// 命令拆分
	int i = 0;
	while (i < cmd_len)
	{
		while (i < cmd_len)
		{
			if ((cmd[i] != ' ') && (cmd[i] != '\t'))
				break;
			i++;
		}

		if (i == cmd_len)
			break;

		int begin = i;

		while (i < cmd_len)
		{
			if ((cmd[i] == ' ') || (cmd[i] == '\t'))
				break;
			i++;
		}

		int end = i;

		cmd_vector.push_back(string(cmd + begin, end - begin));
	}

	if (cmd_vector.size() < 1)
	{
		strncpy(result_buf, "Command is empty\r\nType 'help' for usage.", result_buf_len) [result_buf_len] = 0;
		result_len = strlen(result_buf);
		return;
	}

	if (cmd_vector[0] == "help")
	{
		if (1 == cmd_vector.size())
		{
			strncpy(result_buf, "Available commands:\r\n\treload\r\n\tshowaccessinfo\r\n\tshowaccesstable\r\n\tshowreqlist\r\n\tshutdown\r\n\tban\r\n\tunban", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}

		if (cmd_vector[1] == "reload")
		{
			strncpy(result_buf, "reload: reload conf file, and make valid", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		else if (cmd_vector[1] == "shutdown")
		{
			strncpy(result_buf, "shutdown: shutdown apipoold", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		else
		{
			strncpy(result_buf, "Available commands:\r\n\treload\r\n\tshowuser\r\n\tshowmem\r\n\tshutdown", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
	}	
	else if (cmd_vector[0] == "showreqlist")
	{
		if (2 > cmd_vector.size())
		{
			strncpy(result_buf, "usage: showreqlist {fcgiagent|apipoold}", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		
		string reqname = cmd_vector[1];
		
		UserCmdShowReqlist(reqname.c_str(), result_buf, result_buf_len, result_len); 
		return;
	}
	else if (cmd_vector[0] == "ban")
	{
		if (3 > cmd_vector.size())
		{
			strncpy(result_buf, "usage: ban visitor dstService", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		
		string visitor = cmd_vector[1];
		string dstService = cmd_vector[2];

		UserCmdBan(visitor.c_str(), dstService.c_str(),
				result_buf, result_buf_len, result_len); 
		return;
	}
	else if (cmd_vector[0] == "unban")
	{
		if (3 > cmd_vector.size())
		{
			strncpy(result_buf, "usage: unban visitor dstService", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		
		string visitor = cmd_vector[1];
		string dstService = cmd_vector[2];

		UserCmdUnBan(visitor.c_str(), dstService.c_str(), 
				result_buf, result_buf_len, result_len); 
		return;
	}
	else if (cmd_vector[0] == "showaccessinfo")
	{
		if (1 != cmd_vector.size())
		{
			strncpy(result_buf, "parameter error", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		
		UserCmdShowAccessInfo(result_buf, result_buf_len, result_len); 
		return;
	}
    else if (cmd_vector[0] == "showaccesstable")
    {
        if (1 != cmd_vector.size())
        {
            strncpy(result_buf, "parameter error", result_buf_len) [result_buf_len] = 0;
            result_len = strlen(result_buf);
            return;
        }

        UserCmdShowAccessTable(result_buf, result_buf_len, result_len); 
        return;
    }
	else if (cmd_vector[0] == "shutdown")
	{
		if (1 != cmd_vector.size())
		{
			strncpy(result_buf, "parameter error", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}

		LOG_INFO("UserCmdHandler: shutdown apipoold");
		Dispatcher::stop();

		strncpy(result_buf, "apipoold shutdown", result_buf_len) [result_buf_len] = 0;
		return;
	}
	else if(cmd_vector[0] == "reload")
	{
		if (2 != cmd_vector.size())
		{
			strncpy(result_buf, "usage: reload system | phpservice | apipool | accessconf", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		
		if(cmd_vector[1] != "system" && cmd_vector[1] != "phpservice" 
				&& cmd_vector[1] != "apipool"
				&& cmd_vector[1] != "accessconf"){
			strncpy(result_buf, "usage: reload system | phpservice | apipool | accessconf", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		
		int rc = LoadApipooldConfigure(configureFile,true,cmd_vector[1]);
		if(rc !=0 )
		{
			sprintf(result_buf,"Reload Configure %s error \n",cmd_vector[1].c_str());
			result_len = strlen(result_buf);
			LOG_FATAL("Reload Configure file error\n");
		}
		else
		{
			sprintf(result_buf,"Reload Configure %s Success \n",cmd_vector[1].c_str());
			result_len = strlen(result_buf);
			LOG_DEBUG("Reload Configure %s Success \n",cmd_vector[1].c_str());
		}	
		return;
	}
	else
	{
		snprintf(result_buf, result_buf_len, "Unknown command: '%s'\r\nType 'help' for usage.", cmd_vector[0].c_str());
		result_len = strlen(result_buf);
		return;
	}
}

void UserCmdShowAccessInfo(char *result_buf, int result_buf_len, int& result_len)
{
	char buf[8192] = {0};	
	AccessControler::dumpVisitorHistory(buf, sizeof(buf)-1);

	
	LOG_INFO("UserCmdShowAccessInfo: %s", buf);
	
	strncpy(result_buf, buf, result_buf_len) [result_buf_len] = 0;
	result_len = strlen(result_buf);
	return;
	
}

void UserCmdShowAccessTable(char *result_buf, int result_buf_len, int& result_len)
{
    char buf[8192] = {0};	
    AccessControler::dumpAccessTable(buf, sizeof(buf)-1);


    LOG_INFO("UserCmdShowAccessTable: %s", buf);

    strncpy(result_buf, buf, result_buf_len) [result_buf_len] = 0;
    result_len = strlen(result_buf);
    return;

}

void UserCmdShowReqlist(const char *req_name,char *result_buf, int result_buf_len, int& result_len)
{	
	char buf[4096];
	if(strcmp(req_name, "fcgiagent") == 0)
	{
		FcgiAgentHandler::getFcgiAgentHandlerInformation(buf, sizeof(buf)-1);	
	}
	else if(strcmp(req_name, "apipoold") == 0)
	{
		ApipooldHandler::getApipooldHandlerInformation(buf, sizeof(buf)-1);
	}
	else
	{
		sprintf(buf, "Unknown handler type :%s\n", req_name);
	}

	
	LOG_INFO("UserCmdShowReqlist: %s", buf);
	
	strncpy(result_buf, buf, result_buf_len) [result_buf_len] = 0;
	result_len = strlen(result_buf);
	return;
}

void UserCmdBan(const char* visitor, const char* dstService,
			char *result_buf, int result_buf_len, 
			int& result_len)
{
	char buf[4096];
	int ret = AccessControler::ban(visitor,dstService);
	
	sprintf(buf, "AccessControler::ban: ret:%d\n", ret);
	
	strncpy(result_buf, buf, result_buf_len) [result_buf_len] = 0;
	result_len = strlen(result_buf);
	return;	
}

void UserCmdUnBan(const char* visitor, const char* dstService,
			char *result_buf, int result_buf_len, 
			int& result_len)
{
	char buf[4096];
	int ret = AccessControler::unban(visitor,dstService);
	
	sprintf(buf, "AccessControler::unban: ret:%d\n", ret);
	
	strncpy(result_buf, buf, result_buf_len) [result_buf_len] = 0;
	result_len = strlen(result_buf);
	return;	
}





