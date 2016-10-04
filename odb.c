#include "php_git2.h"
#include "php_git2_priv.h"
#include "odb.h"

/* {{{ proto resource git_odb_new()
 */
PHP_FUNCTION(git_odb_new)
{
	php_git2_t *result = NULL;
	git_odb *out = NULL;
	int error = 0;
	
	error = git_odb_new(&out);

	if (php_git2_check_error(error, "git_odb_new" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_odb_open(string $objects_dir)
 */
PHP_FUNCTION(git_odb_open)
{
	php_git2_t *result = NULL;
	git_odb *out = NULL;
	char *objects_dir = NULL;
	size_t objects_dir_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &objects_dir, &objects_dir_len) == FAILURE) {
		return;
	}

	error = git_odb_open(&out, objects_dir);

	if (php_git2_check_error(error, "git_odb_open" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_odb_add_disk_alternate(resource $odb, string $path)
 */
PHP_FUNCTION(git_odb_add_disk_alternate)
{
	int result;
	zval *odb = NULL;
	php_git2_t *_odb = NULL;
	char *path = NULL;
	size_t path_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &odb, &path, &path_len) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_add_disk_alternate(PHP_GIT2_V(_odb, odb), path);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_odb_free(resource $db)
 */
PHP_FUNCTION(git_odb_free)
{
	zval *db = NULL;
	php_git2_t *_db = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &db) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_db)) {
		git_odb_free(PHP_GIT2_V(_db, odb));
		GIT2_SHOULD_FREE(_db) = 0;
	);

	zval_ptr_dtor(&db);
}
/* }}} */

/* {{{ proto resource git_odb_read(resource $db, string $id)
 */
PHP_FUNCTION(git_odb_read)
{
	php_git2_t *result = NULL, *_db = NULL;
	git_odb_object *out = NULL;
	zval *db = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &db, &id, &id_len) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	error = git_odb_read(&out, PHP_GIT2_V(_db, odb), &__id);

	if (php_git2_check_error(error, "git_odb_read" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_OBJECT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_odb_read_prefix(resource $db, string $short_id, long $len)
 */
PHP_FUNCTION(git_odb_read_prefix)
{
	php_git2_t *result = NULL, *_db = NULL;
	git_odb_object *out = NULL;
	zval *db = NULL;
	char *short_id = NULL;
	size_t short_id_len;
	git_oid __short_id = {0};
	zend_long len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &db, &short_id, &short_id_len, &len) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__short_id, short_id, short_id_len)) {
		RETURN_FALSE;
	}

	error = git_odb_read_prefix(&out, PHP_GIT2_V(_db, odb), &__short_id, len);

	if (php_git2_check_error(error, "git_odb_read_prefix" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_OBJECT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_odb_read_header( $type_out, resource $db, string $id)
 */
PHP_FUNCTION(git_odb_read_header)
{
	php_git2_t *_db = NULL;
	size_t len_out = NULL;
	zval *db = NULL, *_result = NULL;
	git_otype type_out;
	char *id = NULL;
	int id_len = 0, error = 0;
	git_oid __id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &db, &id, &id_len) == FAILURE) {
		return;
	}
	
	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}
	error = git_odb_read_header(&len_out, &type_out, PHP_GIT2_V(_db, odb), &__id);
	if (php_git2_check_error(error, "git_odb_read_header" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	MAKE_STD_ZVAL(_result);
	array_init(_result);
	add_next_index_long(_result, len_out);
	add_next_index_long(_result, type_out);
	RETURN_ZVAL(_result, 0, 1);
}
/* }}} */

/* {{{ proto long git_odb_exists(resource $db, string $id)
 */
PHP_FUNCTION(git_odb_exists)
{
	int result;
	zval *db = NULL;
	php_git2_t *_db = NULL;
	char *id = NULL;
	size_t id_len;
	git_oid __id = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &db, &id, &id_len) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__id, id, id_len)) {
		RETURN_FALSE;
	}

	result = git_odb_exists(PHP_GIT2_V(_db, odb), &__id);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_odb_refresh(resource $db)
 */
PHP_FUNCTION(git_odb_refresh)
{
	int result;
	zval *db = NULL;
	php_git2_t *_db = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &db) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_refresh(PHP_GIT2_V(_db, odb));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_odb_foreach(resource $db, Callable $cb, void $payload)
 */
PHP_FUNCTION(git_odb_foreach)
{
	int result;
	zval *db = NULL, *payload = NULL;
	php_git2_t *_db = NULL;
	zend_fcall_info cb_fci = empty_fcall_info;
	zend_fcall_info_cache cb_fcc = empty_fcall_info_cache;
	php_git2_cb_t *cb_cb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &db, &cb_fci, &cb_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&cb_cb, &cb_fci, &cb_fcc, payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	result = git_odb_foreach(PHP_GIT2_V(_db, odb), php_git2_git_odb_foreach_cb, payload_cb);
	php_git2_cb_free(cb_cb);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_odb_write(resource $odb,  $data,  $type)
 */
PHP_FUNCTION(git_odb_write)
{
	php_git2_t *_odb = NULL;
	git_oid out = {0};
	zval *odb = NULL;
	zval *data = NULL;
	long type = 0;
	int error = 0, data_len = 0;
	char buf[GIT2_OID_HEXSIZE] = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &odb, &data, &data_len, &type) == FAILURE) {
		return;
	}
	
	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_odb_write(&out, PHP_GIT2_V(_odb, odb), data, data_len, type);
	if (php_git2_check_error(error, "git_odb_write" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	git_oid_fmt(buf, &out);
	RETURN_STRING(buf);
}
/* }}} */

/* {{{ proto resource git_odb_open_wstream(resource $db, long $size, long $type)
 */
PHP_FUNCTION(git_odb_open_wstream)
{
	php_git2_t *result = NULL, *_db = NULL;
	git_odb_stream *out = NULL;
	zval *db = NULL;
	zend_long size, type;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rll", &db, &size, &type) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_odb_open_wstream(&out, PHP_GIT2_V(_db, odb), size, type);

	if (php_git2_check_error(error, "git_odb_open_wstream" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_STREAM, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_odb_stream_write(resource $stream, string $buffer)
 */
PHP_FUNCTION(git_odb_stream_write)
{
	int result = 0, buffer_len = 0;
	zval *stream = NULL;
	php_git2_t *_stream = NULL;
	char *buffer = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &stream, &buffer, &buffer_len) == FAILURE) {
		return;
	}
	
	if ((_stream = (php_git2_t *) zend_fetch_resource(Z_RES_P(stream), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_stream_write(PHP_GIT2_V(_stream, odb_stream), buffer, buffer_len);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_odb_stream_finalize_write(resource $stream)
 */
PHP_FUNCTION(git_odb_stream_finalize_write)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	zval *stream = NULL;
	php_git2_t *_stream = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &stream) == FAILURE) {
		return;
	}

	if ((_stream = (php_git2_t *) zend_fetch_resource(Z_RES_P(stream), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_odb_stream_finalize_write(&out, PHP_GIT2_V(_stream, odb_stream));

	if (php_git2_check_error(error, "git_odb_stream_finalize_write" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto long git_odb_stream_read(resource $stream, string $buffer, long $len)
 */
PHP_FUNCTION(git_odb_stream_read)
{
	int result = 0, buffer_len = 0;
	zval *stream = NULL;
	php_git2_t *_stream = NULL;
	char *buffer = NULL;
	long len = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsl", &stream, &buffer, &buffer_len, &len) == FAILURE) {
		return;
	}

	if ((_stream = (php_git2_t *) zend_fetch_resource(Z_RES_P(stream), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_stream_read(PHP_GIT2_V(_stream, odb_stream), buffer, len);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto void git_odb_stream_free(resource $stream)
 */
PHP_FUNCTION(git_odb_stream_free)
{
	zval *stream = NULL;
	php_git2_t *_stream = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &stream) == FAILURE) {
		return;
	}

	if ((_stream = (php_git2_t *) zend_fetch_resource(Z_RES_P(stream), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (GIT2_SHOULD_FREE(_stream)) {
		git_odb_stream_free(PHP_GIT2_V(_stream, odb_stream));
		GIT2_SHOULD_FREE(_stream) = 0;
	);

	zval_ptr_dtor(&stream);
}
/* }}} */

/* {{{ proto resource git_odb_open_rstream(resource $db, string $oid)
 */
PHP_FUNCTION(git_odb_open_rstream)
{
	php_git2_t *result = NULL, *_db = NULL;
	git_odb_stream *out = NULL;
	zval *db = NULL;
	char *oid = NULL;
	size_t oid_len;
	git_oid __oid = {0};
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &db, &oid, &oid_len) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (git_oid_fromstrn(&__oid, oid, oid_len)) {
		RETURN_FALSE;
	}

	error = git_odb_open_rstream(&out, PHP_GIT2_V(_db, odb), &__oid);

	if (php_git2_check_error(error, "git_odb_open_rstream" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_STREAM, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_odb_write_pack(resource $db, Callable $progress_cb, void $progress_payload)
 */
PHP_FUNCTION(git_odb_write_pack)
{
	php_git2_t *result = NULL, *_db = NULL;
	git_odb_writepack *out = NULL;
	zval *db = NULL, *progress_payload = NULL;
	zend_fcall_info progress_fci = empty_fcall_info;
	zend_fcall_info_cache progress_fcc = empty_fcall_info_cache;
	php_git2_cb_t *progress_cb = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfz", &db, &progress_fci, &progress_fcc, &progress_payload) == FAILURE) {
		return;
	}

	if ((_db = (php_git2_t *) zend_fetch_resource(Z_RES_P(db), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_cb_init(&progress_cb, &progress_fci, &progress_fcc, progress_payload TSRMLS_CC)) {
		RETURN_FALSE;
	}

	error = git_odb_write_pack(&out, PHP_GIT2_V(_db, odb), php_git2_git_transfer_progress_callback, progress_cb);

	if (php_git2_check_error(error, "git_odb_write_pack" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	php_git2_cb_free(progress_cb);

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_WRITEPACK, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_odb_hash(string $data, long $type)
 */
PHP_FUNCTION(git_odb_hash)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	char *data = NULL;
	zend_long len, type;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &data, &len, &type) == FAILURE) {
		return;
	}

	error = git_odb_hash(&out, data, len, type);

	if (php_git2_check_error(error, "git_odb_hash" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto string git_odb_hashfile(string $path, long $type)
 */
PHP_FUNCTION(git_odb_hashfile)
{
	git_oid out;
	char __out[GIT2_OID_HEXSIZE] = {0};
	char *path = NULL;
	size_t path_len;
	zend_long type;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &path, &path_len, &type) == FAILURE) {
		return;
	}

	error = git_odb_hashfile(&out, path, type);

	if (php_git2_check_error(error, "git_odb_hashfile" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	git_oid_fmt(__out, &out);

	RETURN_STRING(__out);
}
/* }}} */

/* {{{ proto resource git_odb_object_dup(resource $source)
 */
PHP_FUNCTION(git_odb_object_dup)
{
	php_git2_t *result = NULL, *_source = NULL;
	git_odb_object *dest = NULL;
	zval *source = NULL;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &source) == FAILURE) {
		return;
	}

	if ((_source = (php_git2_t *) zend_fetch_resource(Z_RES_P(source), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_odb_object_dup(&dest, PHP_GIT2_V(_source, odb_object));

	if (php_git2_check_error(error, "git_odb_object_dup" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_OBJECT, dest, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto void git_odb_object_free(resource $object)
 */
PHP_FUNCTION(git_odb_object_free)
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
		git_odb_object_free(PHP_GIT2_V(_object, odb_object));
		GIT2_SHOULD_FREE(_object) = 0;
	);

	zval_ptr_dtor(&object);
}
/* }}} */

/* {{{ proto string git_odb_object_id(resource $object)
 */
PHP_FUNCTION(git_odb_object_id)
{
	const git_oid *result = NULL;
	char __result[GIT2_OID_HEXSIZE] = {0};
	zval *object = NULL;
	php_git2_t *_object = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &object) == FAILURE) {
		return;
	}

	if ((_object = (php_git2_t *) zend_fetch_resource(Z_RES_P(object), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_object_id(PHP_GIT2_V(_object, odb_object));

	git_oid_fmt(__result, result);

	RETURN_STRING(__result);
}
/* }}} */

/* {{{ proto string git_odb_object_data(resource $object)
 */
PHP_FUNCTION(git_odb_object_data)
{
	const char *buffer = NULL;
	zval *object = NULL;
	php_git2_t *_object = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &object) == FAILURE) {
		return;
	}

	if ((_object = (php_git2_t *) zend_fetch_resource(Z_RES_P(object), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	buffer = git_odb_object_data(PHP_GIT2_V(_object, odb_object));

	RETURN_STRINGL(buffer, git_odb_object_size(PHP_GIT2_V(_object, odb_object)));
}
/* }}} */

/* {{{ proto long git_odb_object_size(resource $object)
 */
PHP_FUNCTION(git_odb_object_size)
{
	size_t result;
	zval *object = NULL;
	php_git2_t *_object = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &object) == FAILURE) {
		return;
	}

	if ((_object = (php_git2_t *) zend_fetch_resource(Z_RES_P(object), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_object_size(PHP_GIT2_V(_object, odb_object));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_odb_object_type(resource $object)
 */
PHP_FUNCTION(git_odb_object_type)
{
	git_otype result;
	zval *object = NULL;
	php_git2_t *_object = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &object) == FAILURE) {
		return;
	}

	if ((_object = (php_git2_t *) zend_fetch_resource(Z_RES_P(object), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_object_type(PHP_GIT2_V(_object, odb_object));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_odb_add_backend(resource $odb, resource $backend, long $priority)
 */
PHP_FUNCTION(git_odb_add_backend)
{
	int result;
	zval *odb = NULL, *backend = NULL;
	php_git2_t *_odb = NULL, *_backend = NULL;
	zend_long priority;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrl", &odb, &backend, &priority) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_backend = (php_git2_t *) zend_fetch_resource(Z_RES_P(backend), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_add_backend(PHP_GIT2_V(_odb, odb), PHP_GIT2_V(_backend, odb_backend), priority);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_odb_add_alternate(resource $odb, resource $backend, long $priority)
 */
PHP_FUNCTION(git_odb_add_alternate)
{
	int result;
	zval *odb = NULL, *backend = NULL;
	php_git2_t *_odb = NULL, *_backend = NULL;
	zend_long priority;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrl", &odb, &backend, &priority) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_backend = (php_git2_t *) zend_fetch_resource(Z_RES_P(backend), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_add_alternate(PHP_GIT2_V(_odb, odb), PHP_GIT2_V(_backend, odb_backend), priority);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_odb_num_backends(resource $odb)
 */
PHP_FUNCTION(git_odb_num_backends)
{
	size_t result;
	zval *odb = NULL;
	php_git2_t *_odb = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &odb) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_odb_num_backends(PHP_GIT2_V(_odb, odb));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto resource git_odb_get_backend(resource $odb, long $pos)
 */
PHP_FUNCTION(git_odb_get_backend)
{
	php_git2_t *result = NULL, *_odb = NULL;
	git_odb_backend *out = NULL;
	zval *odb = NULL;
	zend_long pos;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &odb, &pos) == FAILURE) {
		return;
	}

	if ((_odb = (php_git2_t *) zend_fetch_resource(Z_RES_P(odb), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_odb_get_backend(&out, PHP_GIT2_V(_odb, odb), pos);

	if (php_git2_check_error(error, "git_odb_get_backend" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_BACKEND, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto array git_odb_backend_new()
 */
PHP_FUNCTION(git_odb_backend_new)
{
	php_git2_odb_backend *backend;
	php_git2_t *result;
	zval *callbacks, *tmp;
	zend_fcall_info read_fci, write_fci, read_prefix_fci, read_header_fci, writestream_fci,
		exists_fci, foreach_fci, free_fci, refresh_fci;
	zend_fcall_info_cache read_fcc, write_fcc, read_prefix_fcc, read_header_fcc, writestream_fcc,
		exists_fcc, foreach_fcc, free_fcc, refresh_fcc;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"a", &callbacks) == FAILURE) {
		return;
	}

	backend = ecalloc(1, sizeof(php_git2_odb_backend));
	backend->parent.version = GIT_ODB_BACKEND_VERSION;
	backend->parent.read = &php_git2_odb_backend_read;
	backend->parent.write = &php_git2_odb_backend_write;
	backend->parent.read_prefix = &php_git2_odb_backend_read_prefix;
	backend->parent.read_header = &php_git2_odb_backend_read_header;
	//backend->parent.writestream = &php_git2_odb_backend_writestream;
	backend->parent.exists = &php_git2_odb_backend_exists;
	backend->parent.foreach = &php_git2_odb_backend_foreach;
	backend->parent.free = &php_git2_odb_backend_free;
	backend->parent.refresh = &php_git2_odb_refresh;

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("read") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &read_fci, &read_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("write") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &write_fci, &write_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("read_header") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &read_header_fci, &read_header_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("read_prefix") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &read_prefix_fci, &read_prefix_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("exists") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &exists_fci, &exists_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("foreach") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &foreach_fci, &foreach_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("free") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &free_fci, &free_fcc TSRMLS_CC);
	}

	tmp = php_git2_read_arrval(callbacks, ZEND_STRS("refresh") TSRMLS_CC);
	if (tmp) {
		php_git2_fcall_info_wrapper2(tmp, &refresh_fci, &refresh_fcc TSRMLS_CC);
	}

	Z_ADDREF_P(callbacks);
	php_git2_multi_cb_init(&backend->multi, callbacks TSRMLS_CC, 9,
		&read_fci, &read_fcc,
		&write_fci, &write_fcc,
		&read_prefix_fci, &read_prefix_fcc,
		&read_header_fci, &read_header_fcc,
		&writestream_fci, &writestream_fcc,
		&exists_fci, &exists_fcc,
		&foreach_fci, &foreach_fcc,
		&free_fci, &free_fcc,
		&refresh_fci, &refresh_fcc
	);

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_BACKEND, backend, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));
}
/* }}} */

