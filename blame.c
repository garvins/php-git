#include "php_git2.h"
#include "php_git2_priv.h"
#include "blame.h"

/* {{{ proto long git_blame_get_hunk_count(resource $blame)
 */
PHP_FUNCTION(git_blame_get_hunk_count)
{
	uint32_t result = 0;
	zval *blame = NULL;
	php_git2_t *_blame = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blame) == FAILURE) {
		return;
	}
	
	if ((_blame = (php_git2_t *) zend_fetch_resource(Z_RES_P(blame), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
	    RETURN_FALSE;
	}

	result = git_blame_get_hunk_count(PHP_GIT2_V(_blame, blame));
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_blame_get_hunk_byindex(resource $blame, long $index)
 */
PHP_FUNCTION(git_blame_get_hunk_byindex)
{
	const git_blame_hunk *result = NULL;
	zval *blame = NULL, *array = NULL;
	php_git2_t *_blame = NULL;
	long index = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &blame, &index) == FAILURE) {
		return;
	}
	
	if ((_blame = (php_git2_t *) zend_fetch_resource(Z_RES_P(blame), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
	    RETURN_FALSE;
	}

	result = git_blame_get_hunk_byindex(PHP_GIT2_V(_blame, blame), index);
	if (result == NULL) {
		RETURN_FALSE;
	}
	php_git2_git_blame_hunk_to_array(result, &array TSRMLS_CC);
	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto resource git_blame_get_hunk_byline(resource $blame, long $lineno)
 */
PHP_FUNCTION(git_blame_get_hunk_byline)
{
	const git_blame_hunk *result = NULL;
	zval *blame = NULL, *array = NULL;
	php_git2_t *_blame = NULL;
	long lineno = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &blame, &lineno) == FAILURE) {
		return;
	}
	
	if ((_blame = (php_git2_t *) zend_fetch_resource(Z_RES_P(blame), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
	    RETURN_FALSE;
	}

	result = git_blame_get_hunk_byline(PHP_GIT2_V(_blame, blame), lineno);
	if (result == NULL) {
		RETURN_FALSE;
	}
	php_git2_git_blame_hunk_to_array(result, &array TSRMLS_CC);
	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto resource git_blame_file(resource $repo, string $path,  $options)
 */
PHP_FUNCTION(git_blame_file)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_blame *out = NULL;
	zval *repo = NULL, *options = NULL;
	git_blame_options opts = GIT_BLAME_OPTIONS_INIT;
	char *path = NULL;
	int path_len = 0, error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsa", &repo, &path, &path_len, &options) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
	    RETURN_FALSE;
	}

	php_git2_array_to_git_blame_options(&opts, options TSRMLS_CC);
	error = git_blame_file(&out, PHP_GIT2_V(_repo, repository), path, &opts);
	if (php_git2_check_error(error, "git_blame_file" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_BLAME, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_blame_buffer(resource $reference, string $buffer)
 */
PHP_FUNCTION(git_blame_buffer)
{
	php_git2_t *result = NULL, *_reference = NULL;
	git_blame *out = NULL;
	zval *reference = NULL;
	char *buffer = NULL;
	int buffer_len = 0, error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &reference, &buffer, &buffer_len) == FAILURE) {
		return;
	}
	
	if ((_reference = (php_git2_t *) zend_fetch_resource(Z_RES_P(reference), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
	    RETURN_FALSE;
	}

	error = git_blame_buffer(&out, PHP_GIT2_V(_reference, blame), buffer, buffer_len);
	if (php_git2_check_error(error, "git_blame_buffer" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_BLAME, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_blame_free(resource $blame)
 */
PHP_FUNCTION(git_blame_free)
{
	zval *blame = NULL;
	php_git2_t *_blame = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blame) == FAILURE) {
		return;
	}
	
	if ((_blame = (php_git2_t *) zend_fetch_resource(Z_RES_P(blame), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
	    RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_blame)) {
		git_blame_free(PHP_GIT2_V(_blame, blame));
		GIT2_SHOULD_FREE(_blame) = 0;
	};
	zval_ptr_dtor(&blame);
}
/* }}} */

/* {{{ proto void git_blame_options_new()
 */
PHP_FUNCTION(git_blame_options_new)
{
	zval *result;
	git_blame_options options = GIT_BLAME_OPTIONS_INIT;

	php_git2_git_blame_options_to_array(&options, &result TSRMLS_CC);
	RETURN_ZVAL(result, 0, 1);
}
/* }}} */
