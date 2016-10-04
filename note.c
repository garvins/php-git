#include "php_git2.h"
#include "php_git2_priv.h"
#include "note.h"

/* {{{ proto resource git_note_iterator_new(resource $repo, string $notes_ref)
 */
PHP_FUNCTION(git_note_iterator_new)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_note_iterator *out = NULL;
	zval *repo = NULL;
	char *notes_ref = NULL;
	size_t notes_ref_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &notes_ref, &notes_ref_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_note_iterator_new(&out, PHP_GIT2_V(_repo, repository), notes_ref);

	if (php_git2_check_error(error, "git_note_iterator_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_NOTE_ITERATOR, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_note_iterator_free(resource $it)
 */
PHP_FUNCTION(git_note_iterator_free)
{
	zval *it = NULL;
	php_git2_t *_it = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &it) == FAILURE) {
		return;
	}

	if ((_it = (php_git2_t *) zend_fetch_resource(Z_RES_P(it), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_it)) {
		git_note_iterator_free(PHP_GIT2_V(_it, note_iterator));
		GIT2_SHOULD_FREE(_it) = 0;
	}

	zval_ptr_dtor(&it);
}
/* }}} */

/* {{{ proto long git_note_next(string $note_id, string $annotated_id, resource $it)
 */
PHP_FUNCTION(git_note_next)
{
	int result;
	char *note_id = NULL, *annotated_id = NULL;
	size_t note_id_len, annotated_id_len;
	git_oid __note_id = {0}, __annotated_id = {0};
	zval *it = NULL;
	php_git2_t *_it = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ssr", &note_id, &note_id_len, &annotated_id, &annotated_id_len, &it) == FAILURE) {
		return;
	}

	if (git_oid_fromstrn(&__note_id, note_id, note_id_len)) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__annotated_id, annotated_id, annotated_id_len)) {
		RETURN_FALSE;
	}

	if ((_it = (php_git2_t *) zend_fetch_resource(Z_RES_P(it), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_note_next(&__note_id, &__annotated_id, PHP_GIT2_V(_it, note_iterator));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_note_read(resource $repo, string $notes_ref, string $oid)
 */
PHP_FUNCTION(git_note_read)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_note *out = NULL;
	zval *repo = NULL;
	char *notes_ref = NULL, *oid = NULL;
	size_t notes_ref_len, oid_len;
	git_oid __oid = {0};
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rss", &repo, &notes_ref, &notes_ref_len, &oid, &oid_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	error = git_note_read(&out, PHP_GIT2_V(_repo, repository), notes_ref, &__oid);

	if (php_git2_check_error(error, "git_note_read" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_NOTE, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto string git_note_message(resource $note)
 */
PHP_FUNCTION(git_note_message)
{
	const char *result = NULL;
	zval *note = NULL;
	php_git2_t *_note = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &note) == FAILURE) {
		return;
	}

	if ((_note = (php_git2_t *) zend_fetch_resource(Z_RES_P(note), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_note_message(PHP_GIT2_V(_note, note));

	RETURN_STRING(result);
}
/* }}} */

/* {{{ proto string git_note_oid(resource $note)
 */
PHP_FUNCTION(git_note_oid)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *note = NULL;
	php_git2_t *_note = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &note) == FAILURE) {
		return;
	}

	if ((_note = (php_git2_t *) zend_fetch_resource(Z_RES_P(note), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_note_oid(PHP_GIT2_V(_note, note));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_note_create(resource $repo, array $author, array $committer, string $notes_ref, string $oid, string $note, long $force)
 */
PHP_FUNCTION(git_note_create)
{
	git_oid out, __oid = {0};
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *repo = NULL, *author = NULL, *committer = NULL;
	php_git2_t *_repo = NULL;
	git_signature *_author = NULL;
	git_signature *_committer = NULL;
	char *notes_ref = NULL, *oid = NULL, *note = NULL;
	size_t notes_ref_len, oid_len, note_len;
	zend_long force;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"raasssl", &repo, &author, &committer, &notes_ref, &notes_ref_len, &oid, &oid_len, &note, &note_len, &force) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_author, author TSRMLS_CC);

	php_git2_array_to_git_signature(_committer, committer TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	error = git_note_create(&out, PHP_GIT2_V(_repo, repository), _author, _committer, notes_ref, &__oid, note, force);

	if (php_git2_check_error(error, "git_note_create" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);
	git_signature_free(_author);
	git_signature_free(_committer);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto long git_note_remove(resource $repo, string $notes_ref, array $author, array $committer, string $oid)
 */
PHP_FUNCTION(git_note_remove)
{
	int result;
	zval *repo = NULL, *author = NULL, *committer = NULL;
	php_git2_t *_repo = NULL;
	char *notes_ref = NULL, *oid = NULL;
	size_t notes_ref_len, oid_len;
	git_signature *_author = NULL;
	git_signature *_committer = NULL;
	git_oid __oid = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsaas", &repo, &notes_ref, &notes_ref_len, &author, &committer, &oid, &oid_len) == FAILURE) {
		return;
	}

	php_git2_array_to_git_signature(_author, author TSRMLS_CC);

	php_git2_array_to_git_signature(_committer, committer TSRMLS_CC);

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	result = git_note_remove(PHP_GIT2_V(_repo, repository), notes_ref, _author, _committer, &__oid);
	git_signature_free(_author);
	git_signature_free(_committer);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_note_free(resource $note)
 */
PHP_FUNCTION(git_note_free)
{
	zval *note = NULL;
	php_git2_t *_note = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &note) == FAILURE) {
		return;
	}

	if ((_note = (php_git2_t *) zend_fetch_resource(Z_RES_P(note), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_note)) {
		git_note_free(PHP_GIT2_V(_note, note));
		GIT2_SHOULD_FREE(_note) = 0;
	}

	zval_ptr_dtor(&note);
}
/* }}} */

/* {{{ proto string git_note_default_ref(resource $repo)
 */
PHP_FUNCTION(git_note_default_ref)
{
	char *out = NULL;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &repo) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_note_default_ref(&out, PHP_GIT2_V(_repo, repository));

	if (php_git2_check_error(error, "git_note_default_ref" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

/* {{{ proto long git_note_foreach(resource $repo, string $notes_ref, Callable $note_cb, void $payload)
 */
PHP_FUNCTION(git_note_foreach)
{
	int result;
	zval *repo = NULL, *payload = NULL;
	php_git2_t *_repo = NULL;
	char *notes_ref = NULL;
	size_t notes_ref_len;
	zend_fcall_info note_fci = empty_fcall_info;
	zend_fcall_info_cache note_fcc = empty_fcall_info_cache;
	php_git2_cb_t *note_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsfz", &repo, &notes_ref, &notes_ref_len, &note_fci, &note_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&note_cb, &note_fci, &note_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_note_foreach(PHP_GIT2_V(_repo, repository), notes_ref, php_git2_git_note_foreach_cb, payload_cb);
	php_git2_cb_free(note_cb);

	RETURN_LONG(result);
}
/* }}} */

