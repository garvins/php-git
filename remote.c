#include "php_git2.h"
#include "php_git2_priv.h"
#include "remote.h"

/* {{{ proto resource git_remote_create(resource $repo, string $name, string $url)
 */
PHP_FUNCTION(git_remote_create)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_remote *out = NULL;
	zval *repo = NULL;
	char *name = NULL, *url = NULL;
	size_t name_len, url_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &repo, &name, &name_len, &url, &url_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_create(&out, PHP_GIT2_V(_repo, repository), name, url);

	if (php_git2_check_error(error, "git_remote_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REMOTE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_remote_create_with_fetchspec(resource $repo, string $name, string $url, string $fetch)
 */
PHP_FUNCTION(git_remote_create_with_fetchspec)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_remote *out = NULL;
	zval *repo = NULL;
	char *name = NULL, *url = NULL, *fetch = NULL;
	size_t name_len, url_len, fetch_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsss", &repo, &name, &name_len, &url, &url_len, &fetch, &fetch_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_create_with_fetchspec(&out, PHP_GIT2_V(_repo, repository), name, url, fetch);

	if (php_git2_check_error(error, "git_remote_create_with_fetchspec" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REMOTE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_remote_create_inmemory(resource $repo, string $fetch, string $url)
 */
PHP_FUNCTION(git_remote_create_inmemory)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_remote *out = NULL;
	zval *repo = NULL;
	char *fetch = NULL, *url = NULL;
	size_t fetch_len, url_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &repo, &fetch, &fetch_len, &url, &url_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_create_inmemory(&out, PHP_GIT2_V(_repo, repository), fetch, url);

	if (php_git2_check_error(error, "git_remote_create_inmemory" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REMOTE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_remote_load(resource $repo, string $name)
 */
PHP_FUNCTION(git_remote_load)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_remote *out = NULL;
	zval *repo = NULL;
	char *name = NULL;
	size_t name_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_load(&out, PHP_GIT2_V(_repo, repository), name);

	if (php_git2_check_error(error, "git_remote_load" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REMOTE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_remote_save(resource $remote)
 */
PHP_FUNCTION(git_remote_save)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_save(PHP_GIT2_V(_remote, remote));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto resource git_remote_owner(resource $remote)
 */
PHP_FUNCTION(git_remote_owner)
{
	git_repository *result = NULL;
	zval *remote = NULL;
	php_git2_t *_remote = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_owner(PHP_GIT2_V(_remote, remote));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto string git_remote_name(resource $remote)
 */
PHP_FUNCTION(git_remote_name)
{
	const char *result = NULL;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_name(PHP_GIT2_V(_remote, remote));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_remote_url(resource $remote)
 */
PHP_FUNCTION(git_remote_url)
{
	const char *result = NULL;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_url(PHP_GIT2_V(_remote, remote));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_remote_pushurl(resource $remote)
 */
PHP_FUNCTION(git_remote_pushurl)
{
	const char *result = NULL;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_pushurl(PHP_GIT2_V(_remote, remote));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_remote_set_url(resource $remote, string $url)
 */
PHP_FUNCTION(git_remote_set_url)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	char *url = NULL;
	size_t url_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &remote, &url, &url_len) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_set_url(PHP_GIT2_V(_remote, remote), url);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_set_pushurl(resource $remote, string $url)
 */
PHP_FUNCTION(git_remote_set_pushurl)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	char *url = NULL;
	size_t url_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &remote, &url, &url_len) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_set_pushurl(PHP_GIT2_V(_remote, remote), url);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_add_fetch(resource $remote, string $refspec)
 */
PHP_FUNCTION(git_remote_add_fetch)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	char *refspec = NULL;
	size_t refspec_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &remote, &refspec, &refspec_len) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_add_fetch(PHP_GIT2_V(_remote, remote), refspec);

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto array git_remote_get_fetch_refspecs(resource $remote)
 */
PHP_FUNCTION(git_remote_get_fetch_refspecs)
{
	zval *result, *remote = NULL;
	git_strarray *array = NULL;
	php_git2_t *_remote = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_get_fetch_refspecs(array, PHP_GIT2_V(_remote, remote));

	if (php_git2_check_error(error, "git_remote_get_fetch_refspecs" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_strarray_to_array(array, &array TSRMLS_CC);
	git_strarray_free(array);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_remote_set_fetch_refspecs(resource $remote, array $array)
 */
PHP_FUNCTION(git_remote_set_fetch_refspecs)
{
	int result;
	zval *remote = NULL, *array = NULL;
	php_git2_t *_remote = NULL;
	git_strarray _array = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ra", &remote, &array) == FAILURE) {
		return;
	}

	php_git2_array_to_git_strarray(&_array, array TSRMLS_CC);

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_set_fetch_refspecs(PHP_GIT2_V(_remote, remote), &_array);
	git_strarray_free(&_array);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_add_push(resource $remote, string $refspec)
 */
PHP_FUNCTION(git_remote_add_push)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	char *refspec = NULL;
	size_t refspec_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &remote, &refspec, &refspec_len) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_add_push(PHP_GIT2_V(_remote, remote), refspec);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_remote_get_push_refspecs(resource $remote)
 */
PHP_FUNCTION(git_remote_get_push_refspecs)
{
	zval *result, *remote = NULL;
	git_strarray *array = NULL;
	php_git2_t *_remote = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_get_push_refspecs(array, PHP_GIT2_V(_remote, remote));

	if (php_git2_check_error(error, "git_remote_get_push_refspecs" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_strarray_to_array(array, &array TSRMLS_CC);
	git_strarray_free(array);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_remote_set_push_refspecs(resource $remote, array $array)
 */
PHP_FUNCTION(git_remote_set_push_refspecs)
{
	int result;
	zval *remote = NULL, *array = NULL;
	php_git2_t *_remote = NULL;
	git_strarray _array = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ra", &remote, &array) == FAILURE) {
		return;
	}

	php_git2_array_to_git_strarray(&_array, array TSRMLS_CC);

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_set_push_refspecs(PHP_GIT2_V(_remote, remote), &_array);
	git_strarray_free(&_array);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_remote_clear_refspecs(resource $remote)
 */
PHP_FUNCTION(git_remote_clear_refspecs)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_remote_clear_refspecs(PHP_GIT2_V(_remote, remote));
}
/* }}} */

/* {{{ proto long git_remote_refspec_count(resource $remote)
 */
PHP_FUNCTION(git_remote_refspec_count)
{
	size_t result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_refspec_count(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_remote_get_refspec(resource $remote, long $n)
 */
PHP_FUNCTION(git_remote_get_refspec)
{
	const git_refspec *result = NULL;
	zval *remote = NULL;
	php_git2_t *_remote = NULL, *__result = NULL;
	zend_long n;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &remote, &n) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_get_refspec(PHP_GIT2_V(_remote, remote), n);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REFSPEC, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto long git_remote_connect(resource $remote, long $direction)
 */
PHP_FUNCTION(git_remote_connect)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	zend_long direction;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &remote, &direction) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_connect(PHP_GIT2_V(_remote, remote), direction);

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto resource git_remote_ls(resource $remote)
 */
PHP_FUNCTION(git_remote_ls)
{
	git_remote_head **out = NULL;
	size_t size = 0;
	zval *remote = NULL, *retval = NULL, *container = NULL;
	php_git2_t *_remote = NULL;
	int error = 0, i = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}


	if (git_remote_connected(PHP_GIT2_V(_remote, remote)) == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "passed git_remote hasn't been connected");
		RETURN_FALSE;
	}
	error = git_remote_ls(&out, &size, PHP_GIT2_V(_remote, remote));
	if (php_git2_check_error(error, "git_remote_ls" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	MAKE_STD_ZVAL(container);
	array_init(container);
	for (i = 0; i < size; i++) {
		php_git2_git_remote_head_to_array(out[i], &retval TSRMLS_CC);
		add_next_index_zval(container, retval);
	}
	RETURN_ZVAL(container, 0, 1);
}
/* }}} */

/* {{{ proto long git_remote_download(resource $remote)
 */
PHP_FUNCTION(git_remote_download)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_download(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_connected(resource $remote)
 */
PHP_FUNCTION(git_remote_connected)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_connected(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_remote_stop(resource $remote)
 */
PHP_FUNCTION(git_remote_stop)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_remote_stop(PHP_GIT2_V(_remote, remote));
}
/* }}} */

/* {{{ proto void git_remote_disconnect(resource $remote)
 */
PHP_FUNCTION(git_remote_disconnect)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_remote_disconnect(PHP_GIT2_V(_remote, remote));
}
/* }}} */

/* {{{ proto void git_remote_free(resource $remote)
 */
PHP_FUNCTION(git_remote_free)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_remote)) {
		git_remote_free(PHP_GIT2_V(_remote, remote));
		GIT2_SHOULD_FREE(_remote) = 0;
	);

	zval_ptr_dtor(&remote);
}
/* }}} */

/* {{{ proto long git_remote_update_tips(resource $remote)
 */
PHP_FUNCTION(git_remote_update_tips)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_update_tips(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_fetch(resource $remote)
 */
PHP_FUNCTION(git_remote_fetch)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_fetch(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_valid_url(string $url)
 */
PHP_FUNCTION(git_remote_valid_url)
{
	int result;
	char *url = NULL;
	size_t url_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &url, &url_len) == FAILURE) {
		return;
	}

	result = git_remote_valid_url(url);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_supported_url(string $url)
 */
PHP_FUNCTION(git_remote_supported_url)
{
	int result;
	char *url = NULL;
	size_t url_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &url, &url_len) == FAILURE) {
		return;
	}

	result = git_remote_supported_url(url);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_remote_list(resource $repo)
 */
PHP_FUNCTION(git_remote_list)
{
	zval *result, *repo = NULL;
	git_strarray *out = NULL;
	php_git2_t *_repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_remote_list(out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_remote_list" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_strarray_to_array(out, &array TSRMLS_CC);
	git_strarray_free(out);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto void git_remote_check_cert(resource $remote, long $check)
 */
PHP_FUNCTION(git_remote_check_cert)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	zend_long check;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &remote, &check) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_remote_check_cert(PHP_GIT2_V(_remote, remote), check);
}
/* }}} */

/* {{{ proto long git_remote_set_transport(resource $remote, resource $transport)
 */
PHP_FUNCTION(git_remote_set_transport)
{
	int result;
	zval *remote = NULL, *transport = NULL;
	php_git2_t *_remote = NULL, *_transport = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &remote, &transport) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_transport = (php_git2_t *) zend_fetch_resource(Z_RES_P(transport), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_set_transport(PHP_GIT2_V(_remote, remote), PHP_GIT2_V(_transport, transport));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_set_callbacks(resource $remote, array $callbacks)
 */
PHP_FUNCTION(git_remote_set_callbacks)
{
	int result;
	zval *remote = NULL, *callbacks = NULL;
	php_git2_t *_remote = NULL;
	git_remote_callbacks _callbacks = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ra", &remote, &callbacks) == FAILURE) {
		return;
	}

	php_git2_array_to_git_remote_callbacks(&_callbacks, callbacks TSRMLS_CC);

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_set_callbacks(PHP_GIT2_V(_remote, remote), &_callbacks);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_remote_stats(resource $remote)
 */
PHP_FUNCTION(git_remote_stats)
{
	const git_transfer_progress *result = NULL;
	zval *remote = NULL, *array = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_stats(PHP_GIT2_V(_remote, remote));
	if (result == NULL) {
		RETURN_FALSE;
	}

	php_git2_git_transfer_progress_to_array(result, &array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_remote_autotag(resource $remote)
 */
PHP_FUNCTION(git_remote_autotag)
{
	git_remote_autotag_option_t result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_autotag(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_remote_set_autotag(resource $remote, long $value)
 */
PHP_FUNCTION(git_remote_set_autotag)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	zend_long value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &remote, &value) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_remote_set_autotag(PHP_GIT2_V(_remote, remote), value);
}
/* }}} */

/* {{{ proto long git_remote_rename(resource $remote, string $new_name, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_remote_rename)
{
	int result;
	zval *remote = NULL, *payload = NULL;
	php_git2_t *_remote = NULL;
	char *new_name = NULL;
	size_t new_name_len;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsfz", &remote, &new_name, &new_name_len, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_remote_rename(PHP_GIT2_V(_remote, remote), new_name, php_git2_git_remote_rename_problem_cb, payload_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_remote_update_fetchhead(resource $remote)
 */
PHP_FUNCTION(git_remote_update_fetchhead)
{
	int result;
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &remote) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_remote_update_fetchhead(PHP_GIT2_V(_remote, remote));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_remote_set_update_fetchhead(resource $remote, long $value)
 */
PHP_FUNCTION(git_remote_set_update_fetchhead)
{
	zval *remote = NULL;
	php_git2_t *_remote = NULL;
	zend_long value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &remote, &value) == FAILURE) {
		return;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_remote_set_update_fetchhead(PHP_GIT2_V(_remote, remote), value);
}
/* }}} */

/* {{{ proto long git_remote_is_valid_name(string $remote_name)
 */
PHP_FUNCTION(git_remote_is_valid_name)
{
	int result;
	char *remote_name = NULL;
	size_t remote_name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &remote_name, &remote_name_len) == FAILURE) {
		return;
	}

	result = git_remote_is_valid_name(remote_name);

	RETURN_BOOL(result);
}
/* }}} */

