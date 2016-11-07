#include "php_git2.h"
#include "php_git2_priv.h"
#include "diff.h"

/* {{{ proto void git_diff_free(resource $diff)
 */
PHP_FUNCTION(git_diff_free)
{
	zval *diff = NULL;
	php_git2_t *_diff = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &diff) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	// todo rework
	if (0 /* GIT2_SHOULD_FREE(_diff) */) {
		git_diff_free(PHP_GIT2_V(_diff, diff));
		GIT2_SHOULD_FREE(_diff) = 0;
	}

	//zval_ptr_dtor(diff);
}
/* }}} */

/* {{{ proto resource git_diff_tree_to_tree(resource $repo, resource $old_tree, resource $new_tree, array $opts)
 */
PHP_FUNCTION(git_diff_tree_to_tree)
{
	php_git2_t *result = NULL, *_repo = NULL, *_old_tree = NULL, *_new_tree = NULL;
	git_diff *diff = NULL;
	zval *repo = NULL, *old_tree = NULL, *new_tree = NULL, *opts = NULL;
	git_diff_options _opts = GIT_DIFF_OPTIONS_INIT;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|ra", &repo, &old_tree, &new_tree, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_diff_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_old_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_new_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(new_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_diff_tree_to_tree(&diff, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_old_tree, tree), PHP_GIT2_V(_new_tree, tree), &_opts);

	if (php_git2_check_error(error, "git_diff_tree_to_tree" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_diff_options_free(&_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF, diff, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_diff_tree_to_index(resource $repo, resource $old_tree, resource $index, array $opts)
 */
PHP_FUNCTION(git_diff_tree_to_index)
{
	php_git2_t *result = NULL, *_repo = NULL, *_old_tree = NULL, *_index = NULL;
	git_diff *diff = NULL;
	zval *repo = NULL, *old_tree = NULL, *index = NULL, *opts = NULL;
	git_diff_options _opts = GIT_DIFF_OPTIONS_INIT;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rrr|a", &repo, &old_tree, &index, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_diff_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_old_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_diff_tree_to_index(&diff, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_old_tree, tree), PHP_GIT2_V(_index, index), &_opts);

	if (php_git2_check_error(error, "git_diff_tree_to_index" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_diff_options_free(&_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF, diff, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_diff_index_to_workdir(resource $repo, resource $index, array $opts)
 */
PHP_FUNCTION(git_diff_index_to_workdir)
{
	php_git2_t *result = NULL, *_repo = NULL, *_index = NULL;
	git_diff *diff = NULL;
	zval *repo = NULL, *index = NULL, *opts = NULL;
	git_diff_options _opts = GIT_DIFF_OPTIONS_INIT;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|a", &repo, &index, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_diff_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_index = (php_git2_t *) zend_fetch_resource(Z_RES_P(index), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_diff_index_to_workdir(&diff, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_index, index), &_opts);

	if (php_git2_check_error(error, "git_diff_index_to_workdir" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_diff_options_free(&_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF, diff, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_diff_tree_to_workdir(resource $repo, resource $old_tree, array $opts)
 */
PHP_FUNCTION(git_diff_tree_to_workdir)
{
	php_git2_t *result = NULL, *_repo = NULL, *_old_tree = NULL;
	git_diff *diff = NULL;
	zval *repo = NULL, *old_tree = NULL, *opts = NULL;
	git_diff_options _opts = GIT_DIFF_OPTIONS_INIT;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|a", &repo, &old_tree, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_diff_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_old_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_diff_tree_to_workdir(&diff, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_old_tree, tree), &_opts);

	if (php_git2_check_error(error, "git_diff_tree_to_workdir" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_diff_options_free(&_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF, diff, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto resource git_diff_tree_to_workdir_with_index(resource $repo, resource $old_tree, array $opts)
 */
PHP_FUNCTION(git_diff_tree_to_workdir_with_index)
{
	php_git2_t *result = NULL, *_repo = NULL, *_old_tree = NULL;
	git_diff *diff = NULL;
	zval *repo = NULL, *old_tree = NULL, *opts = NULL;
	git_diff_options _opts = GIT_DIFF_OPTIONS_INIT;
	int should_free = 0, error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr|a", &repo, &old_tree, &opts) == FAILURE) {
		return;
	}

	if (opts != NULL) {
		php_git2_array_to_git_diff_options(&_opts, opts TSRMLS_CC);
		should_free = 1;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_old_tree = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_tree), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_diff_tree_to_workdir_with_index(&diff, PHP_GIT2_V(_repo, repository), PHP_GIT2_V(_old_tree, tree), &_opts);

	if (php_git2_check_error(error, "git_diff_tree_to_workdir_with_index" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (should_free) {
		php_git2_git_diff_options_free(&_opts TSRMLS_CC);
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_DIFF, diff, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */

/* {{{ proto long git_diff_merge(resource $onto, resource $from)
 */
PHP_FUNCTION(git_diff_merge)
{
	int result;
	zval *onto = NULL, *from = NULL;
	php_git2_t *_onto = NULL, *_from = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rr", &onto, &from) == FAILURE) {
		return;
	}

	if ((_onto = (php_git2_t *) zend_fetch_resource(Z_RES_P(onto), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if ((_from = (php_git2_t *) zend_fetch_resource(Z_RES_P(from), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_diff_merge(PHP_GIT2_V(_onto, diff), PHP_GIT2_V(_from, diff));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_diff_find_similar(resource $diff, array $options)
 */
PHP_FUNCTION(git_diff_find_similar)
{
	int result, should_free = 0;
	zval *diff = NULL, *options = NULL;
	php_git2_t *_diff = NULL;
	git_diff_find_options _options = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r|a", &diff, &options) == FAILURE) {
		return;
	}

	if (options != NULL) {
		php_git2_array_to_git_diff_find_options(&_options, options TSRMLS_CC);
		should_free = 1;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_diff_find_similar(PHP_GIT2_V(_diff, diff), &_options);

	if (should_free) {
		php_git2_git_diff_find_options_free(&_options TSRMLS_CC);
	}

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_diff_options_init(long $version)
 */
PHP_FUNCTION(git_diff_options_init)
{
	int result = 0;
	git_diff_options options = {0};
	long version = GIT_DIFF_OPTIONS_VERSION;
	zval *out;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"|l", &version) == FAILURE) {
		return;
	}

	result = git_diff_options_init(&options, version);
	php_git2_git_diff_options_to_array(&options, out TSRMLS_CC);
	RETURN_ZVAL(out, 0, 1);
}
/* }}} */

/* {{{ proto long git_diff_num_deltas(resource $diff)
 */
PHP_FUNCTION(git_diff_num_deltas)
{
	size_t result;
	zval *diff = NULL;
	php_git2_t *_diff = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &diff) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_diff_num_deltas(PHP_GIT2_V(_diff, diff));

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_diff_num_deltas_of_type(resource $diff, long $type)
 */
PHP_FUNCTION(git_diff_num_deltas_of_type)
{
	size_t result;
	zval *diff = NULL;
	php_git2_t *_diff = NULL;
	zend_long type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &diff, &type) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_diff_num_deltas_of_type(PHP_GIT2_V(_diff, diff), type);

	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto array git_diff_get_delta(resource $diff, long $idx)
 */
PHP_FUNCTION(git_diff_get_delta)
{
	const git_diff_delta *result = NULL;
	zval *diff = NULL;
	php_git2_t *_diff = NULL;
	zend_long idx;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rl", &diff, &idx) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_diff_get_delta(PHP_GIT2_V(_diff, diff), idx);
	if (result == NULL) {
		RETURN_FALSE;
	}

	array_init(return_value);
	php_git2_git_diff_delta_to_array(result, return_value TSRMLS_CC);

	RETVAL_ARR(Z_ARRVAL_P(return_value));
}
/* }}} */

/* {{{ proto long git_diff_is_sorted_icase(resource $diff)
 */
PHP_FUNCTION(git_diff_is_sorted_icase)
{
	int result;
	zval *diff = NULL;
	php_git2_t *_diff = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r", &diff) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	result = git_diff_is_sorted_icase(PHP_GIT2_V(_diff, diff));

	RETURN_BOOL(result);
}
/* }}} */

/* {{{ proto long git_diff_foreach(resource $diff, Callable $file_cb, Callable $hunk_cb, Callable $line_cb, void $payload)
 */
PHP_FUNCTION(git_diff_foreach)
{
	int result = 0;
	zval *diff = NULL, *payload = NULL;
	php_git2_t *_diff = NULL;
	zend_fcall_info file_fci = empty_fcall_info;
	zend_fcall_info_cache file_fcc = empty_fcall_info_cache;
	zend_fcall_info hunk_fci = empty_fcall_info;
	zend_fcall_info_cache hunk_fcc = empty_fcall_info_cache;
	zend_fcall_info line_fci = empty_fcall_info;
	zend_fcall_info_cache line_fcc = empty_fcall_info_cache;
	php_git2_multi_cb_t *cb = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rfffz", &diff, &file_fci, &file_fcc, &hunk_fci, &hunk_fcc, &line_fci, &line_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_multi_cb_init(&cb, payload TSRMLS_CC, 6,
		&file_fci, &file_fcc,
		&hunk_fci, &hunk_fcc,
		&line_fci, &line_fcc
	);

	result = git_diff_foreach(PHP_GIT2_V(_diff, diff), php_git2_git_diff_file_cb, php_git2_git_diff_hunk_cb, php_git2_git_diff_line_cb, cb);
	php_git2_multi_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto string git_diff_status_char(long $status)
 */
PHP_FUNCTION(git_diff_status_char)
{
	char result;
	zend_long status;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l", &status) == FAILURE) {
		return;
	}

	result = git_diff_status_char(status);

	RETURN_STRING(&result);
}
/* }}} */

/* {{{ proto long git_diff_print(resource $diff, long $format, Callable $print_cb,  $payload)
 */
PHP_FUNCTION(git_diff_print)
{
	int result = 0;
	zval *diff = NULL, *payload = NULL;
	php_git2_t *_diff = NULL;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;
	php_git2_multi_cb_t *cb = NULL;
	long format = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rlfz", &diff, &format, &fci, &fcc, &payload) == FAILURE) {
		return;
	}

	if ((_diff = (php_git2_t *) zend_fetch_resource(Z_RES_P(diff), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	if (php_git2_multi_cb_init(&cb, payload TSRMLS_CC, 6,
                               &empty_fcall_info, &empty_fcall_info_cache,
                               &empty_fcall_info, &empty_fcall_info_cache,
                               &fci, &fcc)) {
		RETURN_FALSE;
	}

	result = git_diff_print(PHP_GIT2_V(_diff, diff), format, php_git2_git_diff_line_cb, cb);
	php_git2_multi_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

/* {{{ proto long git_diff_blobs(resource $old_blob, string $old_as_path, resource $new_blob, string $new_as_path,  $options, Callable $file_cb, Callable $hunk_cb, Callable $line_cb,  $payload)
 */
PHP_FUNCTION(git_diff_blobs)
{
	int result = 0, old_as_path_len = 0, new_as_path_len = 0;
	zval *old_blob = NULL, *new_blob = NULL, *options = NULL, *payload = NULL;
	php_git2_t *_old_blob = NULL, *_new_blob = NULL;
	char *old_as_path = NULL, *new_as_path = NULL;
	zend_fcall_info file_fci = empty_fcall_info;
	zend_fcall_info_cache file_fcc = empty_fcall_info_cache;
	zend_fcall_info hunk_fci = empty_fcall_info;
	zend_fcall_info_cache hunk_fcc = empty_fcall_info_cache;
	zend_fcall_info line_fci = empty_fcall_info;
	zend_fcall_info_cache line_fcc = empty_fcall_info_cache;
    php_git2_multi_cb_t *cb = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r!sr!sa!f!f!f!z", &old_blob, &old_as_path, &old_as_path_len, &new_blob, &new_as_path, &new_as_path_len, &options,
		  &file_fci, &file_fcc, &hunk_fci, &hunk_fcc, &line_fci, &line_fcc, &payload) == FAILURE) {
		return;
	}

    if (old_blob) {
        if ((_old_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

    }
    if (new_blob) {
        if ((_new_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(new_blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

    }
    
    php_git2_multi_cb_init(&cb, payload TSRMLS_CC, 6,
    	&file_fci, &file_fcc,
    	&hunk_fci, &hunk_fcc,
    	&line_fci, &line_fcc
    );
    
    result = git_diff_blobs(
        PHP_GIT2_V_N(_old_blob, blob), old_as_path,
        PHP_GIT2_V_N(_new_blob, blob), new_as_path, NULL,
        (ZEND_FCI_INITIALIZED(file_fci) ? php_git2_git_diff_file_cb : NULL),
        (ZEND_FCI_INITIALIZED(hunk_fci) ? php_git2_git_diff_hunk_cb : NULL),
        (ZEND_FCI_INITIALIZED(line_fci) ? php_git2_git_diff_line_cb : NULL),
        cb);
    php_git2_multi_cb_free(cb);
    if(!ZEND_FCI_INITIALIZED(file_fci)){
        RETURN_LONG(5);
    } else
    {
        
        RETURN_LONG(result);
    }
    
}
/* }}} */

/* {{{ proto long git_diff_blob_to_buffer(resource $old_blob, string $old_as_path, string $buffer, long $buffer_len, string $buffer_as_path,  $options, Callable $file_cb, Callable $hunk_cb, Callable $line_cb,  $payload)
 */
PHP_FUNCTION(git_diff_blob_to_buffer)
{
	int result = 0, old_as_path_len = 0, buffer_len = 0, buffer_as_path_len = 0;
	zval *old_blob = NULL, *options = NULL, *payload = NULL;
	php_git2_t *_old_blob = NULL;
	char *old_as_path = NULL, *buffer = NULL, *buffer_as_path = NULL;
	zend_fcall_info file_fci = empty_fcall_info;
	zend_fcall_info_cache file_fcc = empty_fcall_info_cache;
	zend_fcall_info hunk_fci = empty_fcall_info;
	zend_fcall_info_cache hunk_fcc = empty_fcall_info_cache;
	zend_fcall_info line_fci = empty_fcall_info;
	zend_fcall_info_cache line_fcc = empty_fcall_info_cache;
	php_git2_multi_cb_t *cb = NULL;
	git_diff_options opts = {0};

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rsslsafffz", &old_blob, &old_as_path, &old_as_path_len,
		&buffer, &buffer_len, &buffer_len, &buffer_as_path, &buffer_as_path_len, &options,
		&file_fci, &file_fcc, &hunk_fci, &hunk_fcc, &line_fci, &line_fcc, &payload) == FAILURE) {
		return;
	}

	if ((_old_blob = (php_git2_t *) zend_fetch_resource(Z_RES_P(old_blob), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	php_git2_multi_cb_init(&cb, payload TSRMLS_CC, 6,
		&file_fci, &file_fcc,
		&hunk_fci, &hunk_fcc,
		&line_fci, &line_fcc
	);

	result = git_diff_blob_to_buffer(
		PHP_GIT2_V(_old_blob, blob), old_as_path,
		buffer, buffer_len, buffer_as_path,
		&opts,
		php_git2_git_diff_file_cb, php_git2_git_diff_hunk_cb, php_git2_git_diff_line_cb, cb);
	php_git2_multi_cb_free(cb);
	RETURN_LONG(result);
}
/* }}} */

