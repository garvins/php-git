#include "php_git2.h"
#include "php_git2_priv.h"
#include "repository.h"

/* {{{ proto resource git_repository_new()
*/
PHP_FUNCTION(git_repository_new)
{
	git_repository *repository;
	php_git2_t *git2;
	int error;

	error = git_repository_new(&repository);
	if (php_git2_check_error(error, "git_repository_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&git2, PHP_GIT2_TYPE_REPOSITORY, repository, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(git2));
}

/* {{{ proto resource git_repository_init(string $path, long is_bare = 0)
*/
PHP_FUNCTION(git_repository_init)
{
	git_repository *repository;
	php_git2_t *git2;
	char *path;
	int error, path_len, is_bare = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s|l", &path, &path_len, &is_bare) == FAILURE) {
		return;
	}
	error = git_repository_init(&repository, path, is_bare);
	if (php_git2_check_error(error, "git_repository_init" TSRMLS_CC)) {
		RETURN_FALSE
	}
	if (php_git2_make_resource(&git2, PHP_GIT2_TYPE_REPOSITORY, repository, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(git2));
}

/* {{{ proto resource git_repository_open(string $path)
*/
PHP_FUNCTION(git_repository_open_bare)
{
	char *path;
	int path_len;
	git_repository *repository;
	int error = 0;
	php_git2_t *git2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &path, &path_len) == FAILURE) {
		return;
	}

	error = git_repository_open_bare(&repository, path);
	if (php_git2_check_error(error, "git_repository_open_bare" TSRMLS_CC)) {
		RETURN_FALSE
	}
	if (php_git2_make_resource(&git2, PHP_GIT2_TYPE_REPOSITORY, repository, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}
	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(git2));
}

/* {{{ proto resource git_repository_open(string $path)
 */
PHP_FUNCTION(git_repository_open)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *path = NULL;
	int path_len = 0, error = 0;

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


/* {{{ proto string git_repository_get_namespace(resource $repository)
*/
PHP_FUNCTION(git_repository_get_namespace)
{
	zval *repository;
	php_git2_t *git2;
	const char *ns;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repository) == FAILURE) {
		return;
	}

	if ((git2 = (php_git2_t *) zend_fetch_resource(Z_RES_P(repository), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	ns = git_repository_get_namespace(PHP_GIT2_V(git2, repository));
	if (ns != NULL) {
		RETURN_STRING(ns);
	} else {
		RETURN_STRING("", 1);
	}
}
/* }}} */

/* {{{ proto string git_repository_workdir(resource $repository)
*/
PHP_FUNCTION(git_repository_workdir)
{
	zval *repository;
	php_git2_t *git2;
	const char *workdir;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repository) == FAILURE) {
		return;
	}

	if ((git2 = (php_git2_t *) zend_fetch_resource(Z_RES_P(repository), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	workdir = git_repository_workdir(PHP_GIT2_V(git2, repository));
	if (workdir != NULL) {
		RETURN_STRING(workdir);
	} else {
		RETURN_STRING("", 1);
	}
}
/* }}} */

/* {{{ proto resource git_repository_wrap_odb(resource $odb)
 */
PHP_FUNCTION(git_repository_wrap_odb)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	zval *odb = NULL;
	php_git2_t *_odb = NULL;
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


/* {{{ proto resource git_repository_discover(start_path, across_fs, ceiling_dirs)
*/
PHP_FUNCTION(git_repository_discover)
{
	char *start_path = {0};
	int start_path_len;
	long across_fs;
	char *ceiling_dirs = {0};
	int ceiling_dirs_len;
	int error = 0;
	char buffer[GIT2_BUFFER_SIZE];
	size_t buffer_len = GIT2_BUFFER_SIZE;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sls", &start_path, &start_path_len, &across_fs, &ceiling_dirs, &ceiling_dirs_len) == FAILURE) {
		return;
	}

	error = git_repository_discover(buffer, buffer_len, start_path, across_fs, ceiling_dirs);
	if (php_git2_check_error(error, "git_repository_discover" TSRMLS_CC)) {
		RETURN_FALSE
	}

	RETURN_STRING(buffer);
}

/* {{{ proto resource git_repository_open_ext(string $path, long $flags, string $ceiling_dirs)
 */
PHP_FUNCTION(git_repository_open_ext)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *path = NULL;
	int path_len = 0;
	long flags = 0;
	char *ceiling_dirs = NULL;
	int ceiling_dirs_len = 0;
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
	};
	//zval_ptr_dtor(&repo);
}
/* }}} */

/* {{{ proto resource git_repository_init_ext(string $repo_path,  $opts)
 */
PHP_FUNCTION(git_repository_init_ext)
{
	git_repository_init_options options = GIT_REPOSITORY_INIT_OPTIONS_INIT;
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *repo_path = NULL;
	int repo_path_len = 0, error = 0;
	zval *opts = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sa", &repo_path, &repo_path_len, &opts) == FAILURE) {
		return;
	}

	php_git2_array_to_git_repository_init_options(&options, opts TSRMLS_CC);
	error = git_repository_init_ext(&out, repo_path, &options);
	if (php_git2_check_error(error, "git_repository_init_ext" TSRMLS_CC)) {
		RETURN_FALSE;
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
	php_git2_t *result = NULL;
	git_reference *out = NULL;
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
	int result = 0;
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
	int result = 0;
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
	int result = 0;
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
	const char  *result = NULL;
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
	int result = 0;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *workdir = NULL;
	int workdir_len = 0;
	long update_gitlink = 0;

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
	int result = 0;
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
	php_git2_t *result = NULL;
	git_config *out = NULL;
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
	php_git2_t *result = NULL;
	git_odb *out = NULL;
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
	php_git2_t *result = NULL;
	git_refdb *out = NULL;
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
	php_git2_t *result = NULL;
	git_index *out = NULL;
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

/* {{{ proto resource git_repository_message(long $len, resource $repo)
 */
PHP_FUNCTION(git_repository_message)
{
	zval *repo;
	php_git2_t *_repo;
	char buffer[GIT2_BUFFER_SIZE];
	size_t buffer_len = GIT2_BUFFER_SIZE;
	int error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_message(buffer, buffer_len, PHP_GIT2_V(_repo, repository));
	if (php_git2_check_error(error, "git_repository_message" TSRMLS_CC)) {
		RETURN_FALSE
	}
	RETURN_STRING(buffer);
}
/* }}} */


/* {{{ proto long git_repository_message_remove(resource $repo)
 */
PHP_FUNCTION(git_repository_message_remove)
{
	int result = 0;
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
	int result = 0;
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

/* {{{ proto long git_repository_fetchhead_foreach(resource $repo, Callable $callback,  $payload)
 */
PHP_FUNCTION(git_repository_fetchhead_foreach)
{
	int result = 0;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &fci, &fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb, &fci, &fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_repository_fetchhead_foreach(PHP_GIT2_V(_repo, repository), php_git2_repository_fetchhead_foreach_cb, cb);
	php_git2_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_repository_mergehead_foreach(resource $repo, Callable $callback,  $payload)
 */
PHP_FUNCTION(git_repository_mergehead_foreach)
{
	int result = 0;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &fci, &fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb, &fci, &fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}
	result = git_repository_mergehead_foreach(PHP_GIT2_V(_repo, repository), php_git2_repository_mergehead_foreach_cb, cb);
	php_git2_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */


/* {{{ proto string git_repository_hashfile(resource $repo, string $path, long $type, string $as_path)
 */
PHP_FUNCTION(git_repository_hashfile)
{
	php_git2_t *_repo = NULL;
	git_oid out = {0};
	zval *repo = NULL;
	char *path = NULL, *as_path = NULL, buf[41] = {0};
	int path_len = 0, as_path_len = 0, error = 0;
	long type = 0;

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
	git_oid_fmt(buf, &out);
	RETURN_STRING(buf);
}
/* }}} */

/* {{{ proto long git_repository_set_head(repo, refname)
*/
PHP_FUNCTION(git_repository_set_head)
{
	zval *repo;
	php_git2_t *_repo;
	char *refname = {0};
	int refname_len;
	int error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &refname, &refname_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_set_head(PHP_GIT2_V(_repo, repository), refname);
	if (php_git2_check_error(error, "git_repository_set_head" TSRMLS_CC)) {
		RETURN_FALSE
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto long git_repository_set_head_detached(repo, commitish)
*/
PHP_FUNCTION(git_repository_set_head_detached)
{
	zval *repo;
	php_git2_t *_repo;
	char *commitish = {0};
	int commitish_len;
	int error = 0;
	git_oid id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &commitish, &commitish_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&id, commitish, commitish_len) != GIT_OK) {
		return;
	}

	error = git_repository_set_head_detached(PHP_GIT2_V(_repo, repository), &id);
	if (php_git2_check_error(error, "git_repository_set_head_detached" TSRMLS_CC)) {
		RETURN_FALSE
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto long git_repository_detach_head(repo)
*/
PHP_FUNCTION(git_repository_detach_head)
{
	zval *repo;
	php_git2_t *_repo;
	int error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_detach_head(PHP_GIT2_V(_repo, repository));
	if (php_git2_check_error(error, "git_repository_detach_head" TSRMLS_CC)) {
		RETURN_FALSE
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto long git_repository_state(repo)
*/
PHP_FUNCTION(git_repository_state)
{
	zval *repo;
	php_git2_t *_repo;
	int state = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	state = git_repository_state(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(state);
}
/* }}} */

/* {{{ proto long git_repository_set_namespace(repo, nmspace)
*/
PHP_FUNCTION(git_repository_set_namespace)
{
	zval *repo;
	php_git2_t *_repo;
	char *nmspace = {0};
	int nmspace_len;
	int error = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &nmspace, &nmspace_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_repository_set_namespace(PHP_GIT2_V(_repo, repository), nmspace);
	if (php_git2_check_error(error, "git_repository_set_namespace" TSRMLS_CC)) {
		RETURN_FALSE
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto long git_repository_is_shallow(repo)
*/
PHP_FUNCTION(git_repository_is_shallow)
{
	zval *repo;
	php_git2_t *_repo;
	int is_shallow;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	is_shallow = git_repository_is_shallow(PHP_GIT2_V(_repo, repository));
	RETURN_LONG(is_shallow);
}
/* }}} */

/* {{{ proto array git_repository_init_options_new()
*/
PHP_FUNCTION(git_repository_init_options_new)
{
	git_repository_init_options opts = GIT_REPOSITORY_INIT_OPTIONS_INIT;
	zval *result;

	php_git2_git_repository_init_options_to_array(&opts, &result TSRMLS_CC);
	RETURN_ZVAL(result, 0, 1);
}
/* }}} */