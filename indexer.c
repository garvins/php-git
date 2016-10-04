#include "php_git2.h"
#include "php_git2_priv.h"
#include "indexer.h"

/* {{{ proto resource git_indexer_new(string $path, long $mode, resource $odb, Callable $progress_cb, void $progress_cb_payload)
 */
PHP_FUNCTION(git_indexer_new)
{
	php_git2_t *result = NULL, *_odb = NULL;
	git_indexer *out = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long mode;
	zval *odb = NULL, *progress_cb_payload = NULL;
	zend_fcall_info progress_fci = empty_fcall_info;
	zend_fcall_info_cache progress_fcc = empty_fcall_info_cache;
	php_git2_cb_t *progress_cb = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"slrfz", &path, &path_len, &mode, &odb, &progress_fci, &progress_fcc, &progress_cb_payload) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&progress_cb, &progress_fci, &progress_fcc, progress_cb_payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	error = git_indexer_new(&out, path, mode, PHP_GIT2_V(_odb, odb), php_git2_git_transfer_progress_callback, progress_cb);

	if (php_git2_check_error(error, "git_indexer_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	php_git2_cb_free(progress_cb);

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_INDEXER, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_indexer_append(resource $idx, void $data, array $stats)
 */
PHP_FUNCTION(git_indexer_append)
{
	int result;
	zval *idx = NULL, *stats = NULL;
	php_git2_t *_idx = NULL;
	char *data = NULL;
	zend_long size;
	git_transfer_progress _stats = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsa", &idx, &data, &size, &stats) == FAILURE) {
		return;
	}

	php_git2_array_to_git_transfer_progress(&_stats, stats TSRMLS_CC);

	if ((_idx = (php_git2_t *) zend_fetch_resource(Z_RES_P(idx), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_indexer_append(PHP_GIT2_V(_idx, indexer), data, size, &_stats);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_indexer_commit(resource $idx, array $stats)
 */
PHP_FUNCTION(git_indexer_commit)
{
	int result;
	zval *idx = NULL, *stats = NULL;
	php_git2_t *_idx = NULL;
	git_transfer_progress _stats = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ra", &idx, &stats) == FAILURE) {
		return;
	}

	php_git2_array_to_git_transfer_progress(&_stats, stats TSRMLS_CC);

	if ((_idx = (php_git2_t *) zend_fetch_resource(Z_RES_P(idx), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_indexer_commit(PHP_GIT2_V(_idx, indexer), &_stats);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_indexer_hash(resource $idx)
 */
PHP_FUNCTION(git_indexer_hash)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *idx = NULL;
	php_git2_t *_idx = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &idx) == FAILURE) {
		return;
	}

	if ((_idx = (php_git2_t *) zend_fetch_resource(Z_RES_P(idx), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_indexer_hash(PHP_GIT2_V(_idx, indexer));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto void git_indexer_free(resource $idx)
 */
PHP_FUNCTION(git_indexer_free)
{
	zval *idx = NULL;
	php_git2_t *_idx = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &idx) == FAILURE) {
		return;
	}

	if ((_idx = (php_git2_t *) zend_fetch_resource(Z_RES_P(idx), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_idx)) {
		git_indexer_free(PHP_GIT2_V(_idx, indexer));
		GIT2_SHOULD_FREE(_idx) = 0;
	}

	zval_ptr_dtor(&idx);
}
/* }}} */

