#include "php_git2.h"
#include "php_git2_priv.h"
#include "blob.h"

/* {{{ proto string git_blob_create_frombuffer(resource $repo, void $buffer)
 */
PHP_FUNCTION(git_blob_create_frombuffer)
{
	git_oid oid;
	char __oid[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *buffer = NULL;
	zend_long len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &buffer, &len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_blob_create_frombuffer(&oid, PHP_GIT2_V(_repo, repository), buffer, len);

	if (php_git2_check_error(error, "git_blob_create_frombuffer" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__oid, &oid);

	RETURN_STRING(__oid);
}
/* }}} */

/* {{{ proto string git_blob_create_fromchunks(resource $repo, string $hintpath, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_blob_create_fromchunks)
{
	git_oid id;
	char __id[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	char *hintpath = NULL;
	size_t hintpath_len;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsfz", &repo, &hintpath, &hintpath_len, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	error = git_blob_create_fromchunks(&id, PHP_GIT2_V(_repo, repository), hintpath, php_git2_git_blob_chunk_cb, callback_cb);

	if (php_git2_check_error(error, "git_blob_create_fromchunks" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__id, &id);
	php_git2_cb_free(callback_cb);

	RETURN_STRING(__id);
}
/* }}} */

/* {{{ proto string git_blob_create_fromdisk(resource $repo, string $path)
 */
PHP_FUNCTION(git_blob_create_fromdisk)
{
	git_oid id;
	char __id[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL;
	size_t path_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_blob_create_fromdisk(&id, PHP_GIT2_V(_repo, repository), path);

	if (php_git2_check_error(error, "git_blob_create_fromdisk" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__id, &id);

	RETURN_STRING(__id);
}
/* }}} */

/* {{{ proto string git_blob_create_fromworkdir(resource $repo, string $relative_path)
 */
PHP_FUNCTION(git_blob_create_fromworkdir)
{
	git_oid id;
	char __id[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *relative_path = NULL;
	size_t relative_path_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &relative_path, &relative_path_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_blob_create_fromworkdir(&id, PHP_GIT2_V(_repo, repository), relative_path);

	if (php_git2_check_error(error, "git_blob_create_fromworkdir" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__id, &id);

	RETURN_STRING(__id);
}
/* }}} */

/* {{{ proto string git_blob_filtered_content(resource $blob, string $as_path, long $check_for_binary_data)
 */
PHP_FUNCTION(git_blob_filtered_content)
{
	git_buf out = {0};
	zval *blob = NULL;
	php_git2_t *_blob = NULL;
	char *as_path = NULL;
	size_t as_path_len;
	zend_long check_for_binary_data;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &blob, &as_path, &as_path_len, &check_for_binary_data) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_blob_filtered_content(&out, PHP_GIT2_V(_blob, blob), as_path, check_for_binary_data);

	if (php_git2_check_error(error, "git_blob_filtered_content" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(git_buf_cstr(&out), git_buf_len(&out));
	git_buf_free(&out);
}
/* }}} */

/* {{{ proto void git_blob_free(resource $blob)
 */
PHP_FUNCTION(git_blob_free)
{
	zval *blob = NULL;
	php_git2_t *_blob = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blob) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_blob)) {
		git_blob_free(PHP_GIT2_V(_blob, blob));
		GIT2_SHOULD_FREE(_blob) = 0;
	}

	zval_ptr_dtor(&blob);
}
/* }}} */

/* {{{ proto string git_blob_id(resource $blob)
 */
PHP_FUNCTION(git_blob_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *blob = NULL;
	php_git2_t *_blob = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blob) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_blob_id(PHP_GIT2_V(_blob, blob));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_blob_is_binary(resource $blob)
 */
PHP_FUNCTION(git_blob_is_binary)
{
	int result;
	zval *blob = NULL;
	php_git2_t *_blob = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blob) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_blob_is_binary(PHP_GIT2_V(_blob, blob));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto resource git_blob_lookup(resource $repo, string $id)
 */
PHP_FUNCTION(git_blob_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_blob *blob = NULL;
	zval *repo = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &id, &id_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_blob_lookup(&blob, PHP_GIT2_V(_repo, repository), &__id);

	if (php_git2_check_error(error, "git_blob_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_BLOB, blob, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_blob_lookup_prefix(resource $repo, string $id, long $len)
 */
PHP_FUNCTION(git_blob_lookup_prefix)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_blob *blob = NULL;
	zval *repo = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	zend_long len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &repo, &id, &id_len, &len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_blob_lookup_prefix(&blob, PHP_GIT2_V(_repo, repository), &__id, len);

	if (php_git2_check_error(error, "git_blob_lookup_prefix" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_BLOB, blob, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_blob_owner(resource $blob)
 */
PHP_FUNCTION(git_blob_owner)
{
	git_repository *result = NULL;
	zval *blob = NULL;
	php_git2_t *_blob = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blob) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_blob_owner(PHP_GIT2_V(_blob, blob));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto string git_blob_rawcontent(resource $blob)
 */
PHP_FUNCTION(git_blob_rawcontent)
{
	const char *buffer = NULL;
	zval *blob = NULL;
	php_git2_t *_blob = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blob) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	buffer = git_blob_rawcontent(PHP_GIT2_V(_blob, blob));

	if (buffer == NULL){
		RETURN_FALSE;
	}

	size = git_blob_rawsize(PHP_GIT2_V(blob, blob));
	RETURN_STRINGL(buffer, size);
}
/* }}} */

/* {{{ proto long git_blob_rawsize(resource $blob)
 */
PHP_FUNCTION(git_blob_rawsize)
{
	git_off_t result;
	zval *blob = NULL;
	php_git2_t *_blob = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &blob) == FAILURE) {
		return;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_blob_rawsize(PHP_GIT2_V(_blob, blob));

	RETURN_LONG(result);
}
/* }}} */

