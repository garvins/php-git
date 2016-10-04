#include "php_git2.h"
#include "php_git2_priv.h"
#include "cred.h"

/* {{{ proto long git_cred_has_username(resource $cred)
 */
PHP_FUNCTION(git_cred_has_username)
{
	int result;
	zval *cred = NULL;
	php_git2_t *_cred = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &cred) == FAILURE) {
		return;
	}

	if ((_cred = (php_git2_t *) zend_fetch_resource(Z_RES_P(cred), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_cred_has_username(PHP_GIT2_V(_cred, cred));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto resource git_cred_userpass_plaintext_new(string $username, string $password)
 */
PHP_FUNCTION(git_cred_userpass_plaintext_new)
{
	php_git2_t *result = NULL;
	git_cred *out = NULL;
	char *username = NULL, *password = NULL;
	size_t username_len, password_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &username, &username_len, &password, &password_len) == FAILURE) {
		return;
	}

	error = git_cred_userpass_plaintext_new(&out, username, password);

	if (php_git2_check_error(error, "git_cred_userpass_plaintext_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CRED, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_cred_ssh_key_new(string $username, string $publickey, string $privatekey, string $passphrase)
 */
PHP_FUNCTION(git_cred_ssh_key_new)
{
	php_git2_t *result = NULL;
	git_cred *out = NULL;
	char *username = NULL, *publickey = NULL, *privatekey = NULL, *passphrase = NULL;
	size_t username_len, publickey_len, privatekey_len, passphrase_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ssss", &username, &username_len, &publickey, &publickey_len, &privatekey, &privatekey_len, &passphrase, &passphrase_len) == FAILURE) {
		return;
	}

	error = git_cred_ssh_key_new(&out, username, publickey, privatekey, passphrase);

	if (php_git2_check_error(error, "git_cred_ssh_key_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CRED, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_cred_ssh_custom_new(string $username, string $publickey, Callable $sign_fn, void $sign_data)
 */
PHP_FUNCTION(git_cred_ssh_custom_new)
{
	php_git2_t *result = NULL;
	git_cred *out = NULL;
	char *username = NULL, *publickey = NULL;
	size_t username_len, publickey_len;
	zend_fcall_info sign_fci = empty_fcall_info;
	zend_fcall_info_cache sign_fcc = empty_fcall_info_cache;
	php_git2_cb_t *sign_cb = NULL;
	zval *sign_data = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sslfz", &username, &username_len, &publickey, &publickey_len, &sign_fci, &sign_fcc, &sign_data) == FAILURE) {
		return;
	}

	if (php_git2_cb_init(&sign_cb, &sign_fci, &sign_fcc, sign_data TSRMLS_CC)) {
		RETURN_FALSE;
	}

	error = git_cred_ssh_custom_new(&out, username, publickey, publickey_len, php_git2_git_cred_sign_callback, sign_cb);

	if (php_git2_check_error(error, "git_cred_ssh_custom_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	php_git2_cb_free(sign_cb);

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CRED, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_cred_default_new()
 */
PHP_FUNCTION(git_cred_default_new)
{
	php_git2_t *result = NULL;
	git_cred *out = NULL;
	int error = 0;
	
	error = git_cred_default_new(&out);

	if (php_git2_check_error(error, "git_cred_default_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_CRED, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

