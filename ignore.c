#include "php_git2.h"
#include "php_git2_priv.h"
#include "ignore.h"

/* {{{ proto long git_ignore_add_rule(resource $repo, string $rules)
 */
PHP_FUNCTION(git_ignore_add_rule)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *rules = NULL;
	size_t rules_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &rules, &rules_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_ignore_add_rule(PHP_GIT2_V(_repo, repository), rules);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_ignore_clear_internal_rules(resource $repo)
 */
PHP_FUNCTION(git_ignore_clear_internal_rules)
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

	result = git_ignore_clear_internal_rules(PHP_GIT2_V(_repo, repository));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_ignore_path_is_ignored(resource $repo, string $path)
 */
PHP_FUNCTION(git_ignore_path_is_ignored)
{
	int ignored = 0, error;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *path = NULL;
	size_t path_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_ignore_path_is_ignored(&ignored, PHP_GIT2_V(_repo, repository), path);

	if (php_git2_check_error(error, "git_ignore_path_is_ignored" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_BOOL(ignored);
}
/* }}} */

