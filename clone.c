#include "php_git2.h"
#include "php_git2_priv.h"
#include "clone.h"

/* {{{ proto resource git_clone(string $url, string $local_path, array $options)
 */
PHP_FUNCTION(git_clone)
{
	php_git2_t *result = NULL;
	git_repository *out = NULL;
	char *url = NULL, *local_path = NULL;
	size_t url_len, local_path_len;
	git_clone_options *_options = NULL;
	zval *options = NULL;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss|a", &url, &url_len, &local_path, &local_path_len, &options) == FAILURE) {
		return;
	}

	if (options != NULL) {
		php_git2_array_to_git_clone_options(_options, options TSRMLS_CC);
		should_free = 1;
	}

	error = git_clone(&out, url, local_path, _options);

	if (php_git2_check_error(error, "git_clone" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_clone_options_free(_options TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REPOSITORY, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_clone_into(resource $repo, resource $remote, array $co_opts, string $branch)
 */
PHP_FUNCTION(git_clone_into)
{
	int result, should_free = 0;
	zval *repo = NULL, *remote = NULL, *co_opts = NULL;
	php_git2_t *_repo = NULL, *_remote = NULL;
	git_checkout_opts *_co_opts = NULL;
	char *branch = NULL;
	size_t branch_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|as", &repo, &remote, &co_opts, &branch, &branch_len) == FAILURE) {
		return;
	}

	if (co_opts != NULL) {
		php_git2_array_to_git_checkout_opts(_co_opts, co_opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_remote = (php_git2_t *) zend_fetch_resource(Z_RES_P(remote), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_clone_into(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_remote, remote), _co_opts, branch);

	if (should_free) {
		php_git2_git_checkout_opts_free(_co_opts TSRMLS_CC);
	}

	RETURN_LONG(result);
}
/* }}} */

