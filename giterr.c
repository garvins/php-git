#include "php_git2.h"
#include "php_git2_priv.h"
#include "giterr.h"

/* {{{ proto array giterr_last()
 */
PHP_FUNCTION(giterr_last)
{
	const git_error *result = NULL;
	
	result = giterr_last();

	array_init(return_value);
	php_git2_git_error_to_array(result, return_value);

	RETVAL_ARR(Z_ARRVAL_P(return_value));
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
	git_error *cpy = NULL;
	int error;
	
	error = giterr_detach(cpy);

	if (php_git2_check_error(error, "giterr_detach" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	array_init(return_value);
	php_git2_git_error_to_array(cpy, return_value);

	RETVAL_ARR(Z_ARRVAL_P(return_value));
}
/* }}} */

/* {{{ proto void giterr_set_str(long $error_class, string $string)
 */
PHP_FUNCTION(giterr_set_str)
{
	zend_long error_class;
	char *string = NULL;
	size_t string_len;
	
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

