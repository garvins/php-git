#include "php_git2.h"
#include "php_git2_priv.h"
#include "treebuilder.h"

/* {{{ proto resource git_treebuilder_create(resource $source)
 */
PHP_FUNCTION(git_treebuilder_create)
{
	php_git2_t *result = NULL, *_source = NULL;
	git_treebuilder *out = NULL;
	zval *source = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &source) == FAILURE) {
		return;
	}

	if ((_source = (php_git2_t *) zend_fetch_resource(Z_RES_P(source), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_treebuilder_create(&out, PHP_GIT2_V(_source, tree));

	if (php_git2_check_error(error, "git_treebuilder_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TREEBUILDER, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_treebuilder_clear(resource $bld)
 */
PHP_FUNCTION(git_treebuilder_clear)
{
	zval *bld = NULL;
	php_git2_t *_bld = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &bld) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_treebuilder_clear(PHP_GIT2_V(_bld, treebuilder));
}
/* }}} */

/* {{{ proto long git_treebuilder_entrycount(resource $bld)
 */
PHP_FUNCTION(git_treebuilder_entrycount)
{
	unsigned int result;
	zval *bld = NULL;
	php_git2_t *_bld = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &bld) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_treebuilder_entrycount(PHP_GIT2_V(_bld, treebuilder));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_treebuilder_free(resource $bld)
 */
PHP_FUNCTION(git_treebuilder_free)
{
	zval *bld = NULL;
	php_git2_t *_bld = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &bld) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_bld)) {
		git_treebuilder_free(PHP_GIT2_V(_bld, treebuilder));
		GIT2_SHOULD_FREE(_bld) = 0;
	}

	zval_ptr_dtor(bld);
}
/* }}} */

/* {{{ proto resource git_treebuilder_get(resource $bld, string $filename)
 */
PHP_FUNCTION(git_treebuilder_get)
{
	const git_tree_entry *result = NULL;
	zval *bld = NULL;
	php_git2_t *_bld = NULL, *__result = NULL;
	char *filename = NULL;
	size_t filename_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &bld, &filename, &filename_len) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_treebuilder_get(PHP_GIT2_V(_bld, treebuilder), filename);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_TREE_ENTRY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_treebuilder_insert(resource $bld, string $filename, string $id, long $filemode)
 */
PHP_FUNCTION(git_treebuilder_insert)
{
	php_git2_t *result = NULL, *_bld = NULL;
	const git_tree_entry *out = NULL;
	zval *bld = NULL;
	char *filename = NULL, *id = NULL;
	size_t filename_len, id_len;
	git_oid __id = {0};
	zend_long filemode;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rssl", &bld, &filename, &filename_len, &id, &id_len, &filemode) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_treebuilder_insert(&out, PHP_GIT2_V(_bld, treebuilder), filename, &__id, filemode);

	if (php_git2_check_error(error, "git_treebuilder_insert" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TREE_ENTRY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_treebuilder_remove(resource $bld, string $filename)
 */
PHP_FUNCTION(git_treebuilder_remove)
{
	int result;
	zval *bld = NULL;
	php_git2_t *_bld = NULL;
	char *filename = NULL;
	size_t filename_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &bld, &filename, &filename_len) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_treebuilder_remove(PHP_GIT2_V(_bld, treebuilder), filename);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_treebuilder_filter(resource $bld, Callable $filter, void $payload)
 */
PHP_FUNCTION(git_treebuilder_filter)
{
	zval *bld = NULL, *payload = NULL;
	php_git2_t *_bld = NULL;
	zend_fcall_info filter_fci = empty_fcall_info;
	zend_fcall_info_cache filter_fcc = empty_fcall_info_cache;
	php_git2_cb_t *filter_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &bld, &filter_fci, &filter_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&filter_cb, &filter_fci, &filter_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_treebuilder_filter(PHP_GIT2_V(_bld, treebuilder), php_git2_git_treebuilder_filter_cb, filter_cb);
	php_git2_cb_free(filter_cb);
}
/* }}} */

/* {{{ proto string git_treebuilder_write(resource $repo, resource $bld)
 */
PHP_FUNCTION(git_treebuilder_write)
{
	git_oid id;
	char __id[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *bld = NULL;
	php_git2_t *_repo = NULL, *_bld = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &repo, &bld) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_bld = (php_git2_t *) zend_fetch_resource(Z_RES_P(bld), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_treebuilder_write(&id, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_bld, treebuilder));

	if (php_git2_check_error(error, "git_treebuilder_write" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__id, &id);

	RETURN_STRING(__id);
}
/* }}} */

