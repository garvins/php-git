#include "php_git2.h"
#include "php_git2_priv.h"
#include "merge.h"

/* {{{ proto string git_merge_base(resource $repo, string $one, string $two)
 */
PHP_FUNCTION(git_merge_base)
{
	git_oid out, __one = {0}, __two = {0};
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *one = NULL, *two = NULL;
	size_t one_len, two_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &repo, &one, &one_len, &two, &two_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__one, one, one_len)) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__two, two, two_len)) {
		RETURN_FALSE;
	}

	error = git_merge_base(&out, PHP_GIT2_V(_repo, repository), &__one, &__two);

	if (php_git2_check_error(error, "git_merge_base" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto string git_merge_base_many(resource $repo, long $length, string $input_array[])
 */
PHP_FUNCTION(git_merge_base_many)
{
	git_oid out, __input_array = {0};
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	zend_long length;
	char *input_array = NULL;
	size_t input_array_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rls", &repo, &length, &input_array, &input_array_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__input_array, input_array, input_array_len)) {
		RETURN_FALSE;
	}

	error = git_merge_base_many(&out, PHP_GIT2_V(_repo, repository), length, &__input_array);

	if (php_git2_check_error(error, "git_merge_base_many" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto resource git_merge_head_from_ref(resource $repo, resource $ref)
 */
PHP_FUNCTION(git_merge_head_from_ref)
{
	php_git2_t *result = NULL, *_repo = NULL, *_ref = NULL;
	git_merge_head *out = NULL;
	zval *repo = NULL, *ref = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &repo, &ref) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_merge_head_from_ref(&out, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_ref, reference));

	if (php_git2_check_error(error, "git_merge_head_from_ref" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_MERGE_HEAD, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_merge_head_from_fetchhead(resource $repo, string $branch_name, string $remote_url, string $oid)
 */
PHP_FUNCTION(git_merge_head_from_fetchhead)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_merge_head *out = NULL;
	zval *repo = NULL;
	char *branch_name = NULL, *remote_url = NULL, *oid = NULL;
	size_t branch_name_len, remote_url_len, oid_len;
	git_oid __oid = {0};
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsss", &repo, &branch_name, &branch_name_len, &remote_url, &remote_url_len, &oid, &oid_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	error = git_merge_head_from_fetchhead(&out, PHP_GIT2_V(_repo, repository), branch_name, remote_url, &__oid);

	if (php_git2_check_error(error, "git_merge_head_from_fetchhead" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_MERGE_HEAD, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_merge_head_from_oid(resource $repo, string $oid)
 */
PHP_FUNCTION(git_merge_head_from_oid)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_merge_head *out = NULL;
	zval *repo = NULL;
	char *oid = NULL;
	git_oid __oid = {0};
	size_t oid_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &oid, &oid_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	error = git_merge_head_from_oid(&out, PHP_GIT2_V(_repo, repository), &__oid);

	if (php_git2_check_error(error, "git_merge_head_from_oid" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_MERGE_HEAD, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_merge_head_free(resource $head)
 */
PHP_FUNCTION(git_merge_head_free)
{
	zval *head = NULL;
	php_git2_t *_head = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &head) == FAILURE) {
		return;
	}

	if ((_head = (php_git2_t *) zend_fetch_resource(Z_RES_P(head), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_head) */) {
		git_merge_head_free(PHP_GIT2_V(_head, merge_head));
		GIT2_SHOULD_FREE(_head) = 0;
	}

	//zval_ptr_dtor(head);
}
/* }}} */

/* {{{ proto resource git_merge_trees(resource $repo, resource $ancestor_tree, resource $our_tree, resource $their_tree, array $opts)
 */
PHP_FUNCTION(git_merge_trees)
{
	php_git2_t *result = NULL, *_repo = NULL, *_ancestor_tree = NULL, *_our_tree = NULL, *_their_tree = NULL;
	git_index *out = NULL;
	zval *repo = NULL, *ancestor_tree = NULL, *our_tree = NULL, *their_tree = NULL, *opts = NULL;
	git_merge_tree_opts _opts = {0};
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrrr|a", &repo, &ancestor_tree, &our_tree, &their_tree, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_merge_tree_opts(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_ancestor_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(ancestor_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_our_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(our_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_their_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(their_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_merge_trees(&out, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_ancestor_tree, tree), PHP_GIT2_V(_our_tree, tree), PHP_GIT2_V(_their_tree, tree), &_opts);

	if (php_git2_check_error(error, "git_merge_trees" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_merge_tree_opts_free(&_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_INDEX, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_merge(resource $repo, array $their_heads, array $opts)
 */
PHP_FUNCTION(git_merge)
{
	php_git2_t *result = NULL, *_repo = NULL, *_their_head = NULL;
	git_merge_result *out = NULL;
	zval *repo = NULL, *opts = NULL, *theirhead = NULL;
	const git_merge_head *heads[1];
	int error = 0;
	git_merge_opts options = GIT_MERGE_OPTS_INIT;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rza", &repo, &theirhead, &opts) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_their_head = (php_git2_t *) zend_fetch_resource(Z_RES_P(theirhead), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	heads[0] = PHP_GIT2_V(_their_head, merge_head);

	error = git_merge(&out, PHP_GIT2_V(_repo, repository), heads, 1, &options);

	if (php_git2_check_error(error, "git_merge" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_MERGE_RESULT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_merge_result_is_uptodate(resource $merge_result)
 */
PHP_FUNCTION(git_merge_result_is_uptodate)
{
	int result;
	zval *merge_result = NULL;
	php_git2_t *_merge_result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &merge_result) == FAILURE) {
		return;
	}

	if ((_merge_result = (php_git2_t *) zend_fetch_resource(Z_RES_P(merge_result), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_merge_result_is_uptodate(PHP_GIT2_V(_merge_result, merge_result));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_merge_result_is_fastforward(resource $merge_result)
 */
PHP_FUNCTION(git_merge_result_is_fastforward)
{
	int result;
	zval *merge_result = NULL;
	php_git2_t *_merge_result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &merge_result) == FAILURE) {
		return;
	}

	if ((_merge_result = (php_git2_t *) zend_fetch_resource(Z_RES_P(merge_result), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_merge_result_is_fastforward(PHP_GIT2_V(_merge_result, merge_result));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto string git_merge_result_fastforward_oid(resource $merge_result)
 */
PHP_FUNCTION(git_merge_result_fastforward_oid)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *merge_result = NULL;
	php_git2_t *_merge_result = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &merge_result) == FAILURE) {
		return;
	}

	if ((_merge_result = (php_git2_t *) zend_fetch_resource(Z_RES_P(merge_result), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_merge_result_fastforward_oid(&out, PHP_GIT2_V(_merge_result, merge_result));

	if (php_git2_check_error(error, "git_merge_result_fastforward_oid" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto void git_merge_result_free(resource $merge_result)
 */
PHP_FUNCTION(git_merge_result_free)
{
	zval *merge_result = NULL;
	php_git2_t *_merge_result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &merge_result) == FAILURE) {
		return;
	}

	if ((_merge_result = (php_git2_t *) zend_fetch_resource(Z_RES_P(merge_result), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_merge_result) */) {
		git_merge_result_free(PHP_GIT2_V(_merge_result, merge_result));
		GIT2_SHOULD_FREE(_merge_result) = 0;
	}

	//zval_ptr_dtor(merge_result);
}
/* }}} */

