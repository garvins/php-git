#include "php_git2.h"
#include "php_git2_priv.h"
#include "status.h"

/* {{{ proto long git_status_foreach(resource $repo, Callable $callback,  $payload)
 */
PHP_FUNCTION(git_status_foreach)
{
	int result = 0;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &fci, &fcc, &payload) == FAILURE) {
		return;
	}
	
	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb, &fci, &fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_status_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_status_cb, cb);
	php_git2_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_status_foreach_ext(resource $repo,  $opts, Callable $callback,  $payload)
 */
PHP_FUNCTION(git_status_foreach_ext)
{
	int result = 0;
	zval *repo = NULL, *opts = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb = NULL;
	git_status_options options = GIT_STATUS_OPTIONS_INIT;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rafz", &repo, &opts, &fci, &fcc, &payload) == FAILURE) {
		return;
	}
	
	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_array_to_git_status_options(&options, opts TSRMLS_CC);
	if (php_git2_cb_init(&cb, &fci, &fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_status_foreach_ext(PHP_GIT2_V(_repo, repository), &options, php_git2_git_status_cb, cb);
	php_git2_cb_free(cb);
	if (options.pathspec.count > 0) {
		php_git2_strarray_free(&options.pathspec);
	}
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_status_file(long $status_flags, resource $repo, string $path)
 */
PHP_FUNCTION(git_status_file)
{
	int result = 0, path_len = 0;
	long status_flags = 0;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"lrs", &status_flags, &repo, &path, &path_len) == FAILURE) {
		return;
	}
	
	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_status_file(&status_flags, PHP_GIT2_V(_repo, repository), path);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_status_list_new(resource $repo,  $opts)
 */
PHP_FUNCTION(git_status_list_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_status_list *out = NULL;
	zval *repo = NULL, *opts = NULL;
	git_status_options options = GIT_STATUS_OPTIONS_INIT;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ra", &repo, &opts) == FAILURE) {
		return;
	}
	
	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_array_to_git_status_options(&options, opts TSRMLS_CC);
	error = git_status_list_new(&out, PHP_GIT2_V(_repo, repository), &options);
	if (options.pathspec.count > 0) {
		php_git2_strarray_free(&options.pathspec);
	}
	if (php_git2_check_error(error, "git_status_list_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_STATUS_LIST, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_status_list_entrycount(resource $statuslist)
 */
PHP_FUNCTION(git_status_list_entrycount)
{
	size_t result = 0;
	zval *statuslist = NULL;
	php_git2_t *_statuslist = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &statuslist) == FAILURE) {
		return;
	}
	
	if ((_statuslist = (php_git2_t *) zend_fetch_resource(Z_RES_P(statuslist), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_status_list_entrycount(PHP_GIT2_V(_statuslist, status_list));
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_status_byindex(resource $statuslist, long $idx)
 */
PHP_FUNCTION(git_status_byindex)
{
	const git_status_entry  *result = NULL;
	zval *statuslist = NULL, *out;
	php_git2_t *_statuslist = NULL;
	long idx = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &statuslist, &idx) == FAILURE) {
		return;
	}
	
	if ((_statuslist = (php_git2_t *) zend_fetch_resource(Z_RES_P(statuslist), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_status_byindex(PHP_GIT2_V(_statuslist, status_list), idx);
	if (result == NULL) {
		RETURN_FALSE;
	}
	php_git2_git_status_entry_to_array(result, &out TSRMLS_CC);
	RETURN_ZVAL(out, 0, 1);
}
/* }}} */

/* {{{ proto void git_status_list_free(resource $statuslist)
 */
PHP_FUNCTION(git_status_list_free)
{
	zval *statuslist = NULL;
	php_git2_t *_statuslist = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &statuslist) == FAILURE) {
		return;
	}
	
	if ((_statuslist = (php_git2_t *) zend_fetch_resource(Z_RES_P(statuslist), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_statuslist)) {
		git_status_list_free(PHP_GIT2_V(_statuslist, status_list));
		GIT2_SHOULD_FREE(_statuslist) = 0;
	};
	zval_ptr_dtor(&statuslist);
}
/* }}} */

/* {{{ proto long git_status_should_ignore(long $ignored, resource $repo, string $path)
 */
PHP_FUNCTION(git_status_should_ignore)
{
	int result = 0, path_len = 0;
	long ignored = 0;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"lrs", &ignored, &repo, &path, &path_len) == FAILURE) {
		return;
	}
	
	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_status_should_ignore(ignored, PHP_GIT2_V(_repo, repository), path);
	RETURN_LONG(result);
}
/* }}} */

PHP_FUNCTION(git_status_options_new)
{
	git_status_options options = GIT_STATUS_OPTIONS_INIT;
	zval *result;

	php_git2_git_status_options_to_array(&options, &result TSRMLS_CC);
	RETURN_ZVAL(result, 0, 1);
}
