#include "php_git2.h"
#include "php_git2_priv.h"
#include "giterr.h"

/* {{{ proto resource giterr_last()
 */
PHP_FUNCTION(giterr_last)
{
	const git_error *result = {0};
	zval *array;

	result = giterr_last();
	php_git2_error_to_array(result, &array);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto void giterr_clear()
 */
PHP_FUNCTION(giterr_clear)
{
	giterr_clear();
}
/* }}} */

/* {{{ proto array giterr_detach()
 */
PHP_FUNCTION(giterr_detach)
{
	int result = 0;
	git_error cpy;
	zval *array;

	result = giterr_detach(&cpy);
	php_git2_error_to_array(&cpy, &array);
	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto void giterr_set_str(long $error_class, string $string)
 */
PHP_FUNCTION(giterr_set_str)
{
	long error_class = 0;
	char *string = NULL;
	int string_len = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ls", &error_class, &string, &string_len) == FAILURE) {
		return;
	}
	
	giterr_set_str(error_class, string);
}
/* }}} */

/* {{{ proto void giterr_set_oom()
 */
PHP_FUNCTION(giterr_set_oom)
{
	giterr_set_oom();
}
/* }}} */

