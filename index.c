#include "php_git2.h"
#include "php_git2_priv.h"
#include "index.h"

/* {{{ proto resource git_index_open(string $index_path)
 */
PHP_FUNCTION(git_index_open)
{
	php_git2_t *result = NULL;
	git_index *out = NULL;
	char *index_path = NULL;
	size_t index_path_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &index_path, &index_path_len) == FAILURE) {
		return;
	}

	error = git_index_open(&out, index_path);

	if (php_git2_check_error(error, "git_index_open" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_INDEX, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_index_new()
 */
PHP_FUNCTION(git_index_new)
{
	php_git2_t *result = NULL;
	git_index *out = NULL;
	int error;
	
	error = git_index_new(&out);

	if (php_git2_check_error(error, "git_index_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_INDEX, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_index_free(resource $index)
 */
PHP_FUNCTION(git_index_free)
{
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_index)) {
		git_index_free(PHP_GIT2_V(_index, index));
		GIT2_SHOULD_FREE(_index) = 0;
	}

	zval_ptr_dtor(index);
}
/* }}} */

/* {{{ proto resource git_index_owner(resource $index)
 */
PHP_FUNCTION(git_index_owner)
{
	git_repository *result = NULL;
	zval *index = NULL;
	php_git2_t *_index = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_owner(PHP_GIT2_V(_index, index));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto long git_index_caps(resource $index)
 */
PHP_FUNCTION(git_index_caps)
{
	unsigned int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_caps(PHP_GIT2_V(_index, index));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_set_caps(resource $index, long $caps)
 */
PHP_FUNCTION(git_index_set_caps)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	zend_long caps;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &index, &caps) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_set_caps(PHP_GIT2_V(_index, index), caps);

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_index_read(resource $index, long $force)
 */
PHP_FUNCTION(git_index_read)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	zend_long force;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &index, &force) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_read(PHP_GIT2_V(_index, index), force);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_write(resource $index)
 */
PHP_FUNCTION(git_index_write)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_write(PHP_GIT2_V(_index, index));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_index_path(resource $index)
 */
PHP_FUNCTION(git_index_path)
{
	const char *result = NULL;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_path(PHP_GIT2_V(_index, index));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_index_read_tree(resource $index, resource $tree)
 */
PHP_FUNCTION(git_index_read_tree)
{
	int result;
	zval *index = NULL, *tree = NULL;
	php_git2_t *_index = NULL, *_tree = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &index, &tree) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_read_tree(PHP_GIT2_V(_index, index), PHP_GIT2_V(_tree, tree));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_index_write_tree(resource $index)
 */
PHP_FUNCTION(git_index_write_tree)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *index = NULL;
	php_git2_t *_index = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_index_write_tree(&out, PHP_GIT2_V(_index, index));

	if (php_git2_check_error(error, "git_index_write_tree" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto string git_index_write_tree_to(resource $index, resource $repo)
 */
PHP_FUNCTION(git_index_write_tree_to)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *index = NULL, *repo = NULL;
	php_git2_t *_index = NULL, *_repo = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &index, &repo) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_index_write_tree_to(&out, PHP_GIT2_V(_index, index), PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_index_write_tree_to" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto long git_index_entrycount(resource $index)
 */
PHP_FUNCTION(git_index_entrycount)
{
	size_t result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_entrycount(PHP_GIT2_V(_index, index));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_index_clear(resource $index)
 */
PHP_FUNCTION(git_index_clear)
{
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_index_clear(PHP_GIT2_V(_index, index));
}
/* }}} */

/* {{{ proto array git_index_get_byindex(resource $index, long $n)
 */
PHP_FUNCTION(git_index_get_byindex)
{
	const git_index_entry *result = NULL;
	zval *index = NULL, *array = NULL;
	php_git2_t *_index = NULL;
	zend_long n;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &index, &n) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_get_byindex(PHP_GIT2_V(_index, index), n);
	if (result == NULL) {
		RETURN_FALSE;
	}

	php_git2_git_index_entry_to_array(result, array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto array git_index_get_bypath(resource $index, string $path, long $stage)
 */
PHP_FUNCTION(git_index_get_bypath)
{
	const git_index_entry *result = NULL;
	zval *index = NULL, *array = NULL;
	php_git2_t *_index = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long stage;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &index, &path, &path_len, &stage) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_get_bypath(PHP_GIT2_V(_index, index), path, stage);
	if (result == NULL) {
		RETURN_FALSE;
	}

	php_git2_git_index_entry_to_array(result, array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_index_remove(resource $index, string $path, long $stage)
 */
PHP_FUNCTION(git_index_remove)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long stage;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &index, &path, &path_len, &stage) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_remove(PHP_GIT2_V(_index, index), path, stage);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_remove_directory(resource $index, string $dir, long $stage)
 */
PHP_FUNCTION(git_index_remove_directory)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	char *dir = NULL;
	size_t dir_len;
	zend_long stage;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &index, &dir, &dir_len, &stage) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_remove_directory(PHP_GIT2_V(_index, index), dir, stage);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_add(resource $index, array $source_entry)
 */
PHP_FUNCTION(git_index_add)
{
	int result;
	zval *index = NULL, *source_entry = NULL;
	php_git2_t *_index = NULL;
	git_index_entry _source_entry = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ra", &index, &source_entry) == FAILURE) {
		return;
	}

	php_git2_array_to_git_index_entry(&_source_entry, source_entry TSRMLS_CC);

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_add(PHP_GIT2_V(_index, index), &_source_entry);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_entry_stage(array $entry)
 */
PHP_FUNCTION(git_index_entry_stage)
{
	int result;
	zval *entry = NULL;
	git_index_entry _entry = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"a", &entry) == FAILURE) {
		return;
	}

	php_git2_array_to_git_index_entry(&_entry, entry TSRMLS_CC);

	result = git_index_entry_stage(&_entry);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_add_bypath(resource $index, string $path)
 */
PHP_FUNCTION(git_index_add_bypath)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	char *path = NULL;
	size_t path_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &index, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_add_bypath(PHP_GIT2_V(_index, index), path);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_remove_bypath(resource $index, string $path)
 */
PHP_FUNCTION(git_index_remove_bypath)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	char *path = NULL;
	size_t path_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &index, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_remove_bypath(PHP_GIT2_V(_index, index), path);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_add_all(resource $index, array $pathspec, long $flags, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_index_add_all)
{
	int result;
	zval *index = NULL, *pathspec = NULL, *payload = NULL;
	php_git2_t *_index = NULL;
	git_strarray _pathspec = {0};
	zend_long flags;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ralfz", &index, &pathspec, &flags, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	php_git2_array_to_git_strarray(&_pathspec, pathspec TSRMLS_CC);

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_index_add_all(PHP_GIT2_V(_index, index), &_pathspec, flags, php_git2_git_index_matched_path_cb, callback_cb);
	git_strarray_free(&_pathspec);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_remove_all(resource $index, array $pathspec, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_index_remove_all)
{
	int result;
	zval *index = NULL, *pathspec = NULL, *payload = NULL;
	php_git2_t *_index = NULL;
	git_strarray _pathspec = {0};
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rafz", &index, &pathspec, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	php_git2_array_to_git_strarray(&_pathspec, pathspec TSRMLS_CC);

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_index_remove_all(PHP_GIT2_V(_index, index), &_pathspec, php_git2_git_index_matched_path_cb, callback_cb);
	git_strarray_free(&_pathspec);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_update_all(resource $index, array $pathspec, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_index_update_all)
{
	int result;
	zval *index = NULL, *pathspec = NULL, *payload = NULL;
	php_git2_t *_index = NULL;
	git_strarray _pathspec = {0};
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rafz", &index, &pathspec, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	php_git2_array_to_git_strarray(&_pathspec, pathspec TSRMLS_CC);

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_index_update_all(PHP_GIT2_V(_index, index), &_pathspec, php_git2_git_index_matched_path_cb, callback_cb);
	git_strarray_free(&_pathspec);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_index_find(resource $index, string $path)
 */
PHP_FUNCTION(git_index_find)
{
	size_t at_pos = 0, path_len;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	char *path = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &index, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_index_find(&at_pos, PHP_GIT2_V(_index, index), path);

	if (php_git2_check_error(error, "git_index_find" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(at_pos);
}
/* }}} */

/* {{{ proto long git_index_conflict_add(resource $index, array $ancestor_entry, array $our_entry, array $their_entry)
 */
PHP_FUNCTION(git_index_conflict_add)
{
	int result;
	zval *index = NULL, *ancestor_entry = NULL, *our_entry = NULL, *their_entry = NULL;
	php_git2_t *_index = NULL;
	git_index_entry _ancestor_entry = {0};
	git_index_entry _our_entry = {0};
	git_index_entry _their_entry = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"raaa", &index, &ancestor_entry, &our_entry, &their_entry) == FAILURE) {
		return;
	}

	php_git2_array_to_git_index_entry(&_ancestor_entry, ancestor_entry TSRMLS_CC);

	php_git2_array_to_git_index_entry(&_our_entry, our_entry TSRMLS_CC);

	php_git2_array_to_git_index_entry(&_their_entry, their_entry TSRMLS_CC);

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_conflict_add(PHP_GIT2_V(_index, index), &_ancestor_entry, &_our_entry, &_their_entry);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_index_conflict_get(resource $index, string $path)
 */
PHP_FUNCTION(git_index_conflict_get)
{
	php_git2_t *_index = NULL;
	const git_index_entry *ancestor_out = NULL, *our_out = NULL, *their_out = NULL;
	zval *index = NULL, *ancestor, *our, *their, *container;
	char *path = NULL;
	int path_len = 0, error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &index, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_index_conflict_get(&ancestor_out, &our_out, &their_out, PHP_GIT2_V(_index, index), path);
	if (php_git2_check_error(error, "git_index_conflict_get" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_index_entry_to_array(ancestor_out, ancestor TSRMLS_CC);
	php_git2_git_index_entry_to_array(our_out, our TSRMLS_CC);
	php_git2_git_index_entry_to_array(their_out, their TSRMLS_CC);

    array_init(container);
    add_assoc_zval_ex(container, ZEND_STRL("ancestor"), ancestor);
    add_assoc_zval_ex(container, ZEND_STRL("our"), our);
    add_assoc_zval_ex(container, ZEND_STRL("their"), their);
	RETURN_ZVAL(container, 0, 1);
}
/* }}} */

/* {{{ proto long git_index_conflict_remove(resource $index, string $path)
 */
PHP_FUNCTION(git_index_conflict_remove)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	char *path = NULL;
	size_t path_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &index, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_conflict_remove(PHP_GIT2_V(_index, index), path);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_index_conflict_cleanup(resource $index)
 */
PHP_FUNCTION(git_index_conflict_cleanup)
{
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_index_conflict_cleanup(PHP_GIT2_V(_index, index));
}
/* }}} */

/* {{{ proto long git_index_has_conflicts(resource $index)
 */
PHP_FUNCTION(git_index_has_conflicts)
{
	int result;
	zval *index = NULL;
	php_git2_t *_index = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_index_has_conflicts(PHP_GIT2_V(_index, index));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto resource git_index_conflict_iterator_new(resource $index)
 */
PHP_FUNCTION(git_index_conflict_iterator_new)
{
	php_git2_t *result = NULL, *_index = NULL;
	git_index_conflict_iterator *iterator_out = NULL;
	zval *index = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &index) == FAILURE) {
		return;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_index_conflict_iterator_new(&iterator_out, PHP_GIT2_V(_index, index));

	if (php_git2_check_error(error, "git_index_conflict_iterator_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_INDEX_CONFLICT_ITERATOR, iterator_out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_index_conflict_next(resource $iterator)
 */
PHP_FUNCTION(git_index_conflict_next)
{
	php_git2_t *_iterator = NULL;
	const git_index_entry *ancestor_out = NULL, *our_out = NULL, *their_out = NULL;
	zval *iterator = NULL, *ancestor, *our, *their, *container;
	int error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iterator) == FAILURE) {
		return;
	}

	if ((_iterator = (php_git2_t *) zend_fetch_resource(Z_RES_P(iterator), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_index_conflict_next(&ancestor_out, &our_out, &their_out, PHP_GIT2_V(_iterator, index_conflict_iterator));
    if (error == GIT_ITEROVER) {
        RETURN_NULL();
    } else if (php_git2_check_error(error, "git_index_conflict_next" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_index_entry_to_array(ancestor_out, ancestor TSRMLS_CC);
	php_git2_git_index_entry_to_array(our_out, our TSRMLS_CC);
	php_git2_git_index_entry_to_array(their_out, their TSRMLS_CC);

	array_init(container);
    add_assoc_zval_ex(container, ZEND_STRL("ancestor"), ancestor);
    add_assoc_zval_ex(container, ZEND_STRL("our"), our);
    add_assoc_zval_ex(container, ZEND_STRL("their"), their);
	RETURN_ZVAL(container, 0, 1);
}
/* }}} */

/* {{{ proto void git_index_conflict_iterator_free(resource $iterator)
 */
PHP_FUNCTION(git_index_conflict_iterator_free)
{
	zval *iterator = NULL;
	php_git2_t *_iterator = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iterator) == FAILURE) {
		return;
	}

	if ((_iterator = (php_git2_t *) zend_fetch_resource(Z_RES_P(iterator), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_iterator)) {
		git_index_conflict_iterator_free(PHP_GIT2_V(_iterator, index_conflict_iterator));
		GIT2_SHOULD_FREE(_iterator) = 0;
	}

	zval_ptr_dtor(iterator);
}
/* }}} */

