#include "php_git2.h"
#include "php_git2_priv.h"
#include "repository.h"

/* {{{ proto resource git_repository_new()
 */
PHP_FUNCTION(git_repository_new)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	int error = 0;
	
	error = git_repository_new(&out);

	if (php_git2_check_error(error, "git_repository_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_init(string $path, long $is_bare)
 */
PHP_FUNCTION(git_repository_init)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long is_bare;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &path, &path_len, &is_bare) == FAILURE) {
		return;
	}

	error = git_repository_init(&out, path, is_bare);

	if (php_git2_check_error(error, "git_repository_init" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_open_bare(string $bare_path)
 */
PHP_FUNCTION(git_repository_open_bare)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *bare_path = NULL;
	size_t bare_path_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &bare_path, &bare_path_len) == FAILURE) {
		return;
	}

	error = git_repository_open_bare(&out, bare_path);

	if (php_git2_check_error(error, "git_repository_open_bare" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_open(string $path)
 */
PHP_FUNCTION(git_repository_open)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *path = NULL;
	size_t path_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &path, &path_len) == FAILURE) {
		return;
	}

	error = git_repository_open(&out, path);

	if (php_git2_check_error(error, "git_repository_open" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_repository_get_namespace(resource $repo)
 */
PHP_FUNCTION(git_repository_get_namespace)
{
	const char *result = NULL;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_get_namespace(PHP_GIT2_V(_repo, repository));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_repository_workdir(resource $repo)
 */
PHP_FUNCTION(git_repository_workdir)
{
	const char *result = NULL;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_workdir(PHP_GIT2_V(_repo, repository));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto resource git_repository_wrap_odb(resource $odb)
 */
PHP_FUNCTION(git_repository_wrap_odb)
{
	php_git2_t *result = NULL, *_odb = NULL;
	git_repository *out = NULL;
	zval *odb = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &odb) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_wrap_odb(&out, PHP_GIT2_V(_odb, odb));

	if (php_git2_check_error(error, "git_repository_wrap_odb" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_repository_discover(string $start_path, long $across_fs, string $ceiling_dirs)
 */
PHP_FUNCTION(git_repository_discover)
{
	char path_out[GIT2_BUFFER_SIZE] = {0}, *start_path = NULL, *ceiling_dirs = NULL;
	size_t path_size = GIT2_BUFFER_SIZE, start_path_len, ceiling_dirs_len;
	zend_long across_fs;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sls", &start_path, &start_path_len, &across_fs, &ceiling_dirs, &ceiling_dirs_len) == FAILURE) {
		return;
	}

	error = git_repository_discover(&path_out, path_size, start_path, across_fs, ceiling_dirs);

	if (php_git2_check_error(error, "git_repository_discover" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(&path_out);
}
/* }}} */

/* {{{ proto resource git_repository_open_ext(string $path, long $flags, string $ceiling_dirs)
 */
PHP_FUNCTION(git_repository_open_ext)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *path = NULL, *ceiling_dirs = NULL;
	size_t path_len, ceiling_dirs_len;
	zend_long flags;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sls", &path, &path_len, &flags, &ceiling_dirs, &ceiling_dirs_len) == FAILURE) {
		return;
	}

	error = git_repository_open_ext(&out, path, flags, ceiling_dirs);

	if (php_git2_check_error(error, "git_repository_open_ext" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_repository_free(resource $repo)
 */
PHP_FUNCTION(git_repository_free)
{
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_repo)) {
		git_repository_free(PHP_GIT2_V(_repo, repository));
		GIT2_SHOULD_FREE(_repo) = 0;
	}

	zval_ptr_dtor(&repo);
}
/* }}} */

/* {{{ proto resource git_repository_init_ext(string $repo_path, array $opts)
 */
PHP_FUNCTION(git_repository_init_ext)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *repo_path = NULL;
	size_t repo_path_len;
	git_repository_init_options *_opts = NULL;
	zval *opts = NULL;
	int should_free = 0;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s|a", &repo_path, &repo_path_len, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_repository_init_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	error = git_repository_init_ext(&out, repo_path, _opts);

	if (php_git2_check_error(error, "git_repository_init_ext" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_repository_init_options_free(_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_head(resource $repo)
 */
PHP_FUNCTION(git_repository_head)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_head(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_repository_head" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_repository_head_detached(resource $repo)
 */
PHP_FUNCTION(git_repository_head_detached)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_head_detached(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_head_unborn(resource $repo)
 */
PHP_FUNCTION(git_repository_head_unborn)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_head_unborn(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_is_empty(resource $repo)
 */
PHP_FUNCTION(git_repository_is_empty)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_is_empty(PHP_GIT2_V(_repo, repository));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto string git_repository_path(resource $repo)
 */
PHP_FUNCTION(git_repository_path)
{
	const char *result = NULL;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_path(PHP_GIT2_V(_repo, repository));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_repository_set_workdir(resource $repo, string $workdir, long $update_gitlink)
 */
PHP_FUNCTION(git_repository_set_workdir)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *workdir = NULL;
	size_t workdir_len;
	zend_long update_gitlink;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &repo, &workdir, &workdir_len, &update_gitlink) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_set_workdir(PHP_GIT2_V(_repo, repository), workdir, update_gitlink);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_is_bare(resource $repo)
 */
PHP_FUNCTION(git_repository_is_bare)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_is_bare(PHP_GIT2_V(_repo, repository));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto resource git_repository_config(resource $repo)
 */
PHP_FUNCTION(git_repository_config)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_config *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_config(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_repository_config" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_odb(resource $repo)
 */
PHP_FUNCTION(git_repository_odb)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_odb *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_odb(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_repository_odb" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_refdb(resource $repo)
 */
PHP_FUNCTION(git_repository_refdb)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_refdb *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_refdb(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_repository_refdb" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFDB, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_repository_index(resource $repo)
 */
PHP_FUNCTION(git_repository_index)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_index *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_index(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_repository_index" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_INDEX, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_repository_message(resource $repo)
 */
PHP_FUNCTION(git_repository_message)
{
	char out[GIT2_BUFFER_SIZE] = {0};
	size_t len = GIT2_BUFFER_SIZE;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_message(&out, len, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_repository_message" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(&out);
}
/* }}} */

/* {{{ proto long git_repository_message_remove(resource $repo)
 */
PHP_FUNCTION(git_repository_message_remove)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_message_remove(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_merge_cleanup(resource $repo)
 */
PHP_FUNCTION(git_repository_merge_cleanup)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_merge_cleanup(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_fetchhead_foreach(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_repository_fetchhead_foreach)
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

	result = git_repository_fetchhead_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_repository_fetchhead_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_mergehead_foreach(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_repository_mergehead_foreach)
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

	result = git_repository_mergehead_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_repository_mergehead_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_repository_hashfile(resource $repo, string $path, long $type, string $as_path)
 */
PHP_FUNCTION(git_repository_hashfile)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL, *as_path = NULL;
	size_t path_len, as_path_len;
	zend_long type;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsls", &repo, &path, &path_len, &type, &as_path, &as_path_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_hashfile(&out, PHP_GIT2_V(_repo, repository), path, type, as_path);

	if (php_git2_check_error(error, "git_repository_hashfile" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto long git_repository_set_head(resource $repo, string $refname)
 */
PHP_FUNCTION(git_repository_set_head)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *refname = NULL;
	size_t refname_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &refname, &refname_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_set_head(PHP_GIT2_V(_repo, repository), refname);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_set_head_detached(resource $repo, string $commitish)
 */
PHP_FUNCTION(git_repository_set_head_detached)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *commitish = NULL;
	size_t commitish_len;
	git_oid __commitish = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &commitish, &commitish_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__commitish, commitish, commitish_len)) {
		RETURN_FALSE;
	}

	result = git_repository_set_head_detached(PHP_GIT2_V(_repo, repository), &__commitish);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_detach_head(resource $repo)
 */
PHP_FUNCTION(git_repository_detach_head)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_detach_head(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_state(resource $repo)
 */
PHP_FUNCTION(git_repository_state)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_state(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_set_namespace(resource $repo, string $nmspace)
 */
PHP_FUNCTION(git_repository_set_namespace)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *nmspace = NULL;
	size_t nmspace_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &nmspace, &nmspace_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_set_namespace(PHP_GIT2_V(_repo, repository), nmspace);

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_repository_is_shallow(resource $repo)
 */
PHP_FUNCTION(git_repository_is_shallow)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_repository_is_shallow(PHP_GIT2_V(_repo, repository));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto array git_repository_init_options_new()
 */
PHP_FUNCTION(git_repository_init_options_new)
{
	zval *result;
	git_repository_init_options options = GIT_REPOSITORY_INIT_OPTIONS_INIT;

	php_git2_git_repository_init_options_to_array(&options, &result TSRMLS_CC);
	RETURN_ZVAL(result, 0, 1);
}
/* }}} */

