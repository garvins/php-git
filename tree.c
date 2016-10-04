#include "php_git2.h"
#include "php_git2_priv.h"
#include "tree.h"

/* {{{ proto resource git_tree_entry_byindex(resource $tree, long $idx)
 */
PHP_FUNCTION(git_tree_entry_byindex)
{
	const git_tree_entry *result = NULL;
	zval *tree = NULL;
	php_git2_t *_tree = NULL, *__result = NULL;
	zend_long idx;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &tree, &idx) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_byindex(PHP_GIT2_V(_tree, tree), idx);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_TREE_ENTRY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_tree_entry_byoid(resource $tree, string $oid)
 */
PHP_FUNCTION(git_tree_entry_byoid)
{
	const git_tree_entry *result = NULL;
	zval *tree = NULL;
	php_git2_t *_tree = NULL, *__result = NULL;
	char *oid = NULL;
	size_t oid_len;
	git_oid __oid = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &tree, &oid, &oid_len) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	result = git_tree_entry_byoid(PHP_GIT2_V(_tree, tree), &__oid);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_TREE_ENTRY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_tree_entry_byname(resource $tree, string $filename)
 */
PHP_FUNCTION(git_tree_entry_byname)
{
	const git_tree_entry *result = NULL;
	zval *tree = NULL;
	php_git2_t *_tree = NULL, *__result = NULL;
	char *filename = NULL;
	size_t filename_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &tree, &filename, &filename_len) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_byname(PHP_GIT2_V(_tree, tree), filename);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_TREE_ENTRY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_tree_entry_bypath(resource $root, string $path)
 */
PHP_FUNCTION(git_tree_entry_bypath)
{
	php_git2_t *result = NULL, *_root = NULL;
	git_tree_entry *out = NULL;
	zval *root = NULL;
	char *path = NULL;
	size_t path_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &root, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_root = (php_git2_t *) zend_fetch_resource(Z_RES_P(root), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tree_entry_bypath(&out, PHP_GIT2_V(_root, tree), path);

	if (php_git2_check_error(error, "git_tree_entry_bypath" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TREE_ENTRY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_tree_entry_id(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_id(PHP_GIT2_V(_entry, tree_entry));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_tree_entry_type(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_type)
{
	git_otype result;
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_type(PHP_GIT2_V(_entry, tree_entry));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_tree_entry_name(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_name)
{
	const char *result = NULL;
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_name(PHP_GIT2_V(_entry, tree_entry));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_tree_entrycount(resource $tree)
 */
PHP_FUNCTION(git_tree_entrycount)
{
	size_t result;
	zval *tree = NULL;
	php_git2_t *_tree = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tree) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entrycount(PHP_GIT2_V(_tree, tree));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_tree_entry_filemode(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_filemode)
{
	git_filemode_t result;
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_filemode(PHP_GIT2_V(_entry, tree_entry));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_tree_entry_filemode_raw(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_filemode_raw)
{
	git_filemode_t result;
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_filemode_raw(PHP_GIT2_V(_entry, tree_entry));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_tree_entry_cmp(resource $e1, resource $e2)
 */
PHP_FUNCTION(git_tree_entry_cmp)
{
	int result;
	zval *e1 = NULL, *e2 = NULL;
	php_git2_t *_e1 = NULL, *_e2 = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &e1, &e2) == FAILURE) {
		return;
	}

	if ((_e1 = (php_git2_t *) zend_fetch_resource(Z_RES_P(e1), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_e2 = (php_git2_t *) zend_fetch_resource(Z_RES_P(e2), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_cmp(PHP_GIT2_V(_e1, tree_entry), PHP_GIT2_V(_e2, tree_entry));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_tree_free(resource $tree)
 */
PHP_FUNCTION(git_tree_free)
{
	zval *tree = NULL;
	php_git2_t *_tree = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tree) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_tree)) {
		git_tree_free(PHP_GIT2_V(_tree, tree));
		GIT2_SHOULD_FREE(_tree) = 0;
	);

	zval_ptr_dtor(&tree);
}
/* }}} */

/* {{{ proto void git_tree_entry_free(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_free)
{
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_entry)) {
		git_tree_entry_free(PHP_GIT2_V(_entry, tree_entry));
		GIT2_SHOULD_FREE(_entry) = 0;
	);

	zval_ptr_dtor(&entry);
}
/* }}} */

/* {{{ proto resource git_tree_entry_dup(resource $entry)
 */
PHP_FUNCTION(git_tree_entry_dup)
{
	git_tree_entry *result = NULL;
	zval *entry = NULL;
	php_git2_t *_entry = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_entry_dup(PHP_GIT2_V(_entry, tree_entry));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_TREE_ENTRY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto string git_tree_id(resource $tree)
 */
PHP_FUNCTION(git_tree_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *tree = NULL;
	php_git2_t *_tree = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tree) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_id(PHP_GIT2_V(_tree, tree));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto resource git_tree_lookup(resource $repo, string $id)
 */
PHP_FUNCTION(git_tree_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_tree *out = NULL;
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

	error = git_tree_lookup(&out, PHP_GIT2_V(_repo, repository), &__id);

	if (php_git2_check_error(error, "git_tree_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TREE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_tree_owner(resource $tree)
 */
PHP_FUNCTION(git_tree_owner)
{
	git_repository *result = NULL;
	zval *tree = NULL;
	php_git2_t *_tree = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tree) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tree_owner(PHP_GIT2_V(_tree, tree));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto long git_tree_walk(resource $tree, long $mode, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_tree_walk)
{
	int result;
	zval *tree = NULL, *payload = NULL;
	php_git2_t *_tree = NULL;
	zend_long mode;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rlfz", &tree, &mode, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_tree_walk(PHP_GIT2_V(_tree, tree), mode, php_git2_git_treewalk_cb, payload_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

