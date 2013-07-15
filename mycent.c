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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_mycent.h"
#include "md5lib.h"
#include "randstr.h"

/* If you declare any globals in php_mycent.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(mycent)


/* True global resources - no need for thread safety here */
static int le_mycent;

/* {{{ mycent_functions[]
 *
 * Every user visible function must have an entry in mycent_functions[].
 */
zend_function_entry mycent_functions[] = {
	PHP_FE(mycent_encrypt, NULL)
	PHP_FE(mycent_decrypt, NULL)
	PHP_FE(mycent_auth, NULL)
	PHP_FE(mycent_md5, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in mycent_functions[] */
};
/* }}} */

/* {{{ mycent_module_entry
 */
zend_module_entry mycent_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"mycent",
	mycent_functions,
	PHP_MINIT(mycent),
	PHP_MSHUTDOWN(mycent),
	PHP_RINIT(mycent),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(mycent),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(mycent),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYCENT
ZEND_GET_MODULE(mycent)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("mycent.auth",      "0", PHP_INI_ALL, OnUpdateBool, auth, zend_mycent_globals, mycent_globals)
PHP_INI_END()

/* }}} */

/* {{{ php_mycent_init_globals
 */
/* Uncomment this function if you have INI entries */
static void php_mycent_init_globals(zend_mycent_globals *mycent_globals)
{
	mycent_globals->auth = 0;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(mycent)
{
	ZEND_INIT_MODULE_GLOBALS(mycent, php_mycent_init_globals,
NULL);
	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(mycent)
{
	/* uncomment this line if you have INI entries */
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(mycent)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(mycent)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mycent)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mycent support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini */
	DISPLAY_INI_ENTRIES();
	
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_mycent_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(mycent_encrypt)
{
	char *arg = NULL;
	long key;
	int arg_len, len, i;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &arg, &arg_len, &key) == FAILURE) {
		return;
	}
	
	for(i=0;i<arg_len;i++) {
		arg[i] = (SYMM_SWAP(arg[i])^key);
	}
	len = spprintf(&strg, 0, "%.78s", arg);
	RETURN_STRINGL(strg, len, 0);
}
PHP_FUNCTION(mycent_decrypt)
{
        char *arg = NULL;
        long key;
        int arg_len, len, i;
        char *strg;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &arg, &arg_len, &key) == FAILURE) {
                return;
        }

        for(i=0;i<arg_len;i++) {
                arg[i] ^= key;
                arg[i] = SYMM_SWAP(arg[i]);
        }

		len = spprintf(&strg, 0, "%.78s", arg);
        RETURN_STRINGL(strg, len, 0);
}
PHP_FUNCTION(mycent_auth)
{
	RETURN_BOOL(MYCENT_G(auth));
}
PHP_FUNCTION(mycent_md5)
{
        char *arg = NULL;
        char *key = NULL;
        char *strg, *mstr;
        int arg_len,key_len,len;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &arg, &arg_len, &key, &key_len) == FAILURE) {
                return;
        }
        mstr = hmac_md5(arg,key);
        len = spprintf(&strg, 0, "%.78s", mstr);
		RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
