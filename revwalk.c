#include "php_git2.h"
#include "php_git2_priv.h"
#include "revwalk.h"

/* {{{ proto resource git_revwalk_new(resource $repo)
 */
PHP_FUNCTION(git_revwalk_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_revwalk *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_revwalk_new(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_revwalk_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REVWALK, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_revwalk_reset(resource $walker)
 */
PHP_FUNCTION(git_revwalk_reset)
{
	zval *walker = NULL;
	php_git2_t *_walker = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walker) == FAILURE) {
		return;
	}

	if ((_walker = (php_git2_t *) zend_fetch_resource(Z_RES_P(walker), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_revwalk_reset(PHP_GIT2_V(_walker, revwalk));
}
/* }}} */

/* {{{ proto long git_revwalk_push(resource $walk, string $id)
 */
PHP_FUNCTION(git_revwalk_push)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &id, &id_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	result = git_revwalk_push(PHP_GIT2_V(_walk, revwalk), &__id);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_push_glob(resource $walk, string $glob)
 */
PHP_FUNCTION(git_revwalk_push_glob)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *glob = NULL;
	size_t glob_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &glob, &glob_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_push_glob(PHP_GIT2_V(_walk, revwalk), glob);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_push_head(resource $walk)
 */
PHP_FUNCTION(git_revwalk_push_head)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walk) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_push_head(PHP_GIT2_V(_walk, revwalk));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_hide(resource $walk, string $commit_id)
 */
PHP_FUNCTION(git_revwalk_hide)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *commit_id = NULL;
	size_t commit_id_len;
	git_oid __commit_id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &commit_id, &commit_id_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__commit_id, commit_id, commit_id_len)) {
		RETURN_FALSE;
	}

	result = git_revwalk_hide(PHP_GIT2_V(_walk, revwalk), &__commit_id);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_hide_glob(resource $walk, string $glob)
 */
PHP_FUNCTION(git_revwalk_hide_glob)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *glob = NULL;
	size_t glob_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &glob, &glob_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_hide_glob(PHP_GIT2_V(_walk, revwalk), glob);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_hide_head(resource $walk)
 */
PHP_FUNCTION(git_revwalk_hide_head)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walk) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_hide_head(PHP_GIT2_V(_walk, revwalk));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_push_ref(resource $walk, string $refname)
 */
PHP_FUNCTION(git_revwalk_push_ref)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *refname = NULL;
	size_t refname_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &refname, &refname_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_push_ref(PHP_GIT2_V(_walk, revwalk), refname);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_revwalk_hide_ref(resource $walk, string $refname)
 */
PHP_FUNCTION(git_revwalk_hide_ref)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *refname = NULL;
	size_t refname_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &refname, &refname_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_hide_ref(PHP_GIT2_V(_walk, revwalk), refname);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_revwalk_next(resource $walk)
 */
PHP_FUNCTION(git_revwalk_next)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walk) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_revwalk_next(&out, PHP_GIT2_V(_walk, revwalk));

	if (php_git2_check_error(error, "git_revwalk_next" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto void git_revwalk_sorting(resource $walk, long $sort_mode)
 */
PHP_FUNCTION(git_revwalk_sorting)
{
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	zend_long sort_mode;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &walk, &sort_mode) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_revwalk_sorting(PHP_GIT2_V(_walk, revwalk), sort_mode);
}
/* }}} */

/* {{{ proto long git_revwalk_push_range(resource $walk, string $range)
 */
PHP_FUNCTION(git_revwalk_push_range)
{
	int result;
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	char *range = NULL;
	size_t range_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &walk, &range, &range_len) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_push_range(PHP_GIT2_V(_walk, revwalk), range);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_revwalk_simplify_first_parent(resource $walk)
 */
PHP_FUNCTION(git_revwalk_simplify_first_parent)
{
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walk) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	git_revwalk_simplify_first_parent(PHP_GIT2_V(_walk, revwalk));
}
/* }}} */

/* {{{ proto void git_revwalk_free(resource $walk)
 */
PHP_FUNCTION(git_revwalk_free)
{
	zval *walk = NULL;
	php_git2_t *_walk = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walk) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_walk)) {
		git_revwalk_free(PHP_GIT2_V(_walk, revwalk));
		GIT2_SHOULD_FREE(_walk) = 0;
	}

	zval_ptr_dtor(&walk);
}
/* }}} */

/* {{{ proto resource git_revwalk_repository(resource $walk)
 */
PHP_FUNCTION(git_revwalk_repository)
{
	git_repository *result = NULL;
	zval *walk = NULL;
	php_git2_t *_walk = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &walk) == FAILURE) {
		return;
	}

	if ((_walk = (php_git2_t *) zend_fetch_resource(Z_RES_P(walk), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_revwalk_repository(PHP_GIT2_V(_walk, revwalk));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

