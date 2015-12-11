#include "php_git2.h"
#include "php_git2_priv.h"
#include "checkout.h"

/* {{{ proto long git_checkout_head(resource $repo,  $opts)
 */
PHP_FUNCTION(git_checkout_head)
{
	zval *opts = NULL, *repo = NULL;
	php_git2_t *_repo = NULL;
	int result = 0, should_free = 0;
	git_checkout_opts *options = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r|a", &repo, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		if (php_git2_array_to_git_checkout_opts(&options, opts TSRMLS_CC)) {
			RETURN_FALSE;
		}
		should_free = 1;
	} else {
		//memset(&options, '\0', sizeof(git_checkout_opts));
	}

	ZEND_FETCH_RESOURCE(_repo, php_git2_t*, &repo, -1, PHP_GIT2_RESOURCE_NAME, git2_resource_handle);
	result = git_checkout_head(PHP_GIT2_V(_repo, repository), options);
	if (should_free) {
		php_git_git_checkout_opts_free(options TSRMLS_CC);
	}
	RETURN_LONG(result);
}
/* }}} */


/* {{{ proto long git_checkout_index(resource $repo, resource $index,  $opts)
 */
PHP_FUNCTION(git_checkout_index)
{
	int result = 0, should_free = 0;
	zval *repo = NULL, *index = NULL, *opts = NULL;
    php_git2_t *_repo = NULL, *_index = NULL;
    git_checkout_opts *options = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rra", &repo, &index, &opts) == FAILURE) {
		return;
    }
    
    if (opts != NULL) {
        if (php_git2_array_to_git_checkout_opts(&options, opts TSRMLS_CC)) {
            RETURN_FALSE;
        }
        should_free = 1;
    }

	ZEND_FETCH_RESOURCE(_repo, php_git2_t*, &repo, -1, PHP_GIT2_RESOURCE_NAME, git2_resource_handle);
	ZEND_FETCH_RESOURCE(_index, php_git2_t*, &index, -1, PHP_GIT2_RESOURCE_NAME, git2_resource_handle);
    result = git_checkout_index(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_index, index), options);
    if (should_free) {
        php_git_git_checkout_opts_free(options TSRMLS_CC);
    }
	RETURN_LONG(result);
}
/* }}} */


/* {{{ proto long git_checkout_tree(resource $repo, resource $treeish,  $opts)
 */
PHP_FUNCTION(git_checkout_tree)
{
	int result = 0, should_free = 0;
	zval *repo = NULL, *treeish = NULL, *opts = NULL;
    php_git2_t *_repo = NULL, *_treeish = NULL;
    git_checkout_opts *options = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rra", &repo, &treeish, &opts) == FAILURE) {
		return;
	}
    
    if (opts != NULL) {
        if (php_git2_array_to_git_checkout_opts(&options, opts TSRMLS_CC)) {
            RETURN_FALSE;
        }
        should_free = 1;
    }

	ZEND_FETCH_RESOURCE(_repo, php_git2_t*, &repo, -1, PHP_GIT2_RESOURCE_NAME, git2_resource_handle);
	ZEND_FETCH_RESOURCE(_treeish, php_git2_t*, &treeish, -1, PHP_GIT2_RESOURCE_NAME, git2_resource_handle);
    result = git_checkout_tree(PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_treeish, object), options);
    if (should_free) {
        php_git_git_checkout_opts_free(options TSRMLS_CC);
    }
	RETURN_LONG(result);
}
/* }}} */


