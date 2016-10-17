#include "php_git2.h"
#include "php_git2_priv.h"
#include "object.h"

/* {{{ proto resource git_object_lookup(resource $repo, string $id, long $type)
 */
PHP_FUNCTION(git_object_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_object *object = NULL;
	zval *repo = NULL;
	char *id = NULL;
	git_oid __id = {0};
	size_t id_len;
	zend_long type;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &repo, &id, &id_len, &type) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_object_lookup(&object, PHP_GIT2_V(_repo, repository), &__id, type);

	if (php_git2_check_error(error, "git_object_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, object, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_object_lookup_prefix(resource $repo, string $id, long $type)
 */
PHP_FUNCTION(git_object_lookup_prefix)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_object *object_out = NULL;
	zval *repo = NULL;
	char *id = NULL;
	git_oid __id = {0};
	size_t len;
	zend_long type;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &repo, &id, &len, &type) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_object_lookup_prefix(&object_out, PHP_GIT2_V(_repo, repository), &__id, len, type);

	if (php_git2_check_error(error, "git_object_lookup_prefix" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, object_out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_object_lookup_bypath(resource $treeish, string $path, long $type)
 */
PHP_FUNCTION(git_object_lookup_bypath)
{
	php_git2_t *result = NULL, *_treeish = NULL;
	git_object *out = NULL;
	zval *treeish = NULL;
	char *path = NULL;
	size_t path_len;
	zend_long type;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &treeish, &path, &path_len, &type) == FAILURE) {
		return;
	}

	if ((_treeish = (php_git2_t *) zend_fetch_resource(Z_RES_P(treeish), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_object_lookup_bypath(&out, PHP_GIT2_V(_treeish, object), path, type);

	if (php_git2_check_error(error, "git_object_lookup_bypath" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_object_id(resource $obj)
 */
PHP_FUNCTION(git_object_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *obj = NULL;
	php_git2_t *_obj = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &obj) == FAILURE) {
		return;
	}

	if ((_obj = (php_git2_t *) zend_fetch_resource(Z_RES_P(obj), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_object_id(PHP_GIT2_V(_obj, object));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_object_type(resource $obj)
 */
PHP_FUNCTION(git_object_type)
{
	git_otype result;
	zval *obj = NULL;
	php_git2_t *_obj = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &obj) == FAILURE) {
		return;
	}

	if ((_obj = (php_git2_t *) zend_fetch_resource(Z_RES_P(obj), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_object_type(PHP_GIT2_V(_obj, object));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_object_owner(resource $obj)
 */
PHP_FUNCTION(git_object_owner)
{
	git_repository *result = NULL;
	zval *obj = NULL;
	php_git2_t *_obj = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &obj) == FAILURE) {
		return;
	}

	if ((_obj = (php_git2_t *) zend_fetch_resource(Z_RES_P(obj), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_object_owner(PHP_GIT2_V(_obj, object));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto void git_object_free(resource $object)
 */
PHP_FUNCTION(git_object_free)
{
	zval *object = NULL;
	php_git2_t *_object = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &object) == FAILURE) {
		return;
	}

	if ((_object = (php_git2_t *) zend_fetch_resource(Z_RES_P(object), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_object)) {
		git_object_free(PHP_GIT2_V(_object, object));
		GIT2_SHOULD_FREE(_object) = 0;
	}

	zval_ptr_dtor(object);
}
/* }}} */

/* {{{ proto string git_object_type2string(long $type)
 */
PHP_FUNCTION(git_object_type2string)
{
	const char *result = NULL;
	zend_long type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l", &type) == FAILURE) {
		return;
	}

	result = git_object_type2string(type);

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto long git_object_string2type(string $str)
 */
PHP_FUNCTION(git_object_string2type)
{
	git_otype result;
	char *str = NULL;
	size_t str_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &str, &str_len) == FAILURE) {
		return;
	}

	result = git_object_string2type(str);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_object_typeisloose(long $type)
 */
PHP_FUNCTION(git_object_typeisloose)
{
	int result;
	zend_long type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l", &type) == FAILURE) {
		return;
	}

	result = git_object_typeisloose(type);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_object__size(long $type)
 */
PHP_FUNCTION(git_object__size)
{
	size_t result;
	zend_long type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l", &type) == FAILURE) {
		return;
	}

	result = git_object__size(type);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_object_peel(resource $object, long $target_type)
 */
PHP_FUNCTION(git_object_peel)
{
	php_git2_t *result = NULL, *_object = NULL;
	git_object *peeled = NULL;
	zval *object = NULL;
	zend_long target_type;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &object, &target_type) == FAILURE) {
		return;
	}

	if ((_object = (php_git2_t *) zend_fetch_resource(Z_RES_P(object), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_object_peel(&peeled, PHP_GIT2_V(_object, object), target_type);

	if (php_git2_check_error(error, "git_object_peel" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, peeled, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_object_dup(resource $source)
 */
PHP_FUNCTION(git_object_dup)
{
	php_git2_t *result = NULL, *_source = NULL;
	git_object *dest = NULL;
	zval *source = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &source) == FAILURE) {
		return;
	}

	if ((_source = (php_git2_t *) zend_fetch_resource(Z_RES_P(source), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_object_dup(&dest, PHP_GIT2_V(_source, object));

	if (php_git2_check_error(error, "git_object_dup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, dest, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

