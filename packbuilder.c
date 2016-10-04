#include "php_git2.h"
#include "php_git2_priv.h"
#include "packbuilder.h"

/* {{{ proto resource git_packbuilder_new(resource $repo)
 */
PHP_FUNCTION(git_packbuilder_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_packbuilder *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}
	
	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_packbuilder_new(&out, PHP_GIT2_V(_repo, repository));
	if (php_git2_check_error(error, "git_packbuilder_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_PACKBUILDER, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_packbuilder_set_threads(resource $pb, long $n)
 */
PHP_FUNCTION(git_packbuilder_set_threads)
{
	unsigned int result = 0;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	long n = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &pb, &n) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_packbuilder_set_threads(PHP_GIT2_V(_pb, packbuilder), n);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_insert(resource $pb, string $id, string $name)
 */
PHP_FUNCTION(git_packbuilder_insert)
{
	int result = 0, id_len = 0, name_len = 0;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	char *id = NULL, *name = NULL;
	git_oid __id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &pb, &id, &id_len, &name, &name_len) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}
	result = git_packbuilder_insert(PHP_GIT2_V(_pb, packbuilder), &__id, name);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_insert_tree(resource $pb, string $id)
 */
PHP_FUNCTION(git_packbuilder_insert_tree)
{
	int result = 0, id_len = 0;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	char *id = NULL;
	git_oid __id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &pb, &id, &id_len) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}
	result = git_packbuilder_insert_tree(PHP_GIT2_V(_pb, packbuilder), &__id);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_insert_commit(resource $pb, string $id)
 */
PHP_FUNCTION(git_packbuilder_insert_commit)
{
	int result = 0, id_len = 0;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	char *id = NULL;
	git_oid __id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &pb, &id, &id_len) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}
	result = git_packbuilder_insert_commit(PHP_GIT2_V(_pb, packbuilder), &__id);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_write(resource $pb, string $path, long $mode,  $progress_cb,  $progress_cb_payload)
 */
PHP_FUNCTION(git_packbuilder_write)
{
	int result = 0, path_len = 0;
	zval *pb = NULL, *progress_cb_payload = NULL;
	php_git2_t *_pb = NULL;
	char *path = NULL;
	long mode = 0;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rslfz", &pb, &path, &path_len, &mode, &fci, &fcc, &progress_cb_payload) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb, &fci, &fcc, progress_cb_payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_packbuilder_write(PHP_GIT2_V(_pb, packbuilder), path, mode, php_git2_git_transfer_progress_callback, cb);
	php_git2_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_packbuilder_hash(resource $pb)
 */
PHP_FUNCTION(git_packbuilder_hash)
{
	const git_oid  *result = NULL;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &pb) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_packbuilder_hash(PHP_GIT2_V(_pb, packbuilder));
	git_oid_fmt(__result, result);
	RETURN_STRING(__result);
}
/* }}} */


/* {{{ proto long git_packbuilder_foreach(resource $pb, Callable $cb,  $payload)
 */
PHP_FUNCTION(git_packbuilder_foreach)
{
	int result = 0;
	zval *pb = NULL, *payload = NULL;
	php_git2_t *_pb = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &pb, &fci, &fcc, &payload) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb, &fci, &fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_packbuilder_foreach(PHP_GIT2_V(_pb, packbuilder), php_git2_git_packbuilder_foreach_cb, cb);
	php_git2_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_object_count(resource $pb)
 */
PHP_FUNCTION(git_packbuilder_object_count)
{
	uint32_t result = 0;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &pb) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_packbuilder_object_count(PHP_GIT2_V(_pb, packbuilder));
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_written(resource $pb)
 */
PHP_FUNCTION(git_packbuilder_written)
{
	uint32_t result = 0;
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &pb) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_packbuilder_written(PHP_GIT2_V(_pb, packbuilder));
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_packbuilder_set_callbacks(resource $pb,  $progress_cb,  $progress_cb_payload)
 */
PHP_FUNCTION(git_packbuilder_set_callbacks)
{
	int result = 0;
	zval *pb = NULL, *progress_cb_payload = NULL;
	php_git2_t *_pb = NULL;
	zend_fcall_info fci = empty_fcall_info, *_fci;
	zend_fcall_info_cache fcc = empty_fcall_info_cache, *_fcc;
	php_git2_cb_t *cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &pb, &fci, &fcc, &progress_cb_payload) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	_fci = emalloc(sizeof(zend_fcall_info));
	_fcc = emalloc(sizeof(zend_fcall_info_cache));
	memcpy(_fci, &fci, sizeof(zend_fcall_info));
	memcpy(_fcc, &fcc, sizeof(zend_fcall_info_cache));

	/* TODO(chobie): free memory when the resource removed */
	if (php_git2_cb_init(&cb, &_fci, &_fcc, progress_cb_payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_packbuilder_set_callbacks(PHP_GIT2_V(_pb, packbuilder), php_git2_git_packbuilder_progress, cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_packbuilder_free(resource $pb)
 */
PHP_FUNCTION(git_packbuilder_free)
{
	zval *pb = NULL;
	php_git2_t *_pb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &pb) == FAILURE) {
		return;
	}
	
	if ((_pb = (php_git2_t *) zend_fetch_resource(Z_RES_P(pb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_pb)) {
		git_packbuilder_free(PHP_GIT2_V(_pb, packbuilder));
		GIT2_SHOULD_FREE(_pb) = 0;
	};
	zval_ptr_dtor(&pb);
}
/* }}} */

