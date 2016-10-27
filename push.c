#include "php_git2.h"
#include "php_git2_priv.h"
#include "push.h"

/* {{{ proto resource git_push_new(resource $remote)
 */
PHP_FUNCTION(git_push_new)
{
	php_git2_t *result = NULL, *_remote = NULL;
	git_push *out = NULL;
	zval *remote = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_push_new(&out, PHP_GIT2_V(_remote, remote));

	if (php_git2_check_error(error, "git_push_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_PUSH, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_push_set_options(resource $push, array $opts)
 */
PHP_FUNCTION(git_push_set_options)
{
	int result, should_free = 0;
	zval *push = NULL, *opts = NULL;
	php_git2_t *_push = NULL;
	git_push_options _opts = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r|a", &push, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_push_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_push_set_options(PHP_GIT2_V(_push, push), &_opts);

	if (should_free) {
		php_git2_git_push_options_free(&_opts TSRMLS_CC);
	}

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_push_set_callbacks(resource $push, Callable $pack_progress_cb, void $pack_progress_cb_payload, Callable $transfer_progress_cb, void $transfer_progress_cb_payload)
 */
PHP_FUNCTION(git_push_set_callbacks)
{
	int result;
	zval *push = NULL, *pack_progress_cb_payload = NULL, *transfer_progress_cb_payload = NULL;
	php_git2_t *_push = NULL;
	zend_fcall_info pack_fci = empty_fcall_info, transfer_fci = empty_fcall_info;
	zend_fcall_info_cache pack_fcc = empty_fcall_info_cache, transfer_fcc = empty_fcall_info_cache;
	php_git2_cb_t *pack_cb = NULL, *transfer_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfzfz", &push, &pack_fci, &pack_fcc, &pack_progress_cb_payload, &transfer_fci, &transfer_fcc, &transfer_progress_cb_payload) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&pack_cb, &pack_fci, &pack_fcc, pack_progress_cb_payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&transfer_cb, &transfer_fci, &transfer_fcc, transfer_progress_cb_payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_push_set_callbacks(PHP_GIT2_V(_push, push), php_git2_git_packbuilder_progress, pack_cb, php_git2_git_push_transfer_progress, transfer_cb);
	php_git2_cb_free(pack_cb);
	php_git2_cb_free(transfer_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_push_add_refspec(resource $push, string $refspec)
 */
PHP_FUNCTION(git_push_add_refspec)
{
	int result;
	zval *push = NULL;
	php_git2_t *_push = NULL;
	char *refspec = NULL;
	size_t refspec_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &push, &refspec, &refspec_len) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_push_add_refspec(PHP_GIT2_V(_push, push), refspec);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_push_update_tips(resource $push)
 */
PHP_FUNCTION(git_push_update_tips)
{
	int result;
	zval *push = NULL;
	php_git2_t *_push = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &push) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_push_update_tips(PHP_GIT2_V(_push, push));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_push_finish(resource $push)
 */
PHP_FUNCTION(git_push_finish)
{
	int result;
	zval *push = NULL;
	php_git2_t *_push = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &push) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_push_finish(PHP_GIT2_V(_push, push));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_push_unpack_ok(resource $push)
 */
PHP_FUNCTION(git_push_unpack_ok)
{
	int result;
	zval *push = NULL;
	php_git2_t *_push = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &push) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_push_unpack_ok(PHP_GIT2_V(_push, push));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_push_status_foreach(resource $push, Callable $cb, void $data)
 */
PHP_FUNCTION(git_push_status_foreach)
{
	int result;
	zval *push = NULL, *data = NULL;
	php_git2_t *_push = NULL;
	zend_fcall_info cb_fci = empty_fcall_info;
	zend_fcall_info_cache cb_fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &push, &cb_fci, &cb_fcc, &data) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb_cb, &cb_fci, &cb_fcc, data TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_push_status_foreach(PHP_GIT2_V(_push, push), php_git2_git_push_status_foreach_cb, cb_cb);
	php_git2_cb_free(cb_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_push_free(resource $push)
 */
PHP_FUNCTION(git_push_free)
{
	zval *push = NULL;
	php_git2_t *_push = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &push) == FAILURE) {
		return;
	}

	if ((_push = (php_git2_t *) zend_fetch_resource(Z_RES_P(push), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_push) */) {
		git_push_free(PHP_GIT2_V(_push, push));
		GIT2_SHOULD_FREE(_push) = 0;
	}

	//zval_ptr_dtor(push);
}
/* }}} */

