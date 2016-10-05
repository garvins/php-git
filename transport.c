#include "php_git2.h"
#include "php_git2_priv.h"
#include "transport.h"

/* {{{ proto resource git_transport_new(resource $owner, string $url)
 */
PHP_FUNCTION(git_transport_new)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_transport *out = NULL;
	zval *owner = NULL;
	char *url = NULL;
	size_t url_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &owner, &url, &url_len) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_transport_new(&out, PHP_GIT2_V(_owner, remote), url);

	if (php_git2_check_error(error, "git_transport_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_transport_register(string $prefix, long $priority, Callable $cb, void $param)
 */
PHP_FUNCTION(git_transport_register)
{
	int result;
	char *prefix = NULL;
	size_t prefix_len;
	zend_long priority;
	zend_fcall_info cb_fci = empty_fcall_info;
	zend_fcall_info_cache cb_fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb_cb = NULL;
	zval *param = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"slfz", &prefix, &prefix_len, &priority, &cb_fci, &cb_fcc, &param) == FAILURE) {
		return;
	}

	if (php_git2_cb_init(&cb_cb, &cb_fci, &cb_fcc, param TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_transport_register(prefix, priority, php_git2_git_transport_cb, cb_cb);
	php_git2_cb_free(cb_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_transport_unregister(string $prefix, long $priority)
 */
PHP_FUNCTION(git_transport_unregister)
{
	int result;
	char *prefix = NULL;
	size_t prefix_len;
	zend_long priority;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &prefix, &prefix_len, &priority) == FAILURE) {
		return;
	}

	result = git_transport_unregister(prefix, priority);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_transport_dummy(resource $owner, void $payload)
 */
PHP_FUNCTION(git_transport_dummy)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_transport *out = NULL;
	zval *owner = NULL, *payload = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rz", &owner, &payload) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_transport_dummy(&out, PHP_GIT2_V(_owner, remote), NULL);

	if (php_git2_check_error(error, "git_transport_dummy" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_transport_local(resource $owner, void $payload)
 */
PHP_FUNCTION(git_transport_local)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_transport *out = NULL;
	zval *owner = NULL, *payload = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rz", &owner, &payload) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_transport_local(&out, PHP_GIT2_V(_owner, remote), NULL);

	if (php_git2_check_error(error, "git_transport_local" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_transport_smart(resource $owner, void $payload)
 */
PHP_FUNCTION(git_transport_smart)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_transport *out = NULL;
	zval *owner = NULL, *payload = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rz", &owner, &payload) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_transport_smart(&out, PHP_GIT2_V(_owner, remote), payload);

	if (php_git2_check_error(error, "git_transport_smart" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_smart_subtransport_http(resource $owner)
 */
PHP_FUNCTION(git_smart_subtransport_http)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_smart_subtransport *out = NULL;
	zval *owner = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &owner) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_smart_subtransport_http(&out, PHP_GIT2_V(_owner, transport));

	if (php_git2_check_error(error, "git_smart_subtransport_http" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_SMART_SUBTRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_smart_subtransport_git(resource $owner)
 */
PHP_FUNCTION(git_smart_subtransport_git)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_smart_subtransport *out = NULL;
	zval *owner = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &owner) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_smart_subtransport_git(&out, PHP_GIT2_V(_owner, transport));

	if (php_git2_check_error(error, "git_smart_subtransport_git" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_SMART_SUBTRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_smart_subtransport_ssh(resource $owner)
 */
PHP_FUNCTION(git_smart_subtransport_ssh)
{
	php_git2_t *result = NULL, *_owner = NULL;
	git_smart_subtransport *out = NULL;
	zval *owner = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &owner) == FAILURE) {
		return;
	}

	if ((_owner = (php_git2_t *) zend_fetch_resource(Z_RES_P(owner), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_smart_subtransport_ssh(&out, PHP_GIT2_V(_owner, transport));

	if (php_git2_check_error(error, "git_smart_subtransport_ssh" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_SMART_SUBTRANSPORT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

