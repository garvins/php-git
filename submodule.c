#include "php_git2.h"
#include "php_git2_priv.h"
#include "submodule.h"

/* {{{ proto resource git_submodule_lookup(resource $repo, string $name)
 */
PHP_FUNCTION(git_submodule_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_submodule *submodule = NULL;
	zval *repo = NULL;
	char *name = NULL;
	size_t name_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_submodule_lookup(&submodule, PHP_GIT2_V(_repo, repository), name);

	if (php_git2_check_error(error, "git_submodule_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_SUBMODULE, submodule, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_submodule_foreach(resource $repo, Callable $cb, void $payload)
 */
PHP_FUNCTION(git_submodule_foreach)
{
	int result;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info cb_fci = empty_fcall_info;
	zend_fcall_info_cache cb_fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &cb_fci, &cb_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb_cb, &cb_fci, &cb_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_submodule_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_submodule_foreach_cb, cb_cb);
	php_git2_cb_free(cb_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_submodule_add_setup(resource $repo, string $url, string $path, long $use_gitlink)
 */
PHP_FUNCTION(git_submodule_add_setup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_submodule *submodule = NULL;
	zval *repo = NULL;
	char *url = NULL, *path = NULL;
	size_t url_len, path_len;
	zend_long use_gitlink;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rssl", &repo, &url, &url_len, &path, &path_len, &use_gitlink) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_submodule_add_setup(&submodule, PHP_GIT2_V(_repo, repository), url, path, use_gitlink);

	if (php_git2_check_error(error, "git_submodule_add_setup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_SUBMODULE, submodule, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_submodule_add_finalize(resource $submodule)
 */
PHP_FUNCTION(git_submodule_add_finalize)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_add_finalize(PHP_GIT2_V(_submodule, submodule));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_add_to_index(resource $submodule, long $write_index)
 */
PHP_FUNCTION(git_submodule_add_to_index)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	zend_long write_index;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &submodule, &write_index) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_add_to_index(PHP_GIT2_V(_submodule, submodule), write_index);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_save(resource $submodule)
 */
PHP_FUNCTION(git_submodule_save)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_save(PHP_GIT2_V(_submodule, submodule));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_submodule_owner(resource $submodule)
 */
PHP_FUNCTION(git_submodule_owner)
{
	git_repository *result = NULL;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_owner(PHP_GIT2_V(_submodule, submodule));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto string git_submodule_name(resource $submodule)
 */
PHP_FUNCTION(git_submodule_name)
{
	const char *result = NULL;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_name(PHP_GIT2_V(_submodule, submodule));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_submodule_path(resource $submodule)
 */
PHP_FUNCTION(git_submodule_path)
{
	const char *result = NULL;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_path(PHP_GIT2_V(_submodule, submodule));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_submodule_url(resource $submodule)
 */
PHP_FUNCTION(git_submodule_url)
{
	const char *result = NULL;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_url(PHP_GIT2_V(_submodule, submodule));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_submodule_set_url(resource $submodule, string $url)
 */
PHP_FUNCTION(git_submodule_set_url)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	char *url = NULL;
	size_t url_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &submodule, &url, &url_len) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_set_url(PHP_GIT2_V(_submodule, submodule), url);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_submodule_index_id(resource $submodule)
 */
PHP_FUNCTION(git_submodule_index_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_index_id(PHP_GIT2_V(_submodule, submodule));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_submodule_head_id(resource $submodule)
 */
PHP_FUNCTION(git_submodule_head_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_head_id(PHP_GIT2_V(_submodule, submodule));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_submodule_wd_id(resource $submodule)
 */
PHP_FUNCTION(git_submodule_wd_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_wd_id(PHP_GIT2_V(_submodule, submodule));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_submodule_ignore(resource $submodule)
 */
PHP_FUNCTION(git_submodule_ignore)
{
	git_submodule_ignore_t result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_ignore(PHP_GIT2_V(_submodule, submodule));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_set_ignore(resource $submodule, long $ignore)
 */
PHP_FUNCTION(git_submodule_set_ignore)
{
	git_submodule_ignore_t result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	zend_long ignore;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &submodule, &ignore) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_set_ignore(PHP_GIT2_V(_submodule, submodule), ignore);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_update(resource $submodule)
 */
PHP_FUNCTION(git_submodule_update)
{
	git_submodule_update_t result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_update(PHP_GIT2_V(_submodule, submodule));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_set_update(resource $submodule, long $update)
 */
PHP_FUNCTION(git_submodule_set_update)
{
	git_submodule_update_t result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	zend_long update;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &submodule, &update) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_set_update(PHP_GIT2_V(_submodule, submodule), update);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_fetch_recurse_submodules(resource $submodule)
 */
PHP_FUNCTION(git_submodule_fetch_recurse_submodules)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_fetch_recurse_submodules(PHP_GIT2_V(_submodule, submodule));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_submodule_set_fetch_recurse_submodules(resource $submodule, long $fetch_recurse_submodules)
 */
PHP_FUNCTION(git_submodule_set_fetch_recurse_submodules)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	zend_long fetch_recurse_submodules;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &submodule, &fetch_recurse_submodules) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_set_fetch_recurse_submodules(PHP_GIT2_V(_submodule, submodule), fetch_recurse_submodules);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_init(resource $submodule, long $overwrite)
 */
PHP_FUNCTION(git_submodule_init)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	zend_long overwrite;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &submodule, &overwrite) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_init(PHP_GIT2_V(_submodule, submodule), overwrite);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_sync(resource $submodule)
 */
PHP_FUNCTION(git_submodule_sync)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_sync(PHP_GIT2_V(_submodule, submodule));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_submodule_open(resource $submodule)
 */
PHP_FUNCTION(git_submodule_open)
{
	php_git2_t *result = NULL, *_submodule = NULL;
	git_repository *repo = NULL;
	zval *submodule = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_submodule_open(&repo, PHP_GIT2_V(_submodule, submodule));

	if (php_git2_check_error(error, "git_submodule_open" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, repo, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_submodule_reload(resource $submodule)
 */
PHP_FUNCTION(git_submodule_reload)
{
	int result;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_submodule_reload(PHP_GIT2_V(_submodule, submodule));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_reload_all(resource $repo)
 */
PHP_FUNCTION(git_submodule_reload_all)
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

	result = git_submodule_reload_all(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_submodule_status(resource $submodule)
 */
PHP_FUNCTION(git_submodule_status)
{
	unsigned int status = 0;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_submodule_status(&status, PHP_GIT2_V(_submodule, submodule));

	if (php_git2_check_error(error, "git_submodule_status" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(status);
}
/* }}} */

/* {{{ proto long git_submodule_location(resource $submodule)
 */
PHP_FUNCTION(git_submodule_location)
{
	unsigned int location_status = 0;
	zval *submodule = NULL;
	php_git2_t *_submodule = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &submodule) == FAILURE) {
		return;
	}

	if ((_submodule = (php_git2_t *) zend_fetch_resource(Z_RES_P(submodule), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_submodule_location(&location_status, PHP_GIT2_V(_submodule, submodule));

	if (php_git2_check_error(error, "git_submodule_location" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(location_status);
}
/* }}} */

