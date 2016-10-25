#include "php_git2.h"
#include "php_git2_priv.h"
#include "reflog.h"

/* {{{ proto resource git_reflog_read(resource $repo, string $name)
 */
PHP_FUNCTION(git_reflog_read)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reflog *out = NULL;
	zval *repo = NULL;
	char *name = NULL;
	size_t name_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reflog_read(&out, PHP_GIT2_V(_repo, repository), name);

	if (php_git2_check_error(error, "git_reflog_read" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFLOG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_reflog_write(resource $reflog)
 */
PHP_FUNCTION(git_reflog_write)
{
	int result;
	zval *reflog = NULL;
	php_git2_t *_reflog = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &reflog) == FAILURE) {
		return;
	}

	if ((_reflog = (php_git2_t *) zend_fetch_resource(Z_RES_P(reflog), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_write(PHP_GIT2_V(_reflog, reflog));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reflog_append(resource $reflog, string $id, array $committer, string $msg)
 */
PHP_FUNCTION(git_reflog_append)
{
	int result;
	zval *reflog = NULL, *committer = NULL;
	php_git2_t *_reflog = NULL;
	char *id = NULL, *msg = NULL;
	git_oid __id = {0};
	size_t id_len, msg_len;
	git_signature *_committer = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsas", &reflog, &id, &id_len, &committer, &msg, &msg_len) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_committer, committer TSRMLS_CC);

	if ((_reflog = (php_git2_t *) zend_fetch_resource(Z_RES_P(reflog), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	result = git_reflog_append(PHP_GIT2_V(_reflog, reflog), &__id, _committer, msg);
	git_signature_free(_committer);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reflog_append_to(resource $repo, string $name, string $id, array $committer, string $msg)
 */
PHP_FUNCTION(git_reflog_append_to)
{
	int result;
	zval *repo = NULL, *committer = NULL;
	php_git2_t *_repo = NULL;
	char *name = NULL, *id = NULL, *msg = NULL;
	size_t name_len, id_len, msg_len;
	git_oid __id = {0};
	git_signature *_committer = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rssas", &repo, &name, &name_len, &id, &id_len, &committer, &msg, &msg_len) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_committer, committer TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	result = git_reflog_append_to(PHP_GIT2_V(_repo, repository), name, &__id, _committer, msg);
	git_signature_free(_committer);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reflog_rename(resource $repo, string $old_name, string $name)
 */
PHP_FUNCTION(git_reflog_rename)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *old_name = NULL, *name = NULL;
	size_t old_name_len, name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &repo, &old_name, &old_name_len, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_rename(PHP_GIT2_V(_repo, repository), old_name, name);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reflog_delete(resource $repo, string $name)
 */
PHP_FUNCTION(git_reflog_delete)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *name = NULL;
	size_t name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_delete(PHP_GIT2_V(_repo, repository), name);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reflog_entrycount(resource $reflog)
 */
PHP_FUNCTION(git_reflog_entrycount)
{
	size_t result;
	zval *reflog = NULL;
	php_git2_t *_reflog = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &reflog) == FAILURE) {
		return;
	}

	if ((_reflog = (php_git2_t *) zend_fetch_resource(Z_RES_P(reflog), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_entrycount(PHP_GIT2_V(_reflog, reflog));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_reflog_entry_byindex(resource $reflog, long $idx)
 */
PHP_FUNCTION(git_reflog_entry_byindex)
{
	const git_reflog_entry *result = NULL;
	zval *reflog = NULL;
	php_git2_t *_reflog = NULL, *__result = NULL;
	zend_long idx;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &reflog, &idx) == FAILURE) {
		return;
	}

	if ((_reflog = (php_git2_t *) zend_fetch_resource(Z_RES_P(reflog), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_entry_byindex(PHP_GIT2_V(_reflog, reflog), idx);

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REFLOG_ENTRY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto long git_reflog_drop(resource $reflog, long $idx, long $rewrite_previous_entry)
 */
PHP_FUNCTION(git_reflog_drop)
{
	int result;
	zval *reflog = NULL;
	php_git2_t *_reflog = NULL;
	zend_long idx, rewrite_previous_entry;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rll", &reflog, &idx, &rewrite_previous_entry) == FAILURE) {
		return;
	}

	if ((_reflog = (php_git2_t *) zend_fetch_resource(Z_RES_P(reflog), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_drop(PHP_GIT2_V(_reflog, reflog), idx, rewrite_previous_entry);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_reflog_entry_id_old(resource $entry)
 */
PHP_FUNCTION(git_reflog_entry_id_old)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_entry_id_old(PHP_GIT2_V(_entry, reflog_entry));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_reflog_entry_id_new(resource $entry)
 */
PHP_FUNCTION(git_reflog_entry_id_new)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_entry_id_new(PHP_GIT2_V(_entry, reflog_entry));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto array git_reflog_entry_committer(resource $entry)
 */
PHP_FUNCTION(git_reflog_entry_committer)
{
	const git_signature *result = NULL;
	zval *entry = NULL, *array = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_entry_committer(PHP_GIT2_V(_entry, reflog_entry));

	php_git2_git_signature_to_array(result, array TSRMLS_CC);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto string git_reflog_entry_message(resource $entry)
 */
PHP_FUNCTION(git_reflog_entry_message)
{
	const char *result = NULL;
	zval *entry = NULL;
	php_git2_t *_entry = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &entry) == FAILURE) {
		return;
	}

	if ((_entry = (php_git2_t *) zend_fetch_resource(Z_RES_P(entry), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reflog_entry_message(PHP_GIT2_V(_entry, reflog_entry));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto void git_reflog_free(resource $reflog)
 */
PHP_FUNCTION(git_reflog_free)
{
	zval *reflog = NULL;
	php_git2_t *_reflog = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &reflog) == FAILURE) {
		return;
	}

	if ((_reflog = (php_git2_t *) zend_fetch_resource(Z_RES_P(reflog), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_reflog) */) {
		git_reflog_free(PHP_GIT2_V(_reflog, reflog));
		GIT2_SHOULD_FREE(_reflog) = 0;
	}

	//zval_ptr_dtor(reflog);
}
/* }}} */

