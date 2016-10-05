#include "php_git2.h"
#include "php_git2_priv.h"
#include "status.h"

/* {{{ proto long git_status_foreach(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_status_foreach)
{
	int result;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_status_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_status_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_status_foreach_ext(resource $repo, array $opts, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_status_foreach_ext)
{
	int result, should_free = 0;
	zval *repo = NULL, *opts = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	git_status_options *_opts = NULL;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r|afz", &repo, &opts, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_status_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_status_foreach_ext(PHP_GIT2_V(_repo, repository), _opts, php_git2_git_status_cb, callback_cb);

	if (should_free) {
		php_git2_git_status_options_free(_opts TSRMLS_CC);
	}
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_status_file(resource $repo, string $path)
 */
PHP_FUNCTION(git_status_file)
{
	unsigned int status_flags = 0;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL;
	size_t path_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_status_file(&status_flags, PHP_GIT2_V(_repo, repository), path);

	if (php_git2_check_error(error, "git_status_file" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(status_flags);
}
/* }}} */

/* {{{ proto resource git_status_list_new(resource $repo, array $opts)
 */
PHP_FUNCTION(git_status_list_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_status_list *out = NULL;
	zval *repo = NULL, *opts = NULL;
	git_status_options *_opts = NULL;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r|a", &repo, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_status_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_status_list_new(&out, PHP_GIT2_V(_repo, repository), _opts);

	if (php_git2_check_error(error, "git_status_list_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_status_options_free(_opts TSRMLS_CC);
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
	size_t result;
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

/* {{{ proto array git_status_byindex(resource $statuslist, long $idx)
 */
PHP_FUNCTION(git_status_byindex)
{
	const git_status_entry *result = NULL;
	zval *statuslist = NULL, *array = NULL;
	php_git2_t *_statuslist = NULL;
	zend_long idx;
	
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

	php_git2_git_status_entry_to_array(result, array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
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
	}

	zval_ptr_dtor(&statuslist);
}
/* }}} */

/* {{{ proto long git_status_should_ignore(resource $repo, string $path)
 */
PHP_FUNCTION(git_status_should_ignore)
{
	int ignored = 0, error;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL;
	size_t path_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_status_should_ignore(&ignored, PHP_GIT2_V(_repo, repository), path);

	if (php_git2_check_error(error, "git_status_should_ignore" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(ignored);
}
/* }}} */

/* {{{ proto array git_status_options_new()
 */
PHP_FUNCTION(git_status_options_new)
{
	zval *result;
	git_status_options options = GIT_STATUS_OPTIONS_INIT;

	php_git2_git_status_options_to_array(&options, result TSRMLS_CC);
	RETURN_ZVAL(result, 0, 1);
}
/* }}} */

