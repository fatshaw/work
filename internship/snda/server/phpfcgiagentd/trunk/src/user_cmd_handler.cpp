#include "user_cmd_handler.h"
#include "configure.h"
#include "common.h"
#include "dispatcher.h"
#include "log.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace utils;
extern utils::Configure configure;
extern Dispatcher* p_dispather;

extern char * configfile ;


void UserCmdHandler(const char *cmd, int cmd_len, char *result_buf, int result_buf_len, int& result_len)
{
	LOG_INFO("UserCmdHandler, cmd:[%s]", string(cmd, cmd_len).c_str());

	vector<string> cmd_vector;

	// ÃüÁî²ð·Ö
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
			strncpy(result_buf, "Available commands:\r\n\treload\r\n\tshowuser\r\n\tshowmc\r\n\tchangeoutuin\r\n\tdelmc\r\n\tshowmclist\r\n\tmcinvite\r\n\tshowreqlist\r\n\tshowconninfo\r\n\tdeluser\r\n\tshutdown", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}

		if (cmd_vector[1] == "reload")
		{
			strncpy(result_buf, "reload: reload conf file, and make valid", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		else if (cmd_vector[1] == "showuser")
		{
			strncpy(result_buf, "showuser: show user status\r\nusage: showuser uin", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		else if (cmd_vector[1] == "showmc")
		{
			strncpy(result_buf, "showmc: show information of mc\r\nusage: showmc mcid createtime", result_buf_len) [result_buf_len] = 0;
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
	else if (cmd_vector[0] == "shutdown")
	{
		if (1 != cmd_vector.size())
		{
			strncpy(result_buf, "parameter error", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}

		LOG_INFO("UserCmdHandler: shutdown apipoold");
		p_dispather->stop();

		strncpy(result_buf, "apipoold shutdown", result_buf_len) [result_buf_len] = 0;
		result_len = strlen(result_buf);
		return;
	}
	else if(cmd_vector[0] == "reload")
	{
		if(cmd_vector.size() != 2){
			strncpy(result_buf, "usage: reload Log", result_buf_len) [result_buf_len] = 0;
			result_len = strlen(result_buf);
			return;
		}
		utils::Configure tmpConfigure;
		int iTempRet = tmpConfigure.load(configfile);
		if( iTempRet != 0 )
		{
			LOG_FATAL("Read Config File Fail ret=%d!\n", iTempRet);
			return ;
		}
		LOG_INFO("Read Config File Success : path is %s\n",configfile);
		int ret = init_log(tmpConfigure, true);
		if(ret != 0)
		{
			LOG_FATAL("[Admin User Cmd]Reload Configure file error \n");
			return ;
		}	
		LOG_INFO("[Admin User Cmd ]Reload Configure file success\n");
		strncpy(result_buf,"Reload Configure Success",result_buf_len)[result_buf_len] = 0;
		result_len = strlen(result_buf);
		return ;
	}
	else
	{
		snprintf(result_buf, result_buf_len, "Unknown command: '%s'\r\nType 'help' for usage.", cmd_vector[0].c_str());
		result_len = strlen(result_buf);
		return;
	}
}





