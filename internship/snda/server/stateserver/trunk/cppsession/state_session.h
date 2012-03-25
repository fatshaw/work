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

/* $Id: header 226204 2007-01-01 19:32:10Z iliaa $ */
#ifndef PHP_STATSERVER_H
#define PHP_STATSERVER_H

extern zend_module_entry statserver_module_entry;
#define phpext_statserver_ptr &statserver_module_entry

#ifdef PHP_WIN32
#define PHP_STATSERVER_API __declspec(dllexport)
#else
#define PHP_STATSERVER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(statserver);
PHP_MSHUTDOWN_FUNCTION(statserver);
PHP_RINIT_FUNCTION(statserver);
PHP_RSHUTDOWN_FUNCTION(statserver);
PHP_MINFO_FUNCTION(statserver);

PHP_FUNCTION(confirm_statserver_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(statserver)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(statserver)
*/

/* In every utility function you add that needs to use variables 
   in php_statserver_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as STATSERVER_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define STATSERVER_G(v) TSRMG(statserver_globals_id, zend_statserver_globals *, v)
#else
#define STATSERVER_G(v) (statserver_globals.v)
#endif

#endif	/* PHP_STATSERVER_H */

#include "ext/session/php_session.h"

extern ps_module ps_mod_statserver;
#define ps_memcache_ptr &ps_mod_statserver

PS_FUNCS(statserver);

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
