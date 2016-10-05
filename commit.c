#include "php_git2.h"
#include "php_git2_priv.h"
#include "commit.h"

/* {{{ proto void git_commit_free(resource $commit)
 */
PHP_FUNCTION(git_commit_free)
{
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_commit)) {
		git_commit_free(PHP_GIT2_V(_commit, commit));
		GIT2_SHOULD_FREE(_commit) = 0;
	}

	zval_ptr_dtor(&commit);
}
/* }}} */

/* {{{ proto resource git_commit_lookup(resource $repo, string $id)
 */
PHP_FUNCTION(git_commit_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_commit *commit = NULL;
	zval *repo = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &id, &id_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_commit_lookup(&commit, PHP_GIT2_V(_repo, repository), &__id);

	if (php_git2_check_error(error, "git_commit_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_COMMIT, commit, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto array git_commit_author(resource $commit)
 */
PHP_FUNCTION(git_commit_author)
{
	const git_signature *result = NULL;
	zval *commit = NULL, *array = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_author(PHP_GIT2_V(_commit, commit));

	php_git2_git_signature_to_array(result, array TSRMLS_CC);
	git_signature_free(result);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto resource git_commit_tree(resource $commit)
 */
PHP_FUNCTION(git_commit_tree)
{
	php_git2_t *result = NULL, *_commit = NULL;
	git_tree *tree_out = NULL;
	zval *commit = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_commit_tree(&tree_out, PHP_GIT2_V(_commit, commit));

	if (php_git2_check_error(error, "git_commit_tree" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TREE, tree_out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_commit_lookup_prefix(resource $repo, string $id, long $len)
 */
PHP_FUNCTION(git_commit_lookup_prefix)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_commit *commit = NULL;
	zval *repo = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	zend_long len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &repo, &id, &id_len, &len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_commit_lookup_prefix(&commit, PHP_GIT2_V(_repo, repository), &__id, len);

	if (php_git2_check_error(error, "git_commit_lookup_prefix" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_COMMIT, commit, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_commit_id(resource $commit)
 */
PHP_FUNCTION(git_commit_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_id(PHP_GIT2_V(_commit, commit));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto resource git_commit_owner(resource $commit)
 */
PHP_FUNCTION(git_commit_owner)
{
	git_repository *result = NULL;
	zval *commit = NULL;
	php_git2_t *_commit = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_owner(PHP_GIT2_V(_commit, commit));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto string git_commit_message_encoding(resource $commit)
 */
PHP_FUNCTION(git_commit_message_encoding)
{
	const char *result = NULL;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_message_encoding(PHP_GIT2_V(_commit, commit));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_commit_message(resource $commit)
 */
PHP_FUNCTION(git_commit_message)
{
	const char *result = NULL;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_message(PHP_GIT2_V(_commit, commit));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_commit_message_raw(resource $commit)
 */
PHP_FUNCTION(git_commit_message_raw)
{
	const char *result = NULL;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_message_raw(PHP_GIT2_V(_commit, commit));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_commit_time(resource $commit)
 */
PHP_FUNCTION(git_commit_time)
{
	git_time_t result;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_time(PHP_GIT2_V(_commit, commit));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_commit_time_offset(resource $commit)
 */
PHP_FUNCTION(git_commit_time_offset)
{
	int result;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_time_offset(PHP_GIT2_V(_commit, commit));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_commit_committer(resource $commit)
 */
PHP_FUNCTION(git_commit_committer)
{
	const git_signature *result = NULL;
	zval *commit = NULL, *array = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_committer(PHP_GIT2_V(_commit, commit));

	php_git2_git_signature_to_array(result, array TSRMLS_CC);
	git_signature_free(result);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto string git_commit_raw_header(resource $commit)
 */
PHP_FUNCTION(git_commit_raw_header)
{
	const char *result = NULL;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_raw_header(PHP_GIT2_V(_commit, commit));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_commit_tree_id(resource $commit)
 */
PHP_FUNCTION(git_commit_tree_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_tree_id(PHP_GIT2_V(_commit, commit));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_commit_parentcount(resource $commit)
 */
PHP_FUNCTION(git_commit_parentcount)
{
	unsigned int result;
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &commit) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_parentcount(PHP_GIT2_V(_commit, commit));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_commit_parent(resource $commit, long $n)
 */
PHP_FUNCTION(git_commit_parent)
{
	php_git2_t *result = NULL, *_commit = NULL;
	git_commit *out = NULL;
	zval *commit = NULL;
	zend_long n;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &commit, &n) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_commit_parent(&out, PHP_GIT2_V(_commit, commit), n);

	if (php_git2_check_error(error, "git_commit_parent" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_COMMIT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_commit_parent_id(resource $commit, long $n)
 */
PHP_FUNCTION(git_commit_parent_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *commit = NULL;
	php_git2_t *_commit = NULL;
	zend_long n;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &commit, &n) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_commit_parent_id(PHP_GIT2_V(_commit, commit), n);

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto resource git_commit_nth_gen_ancestor(resource $commit, long $n)
 */
PHP_FUNCTION(git_commit_nth_gen_ancestor)
{
	php_git2_t *result = NULL, *_commit = NULL;
	git_commit *ancestor = NULL;
	zval *commit = NULL;
	zend_long n;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &commit, &n) == FAILURE) {
		return;
	}

	if ((_commit = (php_git2_t *) zend_fetch_resource(Z_RES_P(commit), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_commit_nth_gen_ancestor(&ancestor, PHP_GIT2_V(_commit, commit), n);

	if (php_git2_check_error(error, "git_commit_nth_gen_ancestor" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_COMMIT, ancestor, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_commit_create(resource $repo, string $update_ref, array $author, array $committer, string $message_encoding, string $message, resource $tree, long $parent_count, resource $parents[])
 */
PHP_FUNCTION(git_commit_create)
{
	zval *repo, *tree, *parents, *committer, *author, **element;
	char *update_ref = {0}, *message_encoding = {0}, *message = {0};
	int update_ref_len, message_encoding_len, message_len, parent_count = 0, error = 0, i;
	php_git2_t *_repo, *_tree;
	git_signature __author, __committer;
	char out[GIT2_OID_HEXSIZE] = {0};
	git_oid oid;
	const git_commit **__parents = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
			"rsaassra", &repo, &update_ref, &update_ref_len, &author,
			&committer, &message_encoding, &message_encoding_len, &message,
			&message_len, &tree, &parents) == FAILURE) {
		return;
	}

	memset(&__author, '\0', sizeof(git_signature));
	memset(&__committer, '\0', sizeof(git_signature));

	if (committer == NULL || Z_TYPE_P(committer) == IS_NULL) {
		committer = author;
	}

	php_git2_array_to_signature(&__author, author TSRMLS_CC);
	php_git2_array_to_signature(&__committer, committer TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	parent_count = zend_hash_num_elements(Z_ARRVAL_P(parents));
	__parents = emalloc(parent_count * sizeof(void *));

    i = 0;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(parents),element) {
		git_commit *p = NULL;

        if (Z_TYPE_P(element) == IS_STRING) { //HASH_VAL_IS_STRING
			error = git_oid_fromstr(&oid, Z_STRVAL_PP(element));
			git_commit_lookup(&p, PHP_GIT2_V(_repo, repository), &oid);
        } else if (Z_TYPE_P(element) == IS_RESOURCE) { //HASH_VAL_IS_RESOURCE
			php_git2_t *t;

	        if ((t = (php_git2_t *) zend_fetch_resource(Z_RES_P(*element), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		        RETURN_FALSE;
	        }

			p = PHP_GIT2_V(t, commit);
        }

		__parents[i] = p;
		i++;
    } ZEND_HASH_FOREACH_END();

	error = git_commit_create(
		&oid,
		PHP_GIT2_V(_repo, repository),
		update_ref,
		&__author,
		&__committer,
		message_encoding,
		message,
		PHP_GIT2_V(_tree, tree),
		parent_count,
		__parents
	);

	efree(__parents);

	if (php_git2_check_error(error, "git_commit_create" TSRMLS_CC)) {
		RETURN_FALSE
	}

	git_oid_fmt(out, &oid);
    RETURN_STRING(out);
}
/* }}} */

