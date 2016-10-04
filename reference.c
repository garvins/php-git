#include "php_git2.h"
#include "php_git2_priv.h"
#include "reference.h"

/* {{{ proto resource git_reference_lookup(resource $repo, string $name)
 */
PHP_FUNCTION(git_reference_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference *out = NULL;
	zval *repo = NULL;
	char *name = NULL;
	size_t name_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_lookup(&out, PHP_GIT2_V(_repo, repository), name);

	if (php_git2_check_error(error, "git_reference_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_reference_name_to_id(resource $repo, string $name)
 */
PHP_FUNCTION(git_reference_name_to_id)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *name = NULL;
	size_t name_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &name, &name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_name_to_id(&out, PHP_GIT2_V(_repo, repository), name);

	if (php_git2_check_error(error, "git_reference_name_to_id" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto resource git_reference_dwim(resource $repo, string $shorthand)
 */
PHP_FUNCTION(git_reference_dwim)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference *out = NULL;
	zval *repo = NULL;
	char *shorthand = NULL;
	size_t shorthand_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &shorthand, &shorthand_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_dwim(&out, PHP_GIT2_V(_repo, repository), shorthand);

	if (php_git2_check_error(error, "git_reference_dwim" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_symbolic_create(resource $repo, string $name, string $target, long $force)
 */
PHP_FUNCTION(git_reference_symbolic_create)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference *out = NULL;
	zval *repo = NULL;
	char *name = NULL, *target = NULL;
	size_t name_len, target_len;
	zend_long force;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rssl", &repo, &name, &name_len, &target, &target_len, &force) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_symbolic_create(&out, PHP_GIT2_V(_repo, repository), name, target, force);

	if (php_git2_check_error(error, "git_reference_symbolic_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_create(resource $repo, string $name, string $id, long $force)
 */
PHP_FUNCTION(git_reference_create)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference *out = NULL;
	zval *repo = NULL;
	char *name = NULL, *id = NULL;
	size_t name_len, id_len;
	git_oid __id = {0};
	zend_long force;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rssl", &repo, &name, &name_len, &id, &id_len, &force) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_reference_create(&out, PHP_GIT2_V(_repo, repository), name, &__id, force);

	if (php_git2_check_error(error, "git_reference_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_reference_target(resource $ref)
 */
PHP_FUNCTION(git_reference_target)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_target(PHP_GIT2_V(_ref, reference));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_reference_target_peel(resource $ref)
 */
PHP_FUNCTION(git_reference_target_peel)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_target_peel(PHP_GIT2_V(_ref, reference));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_reference_symbolic_target(resource $ref)
 */
PHP_FUNCTION(git_reference_symbolic_target)
{
	const char *result = NULL;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_symbolic_target(PHP_GIT2_V(_ref, reference));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_reference_type(resource $ref)
 */
PHP_FUNCTION(git_reference_type)
{
	git_ref_t result;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_type(PHP_GIT2_V(_ref, reference));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_reference_name(resource $ref)
 */
PHP_FUNCTION(git_reference_name)
{
	const char *result = NULL;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_name(PHP_GIT2_V(_ref, reference));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto resource git_reference_resolve(resource $ref)
 */
PHP_FUNCTION(git_reference_resolve)
{
	php_git2_t *result = NULL, *_ref = NULL;
	git_reference *out = NULL;
	zval *ref = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_resolve(&out, PHP_GIT2_V(_ref, reference));

	if (php_git2_check_error(error, "git_reference_resolve" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_owner(resource $ref)
 */
PHP_FUNCTION(git_reference_owner)
{
	git_repository *result = NULL;
	zval *ref = NULL;
	php_git2_t *_ref = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_owner(PHP_GIT2_V(_ref, reference));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_reference_symbolic_set_target(resource $ref, string $target)
 */
PHP_FUNCTION(git_reference_symbolic_set_target)
{
	php_git2_t *result = NULL, *_ref = NULL;
	git_reference *out = NULL;
	zval *ref = NULL;
	char *target = NULL;
	size_t target_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &ref, &target, &target_len) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_symbolic_set_target(&out, PHP_GIT2_V(_ref, reference), target);

	if (php_git2_check_error(error, "git_reference_symbolic_set_target" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_set_target(resource $ref, string $id)
 */
PHP_FUNCTION(git_reference_set_target)
{
	php_git2_t *result = NULL, *_ref = NULL;
	git_reference *out = NULL;
	zval *ref = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &ref, &id, &id_len) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_reference_set_target(&out, PHP_GIT2_V(_ref, reference), &__id);

	if (php_git2_check_error(error, "git_reference_set_target" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_rename(resource $ref, string $new_name, long $force)
 */
PHP_FUNCTION(git_reference_rename)
{
	php_git2_t *result = NULL, *_ref = NULL;
	git_reference *new_ref = NULL;
	zval *ref = NULL;
	char *new_name = NULL;
	size_t new_name_len;
	zend_long force;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &ref, &new_name, &new_name_len, &force) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_rename(&new_ref, PHP_GIT2_V(_ref, reference), new_name, force);

	if (php_git2_check_error(error, "git_reference_rename" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, new_ref, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_reference_delete(resource $ref)
 */
PHP_FUNCTION(git_reference_delete)
{
	int result;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_delete(PHP_GIT2_V(_ref, reference));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_reference_list(resource $repo)
 */
PHP_FUNCTION(git_reference_list)
{
	zval *result, *repo = NULL;
	git_strarray *array = NULL;
	php_git2_t *_repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_list(array, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_reference_list" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_strarray_to_array(array, &array TSRMLS_CC);
	git_strarray_free(array);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_reference_foreach(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_reference_foreach)
{
	int result;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_reference_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_reference_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reference_foreach_name(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_reference_foreach_name)
{
	int result;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &repo, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_reference_foreach_name(PHP_GIT2_V(_repo, repository), php_git2_git_reference_foreach_name_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_reference_free(resource $ref)
 */
PHP_FUNCTION(git_reference_free)
{
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_ref)) {
		git_reference_free(PHP_GIT2_V(_ref, reference));
		GIT2_SHOULD_FREE(_ref) = 0;
	}

	zval_ptr_dtor(&ref);
}
/* }}} */

/* {{{ proto long git_reference_cmp(resource $ref1, resource $ref2)
 */
PHP_FUNCTION(git_reference_cmp)
{
	int result;
	zval *ref1 = NULL, *ref2 = NULL;
	php_git2_t *_ref1 = NULL, *_ref2 = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &ref1, &ref2) == FAILURE) {
		return;
	}

	if ((_ref1 = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref1), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_ref2 = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref2), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_cmp(PHP_GIT2_V(_ref1, reference), PHP_GIT2_V(_ref2, reference));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_reference_iterator_new(resource $repo)
 */
PHP_FUNCTION(git_reference_iterator_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference_iterator *out = NULL;
	zval *repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_iterator_new(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_reference_iterator_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE_ITERATOR, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_iterator_glob_new(resource $repo, string $glob)
 */
PHP_FUNCTION(git_reference_iterator_glob_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_reference_iterator *out = NULL;
	zval *repo = NULL;
	char *glob = NULL;
	size_t glob_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &glob, &glob_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_iterator_glob_new(&out, PHP_GIT2_V(_repo, repository), glob);

	if (php_git2_check_error(error, "git_reference_iterator_glob_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE_ITERATOR, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_reference_next(resource $iter)
 */
PHP_FUNCTION(git_reference_next)
{
	php_git2_t *result = NULL, *_iter = NULL;
	git_reference *out = NULL;
	zval *iter = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iter) == FAILURE) {
		return;
	}

	if ((_iter = (php_git2_t *) zend_fetch_resource(Z_RES_P(iter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_next(&out, PHP_GIT2_V(_iter, reference_iterator));

	if (php_git2_check_error(error, "git_reference_next" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_reference_next_name(resource $iter)
 */
PHP_FUNCTION(git_reference_next_name)
{
	char *out = NULL;
	zval *iter = NULL;
	php_git2_t *_iter = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iter) == FAILURE) {
		return;
	}

	if ((_iter = (php_git2_t *) zend_fetch_resource(Z_RES_P(iter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_next_name(&out, PHP_GIT2_V(_iter, reference_iterator));

	if (php_git2_check_error(error, "git_reference_next_name" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto void git_reference_iterator_free(resource $iter)
 */
PHP_FUNCTION(git_reference_iterator_free)
{
	zval *iter = NULL;
	php_git2_t *_iter = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &iter) == FAILURE) {
		return;
	}

	if ((_iter = (php_git2_t *) zend_fetch_resource(Z_RES_P(iter), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_iter)) {
		git_reference_iterator_free(PHP_GIT2_V(_iter, reference_iterator));
		GIT2_SHOULD_FREE(_iter) = 0;
	}

	zval_ptr_dtor(&iter);
}
/* }}} */

/* {{{ proto long git_reference_foreach_glob(resource $repo, string $glob, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_reference_foreach_glob)
{
	int result;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	char *glob = NULL;
	size_t glob_len;
	zend_fcall_info callback_fci = empty_fcall_info;
	zend_fcall_info_cache callback_fcc = empty_fcall_info_cache;
	php_git2_cb_t *callback_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsfz", &repo, &glob, &glob_len, &callback_fci, &callback_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&callback_cb, &callback_fci, &callback_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_reference_foreach_glob(PHP_GIT2_V(_repo, repository), glob, php_git2_git_reference_foreach_name_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_reference_has_log(resource $ref)
 */
PHP_FUNCTION(git_reference_has_log)
{
	int result;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_has_log(PHP_GIT2_V(_ref, reference));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_reference_is_branch(resource $ref)
 */
PHP_FUNCTION(git_reference_is_branch)
{
	int result;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_is_branch(PHP_GIT2_V(_ref, reference));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_reference_is_remote(resource $ref)
 */
PHP_FUNCTION(git_reference_is_remote)
{
	int result;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_is_remote(PHP_GIT2_V(_ref, reference));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_reference_is_tag(resource $ref)
 */
PHP_FUNCTION(git_reference_is_tag)
{
	int result;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_is_tag(PHP_GIT2_V(_ref, reference));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto string git_reference_normalize_name(string $name, long $flags)
 */
PHP_FUNCTION(git_reference_normalize_name)
{
	char buffer_out[GIT2_BUFFER_SIZE] = {0}, *name = NULL;
	size_t buffer_size = GIT2_BUFFER_SIZE, name_len;
	zend_long flags;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &name, &name_len, &flags) == FAILURE) {
		return;
	}

	error = git_reference_normalize_name(buffer_out, buffer_size, name, flags);

	if (php_git2_check_error(error, "git_reference_normalize_name" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(buffer_out);
}
/* }}} */

/* {{{ proto resource git_reference_peel(resource $ref, long $type)
 */
PHP_FUNCTION(git_reference_peel)
{
	php_git2_t *result = NULL, *_ref = NULL;
	git_object *out = NULL;
	zval *ref = NULL;
	zend_long type;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &ref, &type) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_reference_peel(&out, PHP_GIT2_V(_ref, reference), type);

	if (php_git2_check_error(error, "git_reference_peel" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_reference_is_valid_name(string $refname)
 */
PHP_FUNCTION(git_reference_is_valid_name)
{
	int result;
	char *refname = NULL;
	size_t refname_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &refname, &refname_len) == FAILURE) {
		return;
	}

	result = git_reference_is_valid_name(refname);

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto string git_reference_shorthand(resource $ref)
 */
PHP_FUNCTION(git_reference_shorthand)
{
	const char *result = NULL;
	zval *ref = NULL;
	php_git2_t *_ref = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &ref) == FAILURE) {
		return;
	}

	if ((_ref = (php_git2_t *) zend_fetch_resource(Z_RES_P(ref), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_reference_shorthand(PHP_GIT2_V(_ref, reference));

	RETURN_STRING(result);
}
/* }}} */

