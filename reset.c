#include "php_git2.h"
#include "php_git2_priv.h"
#include "reset.h"

/* {{{ proto long git_reset(resource $repo, resource $target, long $reset_type)
 */
PHP_FUNCTION(git_reset)
{
	int result;
	zval *repo = NULL, *target = NULL;
	php_git2_t *_repo = NULL, *_target = NULL;
	zend_long reset_type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrl", &repo, &target, &reset_type) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_target = (php_git2_t *) zend_fetch_resource(Z_RES_P(target), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reset(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_target, object), reset_type);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reset_default(resource $repo, resource $target, array $pathspecs)
 */
PHP_FUNCTION(git_reset_default)
{
	int result;
	zval *repo = NULL, *target = NULL, *pathspecs = NULL;
	php_git2_t *_repo = NULL, *_target = NULL;
	git_strarray _pathspecs = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rra", &repo, &target, &pathspecs) == FAILURE) {
		return;
	}

	php_git2_array_to_git_strarray(&_pathspecs, pathspecs TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_target = (php_git2_t *) zend_fetch_resource(Z_RES_P(target), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reset_default(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_target, object), &_pathspecs);
	git_strarray_free(&_pathspecs);

	RETURN_LONG(result);
}
/* }}} */

