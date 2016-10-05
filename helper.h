/*
 * PHP Libgit2 Extension
 *
 * https://github.com/libgit2/php-git
 *
 * Copyright 2014 Shuhei Tanuma.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef PHP_GIT2_HELPER_H
#define PHP_GIT2_HELPER_H

int php_git2_check_error(int error_code, const char *action TSRMLS_DC);

zval* php_git2_read_arrval(zval *array, char *name, size_t name_len TSRMLS_DC);

long php_git2_read_arrval_long2(zval *array, char *name, size_t name_len, long value TSRMLS_DC);

long php_git2_read_arrval_long(zval *array, char *name, size_t name_len TSRMLS_DC);

const char* php_git2_read_arrval_string(zval *array, char *name, size_t name_len TSRMLS_DC);

void php_git2_array_to_git_signature(git_signature *signature, zval *author TSRMLS_DC);

void php_git2_git_signature_to_array(const git_signature *signature, zval *out TSRMLS_DC);

void php_git2_git_strarray_to_array(git_strarray *array, zval *out TSRMLS_DC);

int php_git2_call_function_v(zend_fcall_info *fci, zend_fcall_info_cache *fcc TSRMLS_DC, zval *retval_ptr_ptr, uint32_t param_count, ...);

int php_git2_cb_init(php_git2_cb_t **out, zend_fcall_info *fci, zend_fcall_info_cache *fcc, void *payload TSRMLS_DC);

void php_git2_cb_free(php_git2_cb_t *target);

void php_git2_array_to_git_strarray(git_strarray *out, zval *array TSRMLS_DC);

void php_git2_git_strarray_free(git_strarray *out);

void php_git2_git_checkout_opts_to_array(git_checkout_opts *opts, zval *out TSRMLS_DC);

void php_git2_git_checkout_opts_free(git_checkout_opts *target TSRMLS_DC);

int php_git2_array_to_git_checkout_opts(git_checkout_opts **out, zval *array TSRMLS_DC);

int php_git2_multi_cb_init(php_git2_multi_cb_t **out, void *payload TSRMLS_DC, int num_callbacks, ...);

void php_git2_multi_cb_free(php_git2_multi_cb_t *target);

void php_git2_diff_line_to_array(const git_diff_line *line, zval *out TSRMLS_DC);

void php_git2_diff_hunk_to_array(const git_diff_hunk *hunk, zval *out TSRMLS_DC);

void php_git2_diff_file_to_array(const git_diff_file *file, zval *out TSRMLS_DC);

void php_git2_git_diff_delta_to_array(const git_diff_delta *delta, zval *out TSRMLS_DC);

void php_git2_array_to_git_diff_options(git_diff_options *options, zval *array TSRMLS_DC);

void php_git2_git_diff_options_free(git_diff_options *options);

void php_git2_git_diff_options_to_array(const git_diff_options *options, zval *out TSRMLS_DC);

int php_git2_git_diff_file_cb(const git_diff_delta *delta, float progress, void *payload);

int php_git2_git_diff_hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload);

int php_git2_git_diff_line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload);

int php_git2_git_blob_chunk_cb(char *content, size_t max_length, void *payload);

void php_git2_git_transfer_progress_to_array(git_transfer_progress *progress, zval *out TSRMLS_DC);

void php_git2_fcall_info_wrapper(zval *target, zend_fcall_info **out_fci, zend_fcall_info_cache **out_fcc TSRMLS_DC);

void php_git2_fcall_info_wrapper2(zval *target, zend_fcall_info *fci, zend_fcall_info_cache *fcc TSRMLS_DC);

void php_git2_array_to_git_blame_options(git_blame_options *options, zval *array TSRMLS_DC);

void php_git2_git_blame_options_to_array(git_blame_options *options, zval *out TSRMLS_DC);

void php_git2_git_blame_hunk_to_array(git_blame_hunk *hunk, zval *out TSRMLS_DC);

void php_git2_git_clone_options_to_array(git_clone_options *options, zval *out TSRMLS_DC);

void php_git2_array_to_git_clone_options(git_clone_options *options, zval *array TSRMLS_DC);

void php_git2_git_diff_find_options_to_array(git_diff_find_options *options, zval *out TSRMLS_DC);

void php_git2_array_to_git_diff_find_options(git_diff_find_options *options, zval *array TSRMLS_DC);

void php_git2_git_diff_find_options_free(git_diff_find_options *options);

void php_git2_git_push_options_to_array(git_push_options *options, zval *out TSRMLS_DC);

void php_git2_array_to_git_push_options(git_push_options *options, zval *array TSRMLS_DC);

void php_git2_git_push_options_free(git_push_options *options);

int php_git2_git_transport_cb(git_transport **out, git_remote *owner, void *param);

int php_git2_git_index_matched_path_cb(const char *path, const char *matched_pathspec, void *payload);

int php_git2_array_to_git_index_entry(git_index_entry *entry, zval *array TSRMLS_DC);

void php_git2_git_index_entry_to_array(const git_index_entry *entry, zval *result TSRMLS_DC);

int php_git2_git_tag_foreach_cb(const char *name, git_oid *oid, void *payload);

int php_git2_git_attr_foreach_cb(const char *name, const char *value, void *payload);

int php_git2_git_submodule_foreach_cb(git_submodule *sm, const char *name, void *payload);

void php_git2_git_error_to_array(const git_error *error, zval *out);

int php_git2_git_treebuilder_filter_cb(const git_tree_entry *entry, void *payload);

int php_git2_git_stash_cb(size_t index, const char* message, const git_oid *stash_id, void *payload);

int php_git2_git_note_foreach_cb(const git_oid *blob_id, const git_oid *annotated_object_id, void *payload);

void php_git2_array_to_git_transfer_progress(git_transfer_progress *progress, zval **array TSRMLS_DC);

int php_git2_git_cred_cb(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *data);

void php_git2_git_remote_head_to_array(git_remote_head *head, zval *out TSRMLS_DC);

int php_git2_git_reference_foreach_cb(git_reference *reference, void *payload);

int php_git2_git_reference_foreach_name_cb(const char *name, void *payload);

int php_git2_git_repository_fetchhead_foreach_cb(const char *ref_name, const char *remote_url, const git_oid *oid,unsigned int is_merge, void *payload);

int php_git2_git_repository_mergehead_foreach_cb(const git_oid *oid, void *payload);

int php_git2_git_remote_rename_problem_cb(const char *problematic_refspec, void *payload);

void php_git2_array_to_git_remote_callbacks(git_remote_callbacks *cb, zval *callbacks TSRMLS_DC);

int php_git2_git_treewalk_cb(const char *root, const git_tree_entry *entry, void *payload);

void php_git2_git_status_options_to_array(git_status_options *options, zval *out TSRMLS_DC);

void php_git2_array_to_git_status_options(git_status_options *options, zval *array TSRMLS_DC);

void php_git2_git_status_entry_to_array(git_status_entry *entry, zval *out TSRMLS_DC);

int php_git2_git_status_cb(const char *path, unsigned int status_flags, void *payload);

int php_git2_git_cred_sign_callback(void *payload, ...);

void php_git2_array_to_git_repository_init_options(git_repository_init_options *opts, zval *array TSRMLS_DC);

void php_git2_git_repository_init_options_to_array(git_repository_init_options *opts, zval *out TSRMLS_DC);

int php_git2_git_odb_foreach_cb(const git_oid *id, void *payload);

int php_git2_git_filter_check_fn(git_filter  *self, void **payload, const git_filter_source *src, const char **attr_values);

void php_git2_git_filter_shutdown_fn(git_filter *self);

int php_git2_git_filter_init_fn(git_filter *self);

int php_git2_git_filter_apply_fn(git_filter *self, void **payload, git_buf *to, const git_buf *from, const git_filter_source *src);

void php_git2_git_filter_cleanup_fn(git_filter *self, void *payload);

int php_git2_git_packbuilder_progress(int stage, unsigned int current, unsigned int total, void *payload);

int php_git2_git_packbuilder_foreach_cb(void *buf, size_t size, void *payload);

int php_git2_git_transfer_progress_callback(const git_transfer_progress *stats, void *payload);

int php_git2_git_push_status_foreach_cb(const char *ref, const char *msg, void *data);

void php_git2_git_revspec_to_array(const git_revspec *revspec, zval *out TSRMLS_DC);

int php_git2_odb_backend_foreach(git_odb_backend *backend, git_odb_foreach_cb cb, void *data);

int php_git2_odb_backend_read(void **buffer, size_t *size, git_otype *type, git_odb_backend *backend, const git_oid *oid);

int php_git2_odb_backend_write(git_odb_backend *backend, const git_oid *oid, const void *buffer, size_t size, git_otype type);

int php_git2_odb_backend_read_prefix(git_oid *out_oid, void **buffer_p, size_t *len_p, git_otype *type_p, git_odb_backend *backend, const git_oid *short_oid, size_t len);

int php_git2_odb_backend_read_header(size_t *len_p, git_otype *type_p, git_odb_backend *backend, const git_oid *oid);

int php_git2_odb_backend_writestream(git_odb_stream **stream_out, git_odb_backend *_backend, size_t length, git_otype type);

int php_git2_odb_backend_exists(git_odb_backend *backend, const git_oid *oid);

void git_ex_cb(INTERNAL_FUNCTION_PARAMETERS);

void php_git2_odb_backend_free(git_odb_backend *_backend);

void php_git2_odb_refresh(git_odb_backend *_backend);

int php_git2_git_push_transfer_progress(unsigned int current, unsigned int total, size_t bytes, void* payload);

#endif
