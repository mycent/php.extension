/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
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

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifndef PHP_MYCENT_H
#define PHP_MYCENT_H

extern zend_module_entry mycent_module_entry;
#define phpext_mycent_ptr &mycent_module_entry

#ifdef PHP_WIN32
#define PHP_MYCENT_API __declspec(dllexport)
#else
#define PHP_MYCENT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(mycent);
PHP_MSHUTDOWN_FUNCTION(mycent);
PHP_RINIT_FUNCTION(mycent);
PHP_RSHUTDOWN_FUNCTION(mycent);
PHP_MINFO_FUNCTION(mycent);

PHP_FUNCTION(mycent_encrypt);
PHP_FUNCTION(mycent_decrypt);
PHP_FUNCTION(mycent_auth);
PHP_FUNCTION(mycent_md5);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     */

ZEND_BEGIN_MODULE_GLOBALS(mycent)
	zend_bool  auth;
ZEND_END_MODULE_GLOBALS(mycent)


/* In every utility function you add that needs to use variables 
   in php_mycent_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as MYCENT_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define MYCENT_G(v) TSRMG(mycent_globals_id, zend_mycent_globals *, v)
#else
#define MYCENT_G(v) (mycent_globals.v)
#endif

#endif	/* PHP_MYCENT_H */

#define SYMM_SWAP(ch) (							\
	((ch & 1) << 7)                |			\
	((ch & (0x2)) << 5)           |			\
	((ch & (0x4)) << 3)          |			\
	((ch & (0x8)) << 1)         |			\
	((ch & (0x10)) >> 1)        |			\
	((ch & (0x20)) >> 3)       |			\
	((ch & (0x40)) >> 5)      |			\
	((ch & (0x80)) >> 7))

/* define your own key value here */
#define KEY  0x35

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
