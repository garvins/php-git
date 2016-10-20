#include "php_git2.h"
#include "php_git2_priv.h"
#include "refspec.h"

/* {{{ proto string git_refspec_src(resource $refspec)
 */
PHP_FUNCTION(git_refspec_src)
{
	const char *result = NULL;
	zval *refspec = NULL;
	php_git2_t *_refspec = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &refspec) == FAILURE) {
		return;
	}

	if ((_refspec = (php_git2_t *) zend_fetch_resource(Z_RES_P(refspec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_src(PHP_GIT2_V(_refspec, refspec));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_refspec_dst(resource $refspec)
 */
PHP_FUNCTION(git_refspec_dst)
{
	const char *result = NULL;
	zval *refspec = NULL;
	php_git2_t *_refspec = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &refspec) == FAILURE) {
		return;
	}

	if ((_refspec = (php_git2_t *) zend_fetch_resource(Z_RES_P(refspec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_dst(PHP_GIT2_V(_refspec, refspec));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_refspec_string(resource $refspec)
 */
PHP_FUNCTION(git_refspec_string)
{
	const char *result = NULL;
	zval *refspec = NULL;
	php_git2_t *_refspec = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &refspec) == FAILURE) {
		return;
	}

	if ((_refspec = (php_git2_t *) zend_fetch_resource(Z_RES_P(refspec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_string(PHP_GIT2_V(_refspec, refspec));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_refspec_force(resource $refspec)
 */
PHP_FUNCTION(git_refspec_force)
{
	int result;
	zval *refspec = NULL;
	php_git2_t *_refspec = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &refspec) == FAILURE) {
		return;
	}

	if ((_refspec = (php_git2_t *) zend_fetch_resource(Z_RES_P(refspec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_force(PHP_GIT2_V(_refspec, refspec));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_refspec_direction(resource $spec)
 */
PHP_FUNCTION(git_refspec_direction)
{
	git_direction result;
	zval *spec = NULL;
	php_git2_t *_spec = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &spec) == FAILURE) {
		return;
	}

	if ((_spec = (php_git2_t *) zend_fetch_resource(Z_RES_P(spec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_direction(PHP_GIT2_V(_spec, refspec));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_refspec_src_matches(resource $refspec, string $refname)
 */
PHP_FUNCTION(git_refspec_src_matches)
{
	int result;
	zval *refspec = NULL;
	php_git2_t *_refspec = NULL;
	char *refname = NULL;
	size_t refname_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &refspec, &refname, &refname_len) == FAILURE) {
		return;
	}

	if ((_refspec = (php_git2_t *) zend_fetch_resource(Z_RES_P(refspec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_src_matches(PHP_GIT2_V(_refspec, refspec), refname);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_refspec_dst_matches(resource $refspec, string $refname)
 */
PHP_FUNCTION(git_refspec_dst_matches)
{
	int result;
	zval *refspec = NULL;
	php_git2_t *_refspec = NULL;
	char *refname = NULL;
	size_t refname_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &refspec, &refname, &refname_len) == FAILURE) {
		return;
	}

	if ((_refspec = (php_git2_t *) zend_fetch_resource(Z_RES_P(refspec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_refspec_dst_matches(PHP_GIT2_V(_refspec, refspec), refname);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_refspec_transform(resource $spec, string $name)
 */
PHP_FUNCTION(git_refspec_transform)
{
	char out[GIT2_BUFFER_SIZE] = {0}, *name = NULL;
	size_t outlen = GIT2_BUFFER_SIZE, name_len;
	zval *spec = NULL;
	php_git2_t *_spec = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &spec, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_spec = (php_git2_t *) zend_fetch_resource(Z_RES_P(spec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_refspec_transform(out, outlen, PHP_GIT2_V(_spec, refspec), name);

	if (php_git2_check_error(error, "git_refspec_transform" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto string git_refspec_rtransform(resource $spec, string $name)
 */
PHP_FUNCTION(git_refspec_rtransform)
{
	char out[GIT2_BUFFER_SIZE] = {0}, *name = NULL;
	size_t outlen = GIT2_BUFFER_SIZE, name_len;
	zval *spec = NULL;
	php_git2_t *_spec = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &spec, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_spec = (php_git2_t *) zend_fetch_resource(Z_RES_P(spec), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_refspec_rtransform(out, outlen, PHP_GIT2_V(_spec, refspec), name);

	if (php_git2_check_error(error, "git_refspec_rtransform" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

