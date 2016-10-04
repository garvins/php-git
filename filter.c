#include "php_git2.h"
#include "php_git2_priv.h"
#include "filter.h"

/* {{{ proto resource git_filter_list_load(resource $repo, resource $blob, string $path, long $mode)
 */
PHP_FUNCTION(git_filter_list_load)
{
	php_git2_t *result = NULL, *_repo = NULL, *_blob = NULL;
	git_filter_list *filters = NULL;
	zval *repo = NULL, *blob = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long mode;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrsl", &repo, &blob, &path, &path_len, &mode) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_filter_list_load(&filters, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_blob, blob), path, mode);

	if (php_git2_check_error(error, "git_filter_list_load" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_FILTER_LIST, filters, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_filter_list_apply_to_data(resource $filters, string $in)
 */
PHP_FUNCTION(git_filter_list_apply_to_data)
{
	git_buf out = {0}, _in = {0};;
	zval *filters = NULL;
	php_git2_t *_filters = NULL;
	char *in = NULL;
	size_t in_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &filters, &in, &in_len) == FAILURE) {
		return;
	}

	if ((_filters = (php_git2_t *) zend_fetch_resource(Z_RES_P(filters), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_buf_set(&_in, in, in_len)) {
		RETURN_FALSE;
	}

	error = git_filter_list_apply_to_data(&out, PHP_GIT2_V(_filters, filter_list), &_in);

	if (php_git2_check_error(error, "git_filter_list_apply_to_data" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(git_buf_cstr(&out), git_buf_len(&out));
	git_buf_free(&out);
}
/* }}} */

/* {{{ proto string git_filter_list_apply_to_file(resource $filters, resource $repo, string $path)
 */
PHP_FUNCTION(git_filter_list_apply_to_file)
{
	git_buf out = {0};
	zval *filters = NULL, *repo = NULL;
	php_git2_t *_filters = NULL, *_repo = NULL;
	char *path = NULL;
	size_t path_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrs", &filters, &repo, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_filters = (php_git2_t *) zend_fetch_resource(Z_RES_P(filters), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_filter_list_apply_to_file(&out, PHP_GIT2_V(_filters, filter_list), PHP_GIT2_V(_repo, repository), path);

	if (php_git2_check_error(error, "git_filter_list_apply_to_file" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(git_buf_cstr(&out), git_buf_len(&out));
	git_buf_free(&out);
}
/* }}} */

/* {{{ proto string git_filter_list_apply_to_blob(resource $filters, resource $blob)
 */
PHP_FUNCTION(git_filter_list_apply_to_blob)
{
	git_buf out = {0};
	zval *filters = NULL, *blob = NULL;
	php_git2_t *_filters = NULL, *_blob = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &filters, &blob) == FAILURE) {
		return;
	}

	if ((_filters = (php_git2_t *) zend_fetch_resource(Z_RES_P(filters), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_filter_list_apply_to_blob(&out, PHP_GIT2_V(_filters, filter_list), PHP_GIT2_V(_blob, blob));

	if (php_git2_check_error(error, "git_filter_list_apply_to_blob" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(git_buf_cstr(&out), git_buf_len(&out));
	git_buf_free(&out);
}
/* }}} */

/* {{{ proto void git_filter_list_free(resource $filters)
 */
PHP_FUNCTION(git_filter_list_free)
{
	zval *filters = NULL;
	php_git2_t *_filters = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &filters) == FAILURE) {
		return;
	}

	if ((_filters = (php_git2_t *) zend_fetch_resource(Z_RES_P(filters), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_filters)) {
		git_filter_list_free(PHP_GIT2_V(_filters, filter_list));
		GIT2_SHOULD_FREE(_filters) = 0;
	);

	zval_ptr_dtor(&filters);
}
/* }}} */

/* {{{ proto resource git_filter_lookup(string $name)
 */
PHP_FUNCTION(git_filter_lookup)
{
	git_filter *result = NULL;
	char *name = NULL;
	size_t name_len;
	php_git2_t *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &name, &name_len) == FAILURE) {
		return;
	}

	result = git_filter_lookup(name);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_FILTER, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_filter_list_new(resource $repo, long $mode)
 */
PHP_FUNCTION(git_filter_list_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_filter_list *out = NULL;
	zval *repo = NULL;
	zend_long mode;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &repo, &mode) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_filter_list_new(&out, PHP_GIT2_V(_repo, repository), mode);

	if (php_git2_check_error(error, "git_filter_list_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_FILTER_LIST, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_filter_list_push(resource $fl, resource $filter, void $payload)
 */
PHP_FUNCTION(git_filter_list_push)
{
	int result;
	zval *fl = NULL, *filter = NULL, *payload = NULL;
	php_git2_t *_fl = NULL, *_filter = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrz", &fl, &filter, &payload) == FAILURE) {
		return;
	}

	if ((_fl = (php_git2_t *) zend_fetch_resource(Z_RES_P(fl), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_filter = (php_git2_t *) zend_fetch_resource(Z_RES_P(filter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_list_push(PHP_GIT2_V(_fl, filter_list), PHP_GIT2_V(_filter, filter), payload);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_filter_list_length(resource $fl)
 */
PHP_FUNCTION(git_filter_list_length)
{
	size_t result;
	zval *fl = NULL;
	php_git2_t *_fl = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &fl) == FAILURE) {
		return;
	}

	if ((_fl = (php_git2_t *) zend_fetch_resource(Z_RES_P(fl), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_list_length(PHP_GIT2_V(_fl, filter_list));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_filter_source_repo(resource $src)
 */
PHP_FUNCTION(git_filter_source_repo)
{
	git_repository *result = NULL;
	zval *src = NULL;
	php_git2_t *_src = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &src) == FAILURE) {
		return;
	}

	if ((_src = (php_git2_t *) zend_fetch_resource(Z_RES_P(src), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_source_repo(PHP_GIT2_V(_src, filter_source));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto string git_filter_source_path(resource $src)
 */
PHP_FUNCTION(git_filter_source_path)
{
	const char *result = NULL;
	zval *src = NULL;
	php_git2_t *_src = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &src) == FAILURE) {
		return;
	}

	if ((_src = (php_git2_t *) zend_fetch_resource(Z_RES_P(src), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_source_path(PHP_GIT2_V(_src, filter_source));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_filter_source_filemode(resource $src)
 */
PHP_FUNCTION(git_filter_source_filemode)
{
	uint16_t result;
	zval *src = NULL;
	php_git2_t *_src = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &src) == FAILURE) {
		return;
	}

	if ((_src = (php_git2_t *) zend_fetch_resource(Z_RES_P(src), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_source_filemode(PHP_GIT2_V(_src, filter_source));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_filter_source_id(resource $src)
 */
PHP_FUNCTION(git_filter_source_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *src = NULL;
	php_git2_t *_src = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &src) == FAILURE) {
		return;
	}

	if ((_src = (php_git2_t *) zend_fetch_resource(Z_RES_P(src), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_source_id(PHP_GIT2_V(_src, filter_source));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_filter_source_mode(resource $src)
 */
PHP_FUNCTION(git_filter_source_mode)
{
	git_filter_mode_t result;
	zval *src = NULL;
	php_git2_t *_src = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &src) == FAILURE) {
		return;
	}

	if ((_src = (php_git2_t *) zend_fetch_resource(Z_RES_P(src), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_source_mode(PHP_GIT2_V(_src, filter_source));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_filter_register(string $name, resource $filter, long $priority)
 */
PHP_FUNCTION(git_filter_register)
{
	int result;
	char *name = NULL;
	size_t name_len;
	zval *filter = NULL;
	php_git2_t *_filter = NULL;
	zend_long priority;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"srl", &name, &name_len, &filter, &priority) == FAILURE) {
		return;
	}

	if ((_filter = (php_git2_t *) zend_fetch_resource(Z_RES_P(filter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_filter_register(name, PHP_GIT2_V(_filter, filter), priority);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_filter_unregister(string $name)
 */
PHP_FUNCTION(git_filter_unregister)
{
	int result;
	char *name = NULL;
	size_t name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &name, &name_len) == FAILURE) {
		return;
	}

	result = git_filter_unregister(name);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_filter_new()
 */
PHP_FUNCTION(git_filter_new)
{
	git_filter *filter = NULL;
	php_git2_t *result = NULL;
	php_git2_filter *_filter = NULL;
	zval *mixed = NULL, *tmp = NULL;
	zend_fcall_info initialize_fci, shutdown_fci, check_fci, apply_fci, cleanup_fci;
	zend_fcall_info_cache initialize_fcc, shutdown_fcc, check_fcc, apply_fcc, cleanup_fcc;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"a", &mixed) == FAILURE) {
		return;
	}

	_filter = (php_git2_filter*)ecalloc(1, sizeof(php_git2_filter));
	filter = (git_filter*)&_filter->super;
	filter->version = GIT_FILTER_VERSION;
	filter->attributes = php_git2_read_arrval_string(mixed, ZEND_STRS("attributes") TSRMLS_CC);

	if (tmp = php_git2_read_arrval(mixed, ZEND_STRS("initialize") TSRMLS_CC)) {
		php_git2_fcall_info_wrapper2(tmp, &initialize_fci, &initialize_fcc TSRMLS_CC);
	}

	if (tmp = php_git2_read_arrval(mixed, ZEND_STRS("shutdown") TSRMLS_CC)) {
		php_git2_fcall_info_wrapper2(tmp, &shutdown_fci, &shutdown_fcc TSRMLS_CC);
	}

	if (tmp = php_git2_read_arrval(mixed, ZEND_STRS("check") TSRMLS_CC)) {
		php_git2_fcall_info_wrapper2(tmp, &check_fci, &check_fcc TSRMLS_CC);
	}

	if (tmp = php_git2_read_arrval(mixed, ZEND_STRS("apply") TSRMLS_CC)) {
		php_git2_fcall_info_wrapper2(tmp, &apply_fci, &apply_fcc TSRMLS_CC);
	}

	if (tmp = php_git2_read_arrval(mixed, ZEND_STRS("cleanup") TSRMLS_CC)) {
		php_git2_fcall_info_wrapper2(tmp, &cleanup_fci, &cleanup_fcc TSRMLS_CC);
	}

	filter->initialize = php_git2_git_filter_init_fn;
	filter->shutdown = php_git2_git_filter_shutdown_fn;
	filter->check = php_git2_git_filter_check_fn;
	filter->apply = php_git2_git_filter_apply_fn;
	filter->cleanup = php_git2_git_filter_cleanup_fn;

	Z_ADDREF_P(mixed);
	php_git2_multi_cb_init(&_filter->multi, mixed TSRMLS_CC, 5,
		&initialize_fci, &initialize_fcc,
		&shutdown_fci, &shutdown_fcc,
		&check_fci, &check_fcc,
		&apply_fci, &apply_fcc,
		&cleanup_fci, &cleanup_fcc
	);

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_FILTER, filter, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

