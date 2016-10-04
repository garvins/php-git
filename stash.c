#include "php_git2.h"
#include "php_git2_priv.h"
#include "stash.h"

/* {{{ proto string git_stash_save(resource $repo, array $stasher, string $message, long $flags)
 */
PHP_FUNCTION(git_stash_save)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *stasher = NULL;
	php_git2_t *_repo = NULL;
	git_signature *_stasher = NULL;
	char *message = NULL;
	size_t message_len;
	zend_long flags;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rasl", &repo, &stasher, &message, &message_len, &flags) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_stasher, stasher TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_stash_save(&out, PHP_GIT2_V(_repo, repository), _stasher, message, flags);

	if (php_git2_check_error(error, "git_stash_save" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);
	git_signature_free(_stasher);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto long git_stash_foreach(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_stash_foreach)
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

	result = git_stash_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_stash_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_stash_drop(resource $repo, long $index)
 */
PHP_FUNCTION(git_stash_drop)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	zend_long index;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &repo, &index) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_stash_drop(PHP_GIT2_V(_repo, repository), index);

	RETURN_LONG(result);
}
/* }}} */

