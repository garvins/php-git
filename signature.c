#include "php_git2.h"
#include "php_git2_priv.h"
#include "signature.h"

/* {{{ proto array git_signature_new(string $name, string $email, long $time, long $offset)
 */
PHP_FUNCTION(git_signature_new)
{
	zval *result;
	git_signature *out = NULL;
	char *name = NULL, *email = NULL;
	size_t name_len, email_len;
	zend_long time, offset;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ssll", &name, &name_len, &email, &email_len, &time, &offset) == FAILURE) {
		return;
	}

	error = git_signature_new(&out, name, email, time, offset);

	if (php_git2_check_error(error, "git_signature_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_signature_to_array(out, array TSRMLS_CC);
	git_signature_free(out);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto array git_signature_now(string $name, string $email)
 */
PHP_FUNCTION(git_signature_now)
{
	zval *result;
	git_signature *out = NULL;
	char *name = NULL, *email = NULL;
	size_t name_len, email_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &name, &name_len, &email, &email_len) == FAILURE) {
		return;
	}

	error = git_signature_now(&out, name, email);

	if (php_git2_check_error(error, "git_signature_now" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_signature_to_array(out, array TSRMLS_CC);
	git_signature_free(out);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto array git_signature_default(resource $repo)
 */
PHP_FUNCTION(git_signature_default)
{
	zval *result, *repo = NULL;
	git_signature *out = NULL;
	php_git2_t *_repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_signature_default(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_signature_default" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_signature_to_array(out, array TSRMLS_CC);
	git_signature_free(out);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

