/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

extern "C" {

/* $Id: header 226204 2007-01-01 19:32:10Z iliaa $ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "state_session.h"
#include "php.h"

#include <sys/stat.h>
#include <sys/types.h>

#if HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#ifdef PHP_WIN32
#include "win32/readdir.h"
#endif
#include <time.h>

#include <fcntl.h>
#include <errno.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext/session/php_session.h"
#include "ext/standard/flock_compat.h"
#include "php_open_temporary_file.h"
}

#include "session.h"

#define FILE_PREFIX "stat_"

/* If you declare any globals in php_statserver.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(statserver)
*/

/* True global resources - no need for thread safety here */
static int le_statserver;

/* {{{ statserver_functions[]
 *
 * Every user visible function must have an entry in statserver_functions[].
 */
zend_function_entry statserver_functions[] = {
	PHP_FE(confirm_statserver_compiled,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in statserver_functions[] */
};
/* }}} */

/* {{{ statserver_module_entry
 */
zend_module_entry statserver_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"statserver",
	statserver_functions,
	PHP_MINIT(statserver),
	PHP_MSHUTDOWN(statserver),
	PHP_RINIT(statserver),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(statserver),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(statserver),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STATSERVER
BEGIN_EXTERN_C()
ZEND_GET_MODULE(statserver)
END_EXTERN_C()
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("statserver.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_statserver_globals, statserver_globals)
    STD_PHP_INI_ENTRY("statserver.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_statserver_globals, statserver_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_statserver_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_statserver_init_globals(zend_statserver_globals *statserver_globals)
{
	statserver_globals->global_value = 0;
	statserver_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(statserver)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(statserver)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(statserver)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(statserver)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "statserver support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_statserver_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_statserver_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "statserver", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */



struct ps_files
{
	StateSession*session;
	FILE * fd;
};

ps_module ps_mod_statserver = {
	PS_MOD(statserver)
};

/* {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(statserver)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	php_session_register_module(&ps_mod_statserver);

	return SUCCESS;
}

#define PS_FILES_DATA ps_files *data = (ps_files*)PS_GET_MOD_DATA()

PS_OPEN_FUNC(statserver)
{
	ps_files *data;			//用于各个函数处理传递的数据结构
	const char *p;
	char address[1024];
	int port;
	system("mkdir -p /tmp/session/");
	FILE * fd = fopen("/tmp/session/log","w+");
	if (*save_path == '\0') {
		sprintf(address,"%s","127.0.0.1");
		port  = 8110;
	}
	else
	{
			p = strchr(save_path, ':');
			if(p) 
			{
				strncpy(address,save_path,p-save_path);
				p++;
				port = atoi(p);
			}
			else
			{
				sprintf(address,"%s","127.0.0.1");
				port  = 8110;
				return SUCCESS;
			}
	}

	StateSession * session = new StateSession;
	session->setServer(address,port);
	if(session->createSocket() == RET_FAIL){
		fputs("create socket error:",fd);
		fputs(session->getLastError().c_str(),fd);
		delete session;
		fclose(fd);
		return FAILURE;
	}
	data = (ps_files*)ecalloc(1, sizeof(*data));
	data->session= session;
	data->fd  = fd;
	PS_SET_MOD_DATA(data);
	fputs("\nsession open end\n",fd);
	return SUCCESS;
}

PS_CLOSE_FUNC(statserver)
{
	//*mod_data 是指向传递的数据结构
	PS_FILES_DATA;
	
	StateSession * session  = data->session;
	if(session)delete session;
	FILE * fd= data->fd;
	fclose(fd);
	efree(data);
	*mod_data = NULL;
	return SUCCESS;
}

PS_READ_FUNC(statserver)
{
	/*
		*mod_data 是指向传递的数据结构
		const char *key 得到的SESSION ID.
		char **val 输出的SESSION数据,字符串.
		int *vallen 输出的SESSION数据的长度。
	*/
	PS_FILES_DATA;
	string result;
	FILE * fd= data->fd;
	StateSession * session = data->session;
	if(session == NULL)return FAILURE;
	StateSession::Args args;
	args.insert(make_pair("session_id",key));
	if(session->request("GetSession",args,result) == RET_FAIL)
	{
		fputs("request error,",fd);
		fputs(session->getLastError().c_str(),fd);
		return FAILURE;
	}
	*vallen = result.length();
	if (*vallen == 0) {
		*val = STR_EMPTY_ALLOC();
		return SUCCESS;
	}
	*val = (char*)emalloc(*vallen);
	strncpy(*val,result.c_str(),*vallen);	
	
	return SUCCESS;
}

PS_WRITE_FUNC(statserver)
{
	/*
	*mod_data 是指向传递的数据结构
	const char *key 得到的SESSION ID.
	char *val 输出的SESSION数据,字符串.
	int *vallen 输出的SESSION数据的长度。
	*/
	PS_FILES_DATA;
	FILE * fd = data->fd;
	StateSession * session = data->session;
	if(session == NULL)
	{
		return FAILURE;
	}
	if(*val == 0)
	{	
		return SUCCESS;
	}
	char expired_time[10];
	sprintf(expired_time,"%d",INI_INT("session.gc_maxlifetime"));
	StateSession::Args args;
	string result;
	args.insert(make_pair("session_id",key));
	args.insert(make_pair("session_data",val));
	args.insert(make_pair("expired_time",expired_time));
	if(session->request("SetSession",args,result) == -1)
	{
		return FAILURE;
	}
	if(result == "false")return FAILURE;
	return SUCCESS;
}

PS_DESTROY_FUNC(statserver)
{
	/*
	*mod_data 是指向传递的数据结构
	const char *key 得到的SESSION ID.
	char **val 输出的SESSION数据,字符串.
	int *vallen 输出的SESSION数据的长度。
	*/
	PS_FILES_DATA;
	StateSession * session = data->session;
	if(session == NULL)return FAILURE;
	StateSession::Args args;
	FILE * fd = data->fd;
	string result;
	args.insert(make_pair("session_id",key));
	args.insert(make_pair("session_data","null"));
	args.insert(make_pair("expired_time","1"));
	session->request("SetSession",args,result);
	if(result == "false")return FAILURE;
	return SUCCESS;
}

PS_GC_FUNC(statserver)
{
	return SUCCESS;
}
