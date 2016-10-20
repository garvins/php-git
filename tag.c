#include "php_git2.h"
#include "php_git2_priv.h"
#include "tag.h"

/* {{{ proto resource git_tag_lookup(resource $repo, string $id)
 */
PHP_FUNCTION(git_tag_lookup)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_tag *out = NULL;
	zval *repo = NULL;
	char *id = NULL;
	git_oid __id = {0};
	size_t id_len;
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

	error = git_tag_lookup(&out, PHP_GIT2_V(_repo, repository), &__id);

	if (php_git2_check_error(error, "git_tag_lookup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TAG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_tag_lookup_prefix(resource $repo, string $id)
 */
PHP_FUNCTION(git_tag_lookup_prefix)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_tag *out = NULL;
	zval *repo = NULL;
	char *id = NULL;
	git_oid __id = {0};
	size_t len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &id, &len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_tag_lookup_prefix(&out, PHP_GIT2_V(_repo, repository), &__id, len);

	if (php_git2_check_error(error, "git_tag_lookup_prefix" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TAG, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_tag_free(resource $tag)
 */
PHP_FUNCTION(git_tag_free)
{
	zval *tag = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_tag)) {
		git_tag_free(PHP_GIT2_V(_tag, tag));
		GIT2_SHOULD_FREE(_tag) = 0;
	}

	zval_ptr_dtor(tag);
}
/* }}} */

/* {{{ proto string git_tag_id(resource $tag)
 */
PHP_FUNCTION(git_tag_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *tag = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_id(PHP_GIT2_V(_tag, tag));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto resource git_tag_owner(resource $tag)
 */
PHP_FUNCTION(git_tag_owner)
{
	git_repository *result = NULL;
	zval *tag = NULL;
	php_git2_t *_tag = NULL, *__result = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_owner(PHP_GIT2_V(_tag, tag));

	if (php_git2_make_resource(&__result, PHP_GIT2_TYPE_REPOSITORY, result, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(__result));
}
/* }}} */

/* {{{ proto resource git_tag_target(resource $tag)
 */
PHP_FUNCTION(git_tag_target)
{
	php_git2_t *result = NULL, *_tag = NULL;
	git_object *target_out = NULL;
	zval *tag = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_target(&target_out, PHP_GIT2_V(_tag, tag));

	if (php_git2_check_error(error, "git_tag_target" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, target_out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_tag_target_id(resource $tag)
 */
PHP_FUNCTION(git_tag_target_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *tag = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_target_id(PHP_GIT2_V(_tag, tag));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto long git_tag_target_type(resource $tag)
 */
PHP_FUNCTION(git_tag_target_type)
{
	git_otype result;
	zval *tag = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_target_type(PHP_GIT2_V(_tag, tag));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_tag_name(resource $tag)
 */
PHP_FUNCTION(git_tag_name)
{
	const char *result = NULL;
	zval *tag = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_name(PHP_GIT2_V(_tag, tag));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto array git_tag_tagger(resource $tag)
 */
PHP_FUNCTION(git_tag_tagger)
{
	const git_signature *result = NULL;
	zval *tag = NULL, *array = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_tagger(PHP_GIT2_V(_tag, tag));

	php_git2_git_signature_to_array(result, array TSRMLS_CC);
	git_signature_free(result);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto string git_tag_message(resource $tag)
 */
PHP_FUNCTION(git_tag_message)
{
	const char *result = NULL;
	zval *tag = NULL;
	php_git2_t *_tag = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_message(PHP_GIT2_V(_tag, tag));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_tag_create(resource $repo, string $tag_name, resource $target, array $tagger, string $message, long $force)
 */
PHP_FUNCTION(git_tag_create)
{
	git_oid oid;
	char __oid[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *target = NULL, *tagger = NULL;
	php_git2_t *_repo = NULL, *_target = NULL;
	char *tag_name = NULL, *message = NULL;
	size_t tag_name_len, message_len;
	git_signature *_tagger = NULL;
	zend_long force;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsrasl", &repo, &tag_name, &tag_name_len, &target, &tagger, &message, &message_len, &force) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_tagger, tagger TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_target = (php_git2_t *) zend_fetch_resource(Z_RES_P(target), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_create(&oid, PHP_GIT2_V(_repo, repository), tag_name, PHP_GIT2_V(_target, object), _tagger, message, force);

	if (php_git2_check_error(error, "git_tag_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__oid, &oid);
	git_signature_free(_tagger);

	RETURN_STRING(__oid);
}
/* }}} */

/* {{{ proto string git_tag_annotation_create(resource $repo, string $tag_name, resource $target, array $tagger, string $message)
 */
PHP_FUNCTION(git_tag_annotation_create)
{
	git_oid oid;
	char __oid[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *target = NULL, *tagger = NULL;
	php_git2_t *_repo = NULL, *_target = NULL;
	char *tag_name = NULL, *message = NULL;
	size_t tag_name_len, message_len;
	git_signature *_tagger = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsras", &repo, &tag_name, &tag_name_len, &target, &tagger, &message, &message_len) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_tagger, tagger TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_target = (php_git2_t *) zend_fetch_resource(Z_RES_P(target), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_annotation_create(&oid, PHP_GIT2_V(_repo, repository), tag_name, PHP_GIT2_V(_target, object), _tagger, message);

	if (php_git2_check_error(error, "git_tag_annotation_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__oid, &oid);
	git_signature_free(_tagger);

	RETURN_STRING(__oid);
}
/* }}} */

/* {{{ proto string git_tag_create_frombuffer(resource $repo, string $buffer, long $force)
 */
PHP_FUNCTION(git_tag_create_frombuffer)
{
	git_oid oid;
	char __oid[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *buffer = NULL;
	size_t buffer_len;
	zend_long force;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &repo, &buffer, &buffer_len, &force) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_create_frombuffer(&oid, PHP_GIT2_V(_repo, repository), buffer, force);

	if (php_git2_check_error(error, "git_tag_create_frombuffer" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__oid, &oid);

	RETURN_STRING(__oid);
}
/* }}} */

/* {{{ proto string git_tag_create_lightweight(resource $repo, string $tag_name, resource $target, long $force)
 */
PHP_FUNCTION(git_tag_create_lightweight)
{
	git_oid oid;
	char __oid[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *target = NULL;
	php_git2_t *_repo = NULL, *_target = NULL;
	char *tag_name = NULL;
	size_t tag_name_len;
	zend_long force;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsrl", &repo, &tag_name, &tag_name_len, &target, &force) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_target = (php_git2_t *) zend_fetch_resource(Z_RES_P(target), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_create_lightweight(&oid, PHP_GIT2_V(_repo, repository), tag_name, PHP_GIT2_V(_target, object), force);

	if (php_git2_check_error(error, "git_tag_create_lightweight" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__oid, &oid);

	RETURN_STRING(__oid);
}
/* }}} */

/* {{{ proto long git_tag_delete(resource $repo, string $tag_name)
 */
PHP_FUNCTION(git_tag_delete)
{
	int result;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *tag_name = NULL;
	size_t tag_name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &tag_name, &tag_name_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_tag_delete(PHP_GIT2_V(_repo, repository), tag_name);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_tag_list(resource $repo)
 */
PHP_FUNCTION(git_tag_list)
{
	zval *array, *repo = NULL;
	git_strarray *tag_names = NULL;
	php_git2_t *_repo = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_list(tag_names, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_tag_list" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_strarray_to_array(tag_names, array TSRMLS_CC);
	git_strarray_free(tag_names);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto array git_tag_list_match(string $pattern, resource $repo)
 */
PHP_FUNCTION(git_tag_list_match)
{
	zval *array, *repo = NULL;
	git_strarray *tag_names = NULL;
	char *pattern = NULL;
	size_t pattern_len;
	php_git2_t *_repo = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sr", &pattern, &pattern_len, &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_list_match(tag_names, pattern, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_tag_list_match" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	php_git2_git_strarray_to_array(tag_names, array TSRMLS_CC);
	git_strarray_free(tag_names);

	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto long git_tag_foreach(resource $repo, Callable $callback, void $payload)
 */
PHP_FUNCTION(git_tag_foreach)
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

	result = git_tag_foreach(PHP_GIT2_V(_repo, repository), php_git2_git_tag_foreach_cb, callback_cb);
	php_git2_cb_free(callback_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_tag_peel(resource $tag)
 */
PHP_FUNCTION(git_tag_peel)
{
	php_git2_t *result = NULL, *_tag = NULL;
	git_object *tag_target_out = NULL;
	zval *tag = NULL;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &tag) == FAILURE) {
		return;
	}

	if ((_tag = (php_git2_t *) zend_fetch_resource(Z_RES_P(tag), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_tag_peel(&tag_target_out, PHP_GIT2_V(_tag, tag));

	if (php_git2_check_error(error, "git_tag_peel" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, tag_target_out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

