#include "php_git2.h"
#include "php_git2_priv.h"
#include "g_config.h"

/* {{{ proto string git_config_find_global()
 */
PHP_FUNCTION(git_config_find_global)
{
	char out[GIT2_BUFFER_SIZE] = {0};
	size_t length = GIT2_BUFFER_SIZE;
	int error;
	
	error = git_config_find_global(out, length);

	if (php_git2_check_error(error, "git_config_find_global" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto string git_config_find_xdg()
 */
PHP_FUNCTION(git_config_find_xdg)
{
	char out[GIT2_BUFFER_SIZE] = {0};
	size_t length = GIT2_BUFFER_SIZE;
	int error;
	
	error = git_config_find_xdg(out, length);

	if (php_git2_check_error(error, "git_config_find_xdg" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto string git_config_find_system()
 */
PHP_FUNCTION(git_config_find_system)
{
	char out[GIT2_BUFFER_SIZE] = {0};
	size_t length = GIT2_BUFFER_SIZE;
	int error;
	
	error = git_config_find_system(out, length);

	if (php_git2_check_error(error, "git_config_find_system" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto resource git_config_open_default()
 */
PHP_FUNCTION(git_config_open_default)
{
	php_git2_t *result = NULL;
	git_config *out = NULL;
	int error;
	
	error = git_config_open_default(&out);

	if (php_git2_check_error(error, "git_config_open_default" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_config_new()
 */
PHP_FUNCTION(git_config_new)
{
	php_git2_t *result = NULL;
	git_config *out = NULL;
	int error;
	
	error = git_config_new(&out);

	if (php_git2_check_error(error, "git_config_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_config_add_file_ondisk(resource $cfg, string $path, long $level, long $force)
 */
PHP_FUNCTION(git_config_add_file_ondisk)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long level, force;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsll", &cfg, &path, &path_len, &level, &force) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_add_file_ondisk(PHP_GIT2_V(_cfg, config), path, level, force);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_config_open_ondisk(string $path)
 */
PHP_FUNCTION(git_config_open_ondisk)
{
	php_git2_t *result = NULL;
	git_config *out = NULL;
	char *path = NULL;
	size_t path_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &path, &path_len) == FAILURE) {
		return;
	}

	error = git_config_open_ondisk(&out, path);

	if (php_git2_check_error(error, "git_config_open_ondisk" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_config_open_level(resource $parent, long $level)
 */
PHP_FUNCTION(git_config_open_level)
{
	php_git2_t *result = NULL, *_parent = NULL;
	git_config *out = NULL;
	zval *parent = NULL;
	zend_long level;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &parent, &level) == FAILURE) {
		return;
	}

	if ((_parent = (php_git2_t *) zend_fetch_resource(Z_RES_P(parent), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_open_level(&out, PHP_GIT2_V(_parent, config), level);

	if (php_git2_check_error(error, "git_config_open_level" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_config_open_global(resource $config)
 */
PHP_FUNCTION(git_config_open_global)
{
	php_git2_t *result = NULL, *_config = NULL;
	git_config *out = NULL;
	zval *config = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &config) == FAILURE) {
		return;
	}

	if ((_config = (php_git2_t *) zend_fetch_resource(Z_RES_P(config), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_open_global(&out, PHP_GIT2_V(_config, config));

	if (php_git2_check_error(error, "git_config_open_global" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_config_refresh(resource $cfg)
 */
PHP_FUNCTION(git_config_refresh)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &cfg) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_refresh(PHP_GIT2_V(_cfg, config));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_config_free(resource $cfg)
 */
PHP_FUNCTION(git_config_free)
{
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &cfg) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_cfg) */) {
		git_config_free(PHP_GIT2_V(_cfg, config));
		GIT2_SHOULD_FREE(_cfg) = 0;
	}

	//zval_ptr_dtor(cfg);
}
/* }}} */

/* {{{ proto array git_config_get_entry(resource $cfg, string $name)
 */
PHP_FUNCTION(git_config_get_entry)
{
	zval *array, *cfg = NULL;
	const git_config_entry *out = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_get_entry(&out, PHP_GIT2_V(_cfg, config), name);

	if (php_git2_check_error(error, "git_config_get_entry" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (out == NULL) {
		RETURN_FALSE;
	}

	php_git2_git_config_entry_to_array(out, array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_config_get_int32(resource $cfg, string $name)
 */
PHP_FUNCTION(git_config_get_int32)
{
	int32_t out = 0;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_get_int32(&out, PHP_GIT2_V(_cfg, config), name);

	if (php_git2_check_error(error, "git_config_get_int32" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_get_int64(resource $cfg, string $name)
 */
PHP_FUNCTION(git_config_get_int64)
{
	int64_t out = 0;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_get_int64(&out, PHP_GIT2_V(_cfg, config), name);

	if (php_git2_check_error(error, "git_config_get_int64" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_get_bool(resource $cfg, string $name)
 */
PHP_FUNCTION(git_config_get_bool)
{
	int out = 0, error;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_get_bool(&out, PHP_GIT2_V(_cfg, config), name);

	if (php_git2_check_error(error, "git_config_get_bool" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto string git_config_get_string(resource $cfg, string $name)
 */
PHP_FUNCTION(git_config_get_string)
{
	const char *out = NULL;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_get_string(&out, PHP_GIT2_V(_cfg, config), name);

	if (php_git2_check_error(error, "git_config_get_string" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto long git_config_get_multivar_foreach(resource $cfg, string $name, string $regexp, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_config_get_multivar_foreach)
{
	int result;
	zval *cfg = NULL, *payload = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL, *regexp = NULL;
	size_t name_len, regexp_len;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rssfz", &cfg, &name, &name_len, &regexp, &regexp_len, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_config_get_multivar_foreach(PHP_GIT2_V(_cfg, config), name, regexp, php_git2_git_config_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_config_multivar_iterator_new(resource $cfg, string $name, string $regexp)
 */
PHP_FUNCTION(git_config_multivar_iterator_new)
{
	php_git2_t *result = NULL, *_cfg = NULL;
	git_config_iterator *out = NULL;
	zval *cfg = NULL;
	char *name = NULL, *regexp = NULL;
	size_t name_len, regexp_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &cfg, &name, &name_len, &regexp, &regexp_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_multivar_iterator_new(&out, PHP_GIT2_V(_cfg, config), name, regexp);

	if (php_git2_check_error(error, "git_config_multivar_iterator_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG_ITERATOR, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto array git_config_next(resource $iter)
 */
PHP_FUNCTION(git_config_next)
{
	zval *array, *iter = NULL;
	git_config_entry *entry = NULL;
	php_git2_t *_iter = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iter) == FAILURE) {
		return;
	}

	if ((_iter = (php_git2_t *) zend_fetch_resource(Z_RES_P(iter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_next(&entry, PHP_GIT2_V(_iter, config_iterator));

	if (php_git2_check_error(error, "git_config_next" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (entry == NULL) {
		RETURN_FALSE;
	}

	php_git2_git_config_entry_to_array(entry, array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto void git_config_iterator_free(resource $iter)
 */
PHP_FUNCTION(git_config_iterator_free)
{
	zval *iter = NULL;
	php_git2_t *_iter = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iter) == FAILURE) {
		return;
	}

	if ((_iter = (php_git2_t *) zend_fetch_resource(Z_RES_P(iter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_iter) */) {
		git_config_iterator_free(PHP_GIT2_V(_iter, config_iterator));
		GIT2_SHOULD_FREE(_iter) = 0;
	}

	//zval_ptr_dtor(iter);
}
/* }}} */

/* {{{ proto long git_config_set_int32(resource $cfg, string $name, long $value)
 */
PHP_FUNCTION(git_config_set_int32)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	zend_long value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &cfg, &name, &name_len, &value) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_set_int32(PHP_GIT2_V(_cfg, config), name, value);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_set_int64(resource $cfg, string $name, long $value)
 */
PHP_FUNCTION(git_config_set_int64)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	zend_long value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &cfg, &name, &name_len, &value) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_set_int64(PHP_GIT2_V(_cfg, config), name, value);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_set_bool(resource $cfg, string $name, long $value)
 */
PHP_FUNCTION(git_config_set_bool)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	zend_long value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &cfg, &name, &name_len, &value) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_set_bool(PHP_GIT2_V(_cfg, config), name, value);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_set_string(resource $cfg, string $name, string $value)
 */
PHP_FUNCTION(git_config_set_string)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL, *value = NULL;
	size_t name_len, value_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &cfg, &name, &name_len, &value, &value_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_set_string(PHP_GIT2_V(_cfg, config), name, value);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_set_multivar(resource $cfg, string $name, string $regexp, string $value)
 */
PHP_FUNCTION(git_config_set_multivar)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL, *regexp = NULL, *value = NULL;
	size_t name_len, regexp_len, value_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsss", &cfg, &name, &name_len, &regexp, &regexp_len, &value, &value_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_set_multivar(PHP_GIT2_V(_cfg, config), name, regexp, value);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_delete_entry(resource $cfg, string $name)
 */
PHP_FUNCTION(git_config_delete_entry)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_delete_entry(PHP_GIT2_V(_cfg, config), name);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_delete_multivar(resource $cfg, string $name, string $regexp)
 */
PHP_FUNCTION(git_config_delete_multivar)
{
	int result;
	zval *cfg = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL, *regexp = NULL;
	size_t name_len, regexp_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &cfg, &name, &name_len, &regexp, &regexp_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_config_delete_multivar(PHP_GIT2_V(_cfg, config), name, regexp);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_foreach(resource $cfg, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_config_foreach)
{
	int result;
	zval *cfg = NULL, *payload = NULL;
	php_git2_t *_cfg = NULL;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &cfg, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_config_foreach(PHP_GIT2_V(_cfg, config), php_git2_git_config_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_config_iterator_new(resource $cfg)
 */
PHP_FUNCTION(git_config_iterator_new)
{
	php_git2_t *result = NULL, *_cfg = NULL;
	git_config_iterator *out = NULL;
	zval *cfg = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &cfg) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_iterator_new(&out, PHP_GIT2_V(_cfg, config));

	if (php_git2_check_error(error, "git_config_iterator_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG_ITERATOR, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_config_iterator_glob_new(resource $cfg, string $regexp)
 */
PHP_FUNCTION(git_config_iterator_glob_new)
{
	php_git2_t *result = NULL, *_cfg = NULL;
	git_config_iterator *out = NULL;
	zval *cfg = NULL;
	char *regexp = NULL;
	size_t regexp_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &cfg, &regexp, &regexp_len) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_iterator_glob_new(&out, PHP_GIT2_V(_cfg, config), regexp);

	if (php_git2_check_error(error, "git_config_iterator_glob_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CONFIG_ITERATOR, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_config_foreach_match(resource $cfg, string $regexp, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_config_foreach_match)
{
	int result;
	zval *cfg = NULL, *payload = NULL;
	php_git2_t *_cfg = NULL;
	char *regexp = NULL;
	size_t regexp_len;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsfz", &cfg, &regexp, &regexp_len, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_config_foreach_match(PHP_GIT2_V(_cfg, config), regexp, php_git2_git_config_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_config_get_mapped(resource $cfg, string $name, array $maps, long $map_n)
 */
PHP_FUNCTION(git_config_get_mapped)
{
	int out = 0, error;
	zval *cfg = NULL, *maps = NULL;
	php_git2_t *_cfg = NULL;
	char *name = NULL;
	size_t name_len;
	git_cvar_map _maps = {0};
	zend_long map_n;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsal", &cfg, &name, &name_len, &maps, &map_n) == FAILURE) {
		return;
	}

	php_git2_array_to_git_cvar_map(&_maps, maps TSRMLS_CC);

	if ((_cfg = (php_git2_t *) zend_fetch_resource(Z_RES_P(cfg), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_config_get_mapped(&out, PHP_GIT2_V(_cfg, config), name, &_maps, map_n);

	if (php_git2_check_error(error, "git_config_get_mapped" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_lookup_map_value(array $maps, long $map_n, string $value)
 */
PHP_FUNCTION(git_config_lookup_map_value)
{
	int out = 0, error;
	zval *maps = NULL;
	git_cvar_map _maps = {0};
	zend_long map_n;
	char *value = NULL;
	size_t value_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"als", &maps, &map_n, &value, &value_len) == FAILURE) {
		return;
	}

	php_git2_array_to_git_cvar_map(&_maps, maps TSRMLS_CC);

	error = git_config_lookup_map_value(&out, &_maps, map_n, value);

	if (php_git2_check_error(error, "git_config_lookup_map_value" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_parse_bool(string $value)
 */
PHP_FUNCTION(git_config_parse_bool)
{
	int out = 0, error;
	char *value = NULL;
	size_t value_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &value, &value_len) == FAILURE) {
		return;
	}

	error = git_config_parse_bool(&out, value);

	if (php_git2_check_error(error, "git_config_parse_bool" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_parse_int32(string $value)
 */
PHP_FUNCTION(git_config_parse_int32)
{
	int32_t out = 0;
	char *value = NULL;
	size_t value_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &value, &value_len) == FAILURE) {
		return;
	}

	error = git_config_parse_int32(&out, value);

	if (php_git2_check_error(error, "git_config_parse_int32" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_parse_int64(string $value)
 */
PHP_FUNCTION(git_config_parse_int64)
{
	int64_t out = 0;
	char *value = NULL;
	size_t value_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &value, &value_len) == FAILURE) {
		return;
	}

	error = git_config_parse_int64(&out, value);

	if (php_git2_check_error(error, "git_config_parse_int64" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_LONG(out);
}
/* }}} */

/* {{{ proto long git_config_backend_foreach_match(resource $backend, string $regexp, Callable $cb, void $data)
 */
PHP_FUNCTION(git_config_backend_foreach_match)
{
	int result;
	zval *backend = NULL, *data = NULL;
	php_git2_t *_backend = NULL;
	char *regexp = NULL;
	size_t regexp_len;
	zend_fcall_info cb_fci = empty_fcall_info;
	zend_fcall_info_cache cb_fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsfz", &backend, &regexp, &regexp_len, &cb_fci, &cb_fcc, &data) == FAILURE) {
		return;
	}

	if ((_backend = (php_git2_t *) zend_fetch_resource(Z_RES_P(backend), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb_cb, &cb_fci, &cb_fcc, data TSRMLS_CC)) {
		RETURN_FALSE;
	}

	//result = git_config_backend_foreach_match(PHP_GIT2_V(_backend, config_backend), regexp, php_git2_git_config_backend_foreach_match_cb, cb_cb);
	php_git2_cb_free(cb_cb);

	RETURN_LONG(result);
}
/* }}} */

