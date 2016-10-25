#include "php_git2.h"
#include "php_git2_priv.h"
#include "helper.h"

int php_git2_check_error(int error_code, const char *action TSRMLS_DC)
{
	int result = 0;
	const git_error * error;
	if (!error_code) {
		return result;
	}

	error = giterr_last();
	php_error_docref(NULL TSRMLS_CC, E_WARNING, "WARNING %d %s - %s",
		error_code, action, (error && error->message) ? error->message : "???");

	result = 1;
	return result;
}

zval* php_git2_read_arrval(zval *array, char *name, size_t name_len TSRMLS_DC)
{
	zval *result = NULL, *element;
    zend_string *str = zend_string_init(name, name_len, 0);

	if ((element = zend_hash_find(Z_ARRVAL_P(array), str)) != NULL) {
		result = element;
	}

	return result;
}

long php_git2_read_arrval_long2(zval *array, char *name, size_t name_len, long value TSRMLS_DC)
{
	zval *tmp;
	long result = value;

	tmp = php_git2_read_arrval(array, name, name_len TSRMLS_CC);
	if (tmp) {
		result = Z_LVAL_P(tmp);
	}

	return result;
}

long php_git2_read_arrval_long(zval *array, char *name, size_t name_len TSRMLS_DC)
{
	zval *tmp;
	long result = 0;

	tmp = php_git2_read_arrval(array, name, name_len TSRMLS_CC);
	if (tmp) {
		result = Z_LVAL_P(tmp);
	}

	return result;
}

char* php_git2_read_arrval_string(zval *array, char *name, size_t name_len TSRMLS_DC)
{
	zval *tmp;
	char *result = NULL;

	tmp = php_git2_read_arrval(array, name, name_len TSRMLS_CC);
	if (tmp) {
		result = Z_STRVAL_P(tmp);
	}

	return result;
}

void php_git2_array_to_git_signature(git_signature *signature, zval *author TSRMLS_DC)
{
	zval *name = NULL, *email = NULL, *time = NULL;

	name  = php_git2_read_arrval(author, ZEND_STRL("name") TSRMLS_CC);
	email = php_git2_read_arrval(author, ZEND_STRL("email") TSRMLS_CC);
	time  = php_git2_read_arrval(author, ZEND_STRL("time") TSRMLS_CC);

	signature->name = Z_STRVAL_P(name);
	signature->email = Z_STRVAL_P(email);

	//instanceof_function_ex(const zend_class_entry *instance_ce, const zend_class_entry *ce, zend_bool interfaces_only TSRMLS_DC);
	if (time != NULL &&
		Z_TYPE_P(time) == IS_OBJECT &&
		instanceof_function_ex(Z_OBJCE_P(time), php_date_get_date_ce(), 0 TSRMLS_CC)) {
		php_date_obj *date;

		date = (php_date_obj *) Z_OBJ_P(time);
		signature->when.time = date->time->sse;
		signature->when.offset = date->time->z;
	}
}

void php_git2_git_signature_to_array(const git_signature *signature, zval *out TSRMLS_DC)
{
	if (signature->name == NULL) {
		add_assoc_null_ex(out, ZEND_STRL("name"));
	} else {
		add_assoc_string_ex(out, ZEND_STRL("name"), signature->name);
	}

	if (signature->email == NULL) {
		add_assoc_null_ex(out, ZEND_STRL("email"));
	} else {
		add_assoc_string_ex(out, ZEND_STRL("email"), signature->email);
	}

	add_assoc_long_ex(out, ZEND_STRL("time"), signature->when.time);
}

void php_git2_git_strarray_to_array(git_strarray *array, zval *out TSRMLS_DC)
{
	int i = 0;

	array_init(out);

	for (i = 0; i < array->count; i++) {
		add_next_index_string(out, array->strings[i]);
	}
}

int php_git2_call_function_v(zend_fcall_info *fci, zend_fcall_info_cache *fcc TSRMLS_DC, zval *retval, uint32_t param_count, ...)
{
	zval *params = NULL, *param;
	va_list ap;
	int i = 0;

	if (param_count > 0) {
		params = emalloc(sizeof(zval) * param_count);
		va_start(ap, param_count);
		for (i = 0; i < param_count; i++) {
		    param = va_arg(ap, zval*);
			params[i] = *param;
		}
		va_end(ap);
	} else {
		params = NULL;
	}

	if (ZEND_FCI_INITIALIZED(*fci)) {
		fci->params         = params;
		fci->retval         = retval;
		fci->param_count    = param_count;
		fci->no_separation  = 1;

		if (zend_call_function(fci, fcc TSRMLS_CC) != SUCCESS) {
			if (param_count > 0) {
				for (i = 0; i < param_count; i++) {
					zval_ptr_dtor(&params[i]);
				}
				efree(params);
			}
			return 1;
		}
		zend_fcall_info_args_clear(fci, 0);
	}

	if (param_count > 0) {
		for (i = 0; i < param_count; i++) {
			zval_ptr_dtor(&params[i]);
		}
		efree(params);
	}
	return 0;
}

int php_git2_cb_init(php_git2_cb_t **out, zend_fcall_info *fci, zend_fcall_info_cache *fcc, void *payload TSRMLS_DC)
{
	php_git2_cb_t *cb;

	cb = (struct php_git2_cb_t*)emalloc(sizeof(php_git2_cb_t));
	if (cb == NULL) {
		return 1;
	}

	cb->payload = payload;
	cb->fci = fci;
	cb->fcc = fcc;
	GIT2_TSRMLS_SET2(cb, TSRMLS_C);

	*out = cb;
	return 0;
}

void php_git2_cb_free(php_git2_cb_t *target)
{
	efree(target);
}

void php_git2_array_to_git_strarray(git_strarray *out, zval *array TSRMLS_DC)
{
	int elements = 0, i;
	zval *value;

	if (array == NULL) {
		return;
	}
    
	if (Z_TYPE_P(array) != IS_ARRAY){
		return;
	}
    
	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		return;
	}

	elements = zend_hash_num_elements(Z_ARRVAL_P(array));
	out->strings = (char**)emalloc(sizeof(char*) * elements);
	out->count = elements;

	i = 0;
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), value) {
		char *buffer;

		Z_STRVAL_P(value);
		buffer = emalloc(sizeof(char*) * Z_STRLEN_P(value) + 1);
		memcpy(buffer, Z_STRVAL_P(value), Z_STRLEN_P(value));
		buffer[Z_STRLEN_P(value)] = '\0';
		out->strings[i] = buffer;
		i++;
    } ZEND_HASH_FOREACH_END();
}

void php_git2_git_strarray_free(git_strarray *out)
{
	int i = 0;
	for (i = 0; i < out->count; i++) {
		efree(out->strings[i]);
	}
	efree(out->strings);
}

void php_git2_git_checkout_opts_to_array(git_checkout_opts *opts, zval *out TSRMLS_DC)
{
    git_checkout_opts tmp = GIT_CHECKOUT_OPTS_INIT;
    zval paths;

	if (!opts) {
	    opts = &tmp;
	}

	add_assoc_long_ex(out, ZEND_STRL("version"), opts->version);
	add_assoc_long_ex(out, ZEND_STRL("checkout_strategy"), opts->checkout_strategy);
	add_assoc_long_ex(out, ZEND_STRL("disable_filters"), opts->disable_filters);
	add_assoc_long_ex(out, ZEND_STRL("dir_mode"), opts->dir_mode);
	add_assoc_long_ex(out, ZEND_STRL("file_mode"), opts->file_mode);
	add_assoc_long_ex(out, ZEND_STRL("file_open_flags"), opts->file_open_flags);
	add_assoc_long_ex(out, ZEND_STRL("notify_flags"), opts->notify_flags);

	if (opts->notify_payload != NULL) {

	} else {
		add_assoc_null_ex(out, ZEND_STRL("notify_cb"));
	}

	if (opts->notify_payload != NULL) {

	} else {
		add_assoc_null_ex(out, ZEND_STRL("notify_payload"));
	}

	if (opts->progress_cb != NULL) {

	} else {
		add_assoc_null_ex(out, ZEND_STRL("progress_cb"));
	}

	if (opts->progress_payload != NULL) {

	} else {
		add_assoc_null_ex(out, ZEND_STRL("progress_payload"));
	}

	if (opts->paths.count > 0) {
		php_git2_git_strarray_to_array(&opts->paths, &paths TSRMLS_CC);
	} else {
		array_init(&paths);
	}
	add_assoc_zval_ex(out, ZEND_STRL("paths"), &paths);

	if (opts->baseline != NULL) {
		// git_tree
	} else {
		add_assoc_null_ex(out, ZEND_STRL("baseline"));
	}

	add_assoc_string_ex(out, ZEND_STRL("target_directory"), (opts->target_directory) ? strdup(opts->target_directory) : "");
	add_assoc_string_ex(out, ZEND_STRL("our_label"), (opts->our_label) ? strdup(opts->our_label) : "");
	add_assoc_string_ex(out, ZEND_STRL("their_label"), (opts->their_label) ? strdup(opts->their_label) : "");
}

void php_git2_git_checkout_opts_free(git_checkout_opts *target TSRMLS_DC)
{
    // todo rework needed
	/* php_git2_cb_t *tmp;

	if (target->notify_payload) {
		tmp = (php_git2_cb_t*)target->notify_payload;
		if (tmp->fci) {
			efree(tmp->fci);
		}
		if (tmp->fcc) {
			efree(tmp->fcc);
		}
		efree(target->notify_payload);
	}
	if (target->progress_payload) {
		tmp = (php_git2_cb_t*)target->progress_payload;
		if (tmp->fci) {
			efree(tmp->fci);
		}
		if (tmp->fcc) {
			efree(tmp->fcc);
		}
		efree(target->progress_payload);
	}

	php_git2_git_strarray_free(&target->paths);
	efree(target); */
}

int php_git2_git_checkout_notify_cb(git_checkout_notify_t why, const char *path, const git_diff_file *baseline,
        const git_diff_file *target, const git_diff_file *workdir, void *payload)
{
	/* TODO(chobie): implement callback */

	return 0;
}

void php_git2_git_checkout_progress_cb(const char *path, size_t completed_steps, size_t total_steps, void *payload)
{
	zval *param_path, *param_completed_steps, *param_total_steps, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	GIT2_TSRMLS_SET(p->tsrm_ls);

	Z_ADDREF_P(p->payload);
	ZVAL_NULL(param_path);

	if (path != NULL) {
		ZVAL_STRING(param_path, path);
	}

	ZVAL_LONG(param_completed_steps, completed_steps);
	ZVAL_LONG(param_total_steps, total_steps);

	SEPARATE_ZVAL(p->payload);
	ZVAL_MAKE_REF(p->payload);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 4, param_path, param_completed_steps, param_total_steps, p->payload)) {
		return;
	}

	zval_ptr_dtor(retval_ptr);
}

void php_git2_fcall_info_wrapper(zval *target, zend_fcall_info **out_fci, zend_fcall_info_cache **out_fcc TSRMLS_DC)
{
	char *is_callable_error = NULL;
	zend_fcall_info *fci = NULL;
	zend_fcall_info_cache *fcc = NULL;

	fci = (zend_fcall_info*)emalloc(sizeof(zend_fcall_info));
	fcc = (zend_fcall_info_cache*)emalloc(sizeof(zend_fcall_info_cache));
	memcpy(fci, &empty_fcall_info, sizeof(zend_fcall_info));
	memcpy(fcc, &empty_fcall_info_cache, sizeof(zend_fcall_info_cache));

	if (zend_fcall_info_init(target, 0, fci, fcc, NULL, &is_callable_error TSRMLS_CC) == SUCCESS) {
		if (is_callable_error) {
			efree(is_callable_error);
		}
	} else {
		fprintf(stderr, "FAILED");
		efree(fci);
		efree(fcc);
		return;
	}

	*out_fci = fci;
	*out_fcc = fcc;
}

void php_git2_fcall_info_wrapper2(zval *target, zend_fcall_info *fci, zend_fcall_info_cache *fcc TSRMLS_DC)
{
	char *is_callable_error = NULL;

	memcpy(fci, &empty_fcall_info, sizeof(zend_fcall_info));
	memcpy(fcc, &empty_fcall_info_cache, sizeof(zend_fcall_info_cache));

	if (zend_fcall_info_init(target, 0, fci, fcc, NULL, &is_callable_error TSRMLS_CC) == SUCCESS) {
		if (is_callable_error) {
			efree(is_callable_error);
		}
	} else {
		fprintf(stderr, "FAILED");
		efree(fci);
		efree(fcc);
		return;
	}
}

int php_git2_array_to_git_checkout_opts(git_checkout_opts *out, zval *array TSRMLS_DC)
{
	/*php_git2_cb_t *notify_payload = NULL, *progress_payload= NULL;
	zval *notify_cb = NULL, *progress_cb = NULL;

	notify_cb = php_git2_read_arrval(array, ZEND_STRL("notify_cb") TSRMLS_CC);
	progress_cb = php_git2_read_arrval(array, ZEND_STRL("progress_cb") TSRMLS_CC);
	out->notify_cb = php_git2_git_checkout_notify_cb;
	out->progress_cb = php_git2_git_checkout_progress_cb;*/

	out->version = php_git2_read_arrval_long(array, ZEND_STRL("version") TSRMLS_CC);
	out->checkout_strategy = php_git2_read_arrval_long(array, ZEND_STRL("checkout_strategy") TSRMLS_CC);
	out->disable_filters = php_git2_read_arrval_long(array, ZEND_STRL("disable_filters") TSRMLS_CC);
	out->dir_mode = php_git2_read_arrval_long(array, ZEND_STRL("dir_mode") TSRMLS_CC);
	out->file_mode = php_git2_read_arrval_long(array, ZEND_STRL("file_mode") TSRMLS_CC);
	out->file_open_flags = php_git2_read_arrval_long(array, ZEND_STRL("file_open_flags") TSRMLS_CC);
	out->notify_flags = php_git2_read_arrval_long(array, ZEND_STRL("notify_flags") TSRMLS_CC);

	/*//notify_cb
	if (Z_TYPE_P(notify_cb) != IS_NULL) {
		zend_fcall_info *fci;
		zend_fcall_info_cache *fcc;

		php_git2_fcall_info_wrapper(notify_cb, &fci, &fcc TSRMLS_CC);
		if (php_git2_cb_init(&notify_payload, fci, fcc,
		 	php_git2_read_arrval(array, ZEND_STRL("notify_payload") TSRMLS_CC) TSRMLS_CC)) {
		}
		opts->notify_payload = notify_payload;
	} else {
		opts->notify_cb = NULL;
	}

	//progress_cb
	if (Z_TYPE_P(progress_cb) != IS_NULL) {
		zend_fcall_info *fci;
		zend_fcall_info_cache *fcc;

		php_git2_fcall_info_wrapper(progress_cb, &fci, &fcc TSRMLS_CC);
		if (php_git2_cb_init(&progress_payload, fci, fcc,
		 	php_git2_read_arrval(array, ZEND_STRL("progress_payload") TSRMLS_CC) TSRMLS_CC)) {
		}
		opts->progress_payload = progress_payload;
	} else {
		opts->progress_cb = NULL;
	}

	php_git2_array_to_git_strarray(&opts->paths, php_git2_read_arrval(array, ZEND_STRL("paths") TSRMLS_CC) TSRMLS_CC);*/

	out->target_directory = php_git2_read_arrval_string(array, ZEND_STRL("target_directory") TSRMLS_CC);
	out->our_label = php_git2_read_arrval_string(array, ZEND_STRL("our_label") TSRMLS_CC);
	out->their_label = php_git2_read_arrval_string(array, ZEND_STRL("their_label") TSRMLS_CC);

	return 0;
}

int php_git2_multi_cb_init(php_git2_multi_cb_t **out, void *payload TSRMLS_DC, int num_args, ...)
{
    int num_callbacks = num_args / 2;
	php_git2_multi_cb_t *cb;
	int i = 0;
	va_list ap;

	cb = (struct php_git2_multi_cb_t*)emalloc(sizeof(php_git2_multi_cb_t));
	if (cb == NULL) {
		return 1;
	}

	cb->payload = payload;
	cb->num_callbacks = num_callbacks;
	GIT2_TSRMLS_SET2(cb, TSRMLS_C);

	cb->callbacks = emalloc(sizeof(php_git2_fcall_t) * num_callbacks);
	memset(cb->callbacks, '\0', sizeof(php_git2_fcall_t) * num_callbacks);
    va_start(ap, num_args);
    for (i = 0; i < num_callbacks; i++) {
        zend_fcall_info* fci = va_arg(ap, zend_fcall_info*);
        zend_fcall_info_cache* fcc = va_arg(ap, zend_fcall_info_cache*);
        
        if (!fci) {
            memcpy(&cb->callbacks[i].fci, &empty_fcall_info, sizeof(zend_fcall_info));
        } else {
            memcpy(&cb->callbacks[i].fci, &fci, sizeof(zend_fcall_info));
        }

        if (!fcc) {
            memcpy(&cb->callbacks[i].fcc, &empty_fcall_info_cache, sizeof(zend_fcall_info_cache));
        } else {
            memcpy(&cb->callbacks[i].fcc, &fcc, sizeof(zend_fcall_info_cache));
        }
        
        memcpy(&cb->callbacks[i].fci, &fci, sizeof(zend_fcall_info));
        memcpy(&cb->callbacks[i].fcc, &fcc, sizeof(zend_fcall_info_cache));
    }
	va_end(ap);

	*out = cb;
	return 0;
}

void php_git2_multi_cb_free(php_git2_multi_cb_t *target)
{
	efree(target->callbacks);
	efree(target);
}

void php_git2_diff_line_to_array(const git_diff_line *line, zval *out TSRMLS_DC)
{
	array_init(out);
	add_assoc_stringl_ex(out, ZEND_STRL("origin"), strdup(&line->origin), 1);
	add_assoc_long_ex(out, ZEND_STRL("old_lineno"), line->old_lineno);
	add_assoc_long_ex(out, ZEND_STRL("new_lineno"), line->new_lineno);
	add_assoc_long_ex(out, ZEND_STRL("num_lines"), line->num_lines);
	add_assoc_long_ex(out, ZEND_STRL("content_len"), line->content_len);
	add_assoc_long_ex(out, ZEND_STRL("content_offset"), line->content_offset);
	add_assoc_stringl_ex(out, ZEND_STRL("content"), strdup(line->content), line->content_len);
}

void php_git2_diff_hunk_to_array(const git_diff_hunk *hunk, zval *out TSRMLS_DC)
{
	if (hunk == NULL) {
		ZVAL_NULL(out);
	} else {
		array_init(out);
		add_assoc_long_ex(out, ZEND_STRL("old_start"), hunk->old_start);
		add_assoc_long_ex(out, ZEND_STRL("old_lines"), hunk->old_lines);
		add_assoc_long_ex(out, ZEND_STRL("new_start"), hunk->new_start);
		add_assoc_long_ex(out, ZEND_STRL("new_lines"), hunk->new_lines);
		add_assoc_stringl_ex(out, ZEND_STRL("header"), strdup(hunk->header), 128);
	}
}

void php_git2_diff_file_to_array(const git_diff_file *file, zval *out TSRMLS_DC)
{
	char buf[41] = {0};

	array_init(out);
	git_oid_fmt(buf, &file->oid);

	add_assoc_string_ex(out, ZEND_STRL("oid"), buf);
	add_assoc_string_ex(out, ZEND_STRL("path"), strdup(file->path));
	add_assoc_long_ex(out, ZEND_STRL("size"), file->size);
	add_assoc_long_ex(out, ZEND_STRL("flags"), file->flags);
	add_assoc_long_ex(out, ZEND_STRL("mode"), file->mode);
}

void php_git2_git_diff_delta_to_array(const git_diff_delta *delta, zval *out TSRMLS_DC)
{
	zval *old, *new;

	array_init(out);

	add_assoc_long_ex(out, ZEND_STRL("status"), delta->status);
	add_assoc_long_ex(out, ZEND_STRL("flags"), delta->flags);
	add_assoc_long_ex(out, ZEND_STRL("similarity"), delta->similarity);
	add_assoc_long_ex(out, ZEND_STRL("nfiles"), delta->nfiles);

	php_git2_diff_file_to_array(&delta->old_file, old TSRMLS_CC);
	php_git2_diff_file_to_array(&delta->new_file, new TSRMLS_CC);

	add_assoc_zval_ex(out, ZEND_STRL("old_file"), old);
	add_assoc_zval_ex(out, ZEND_STRL("new_file"), new);
}

void php_git2_array_to_git_diff_options(git_diff_options *options, zval *array TSRMLS_DC)
{
    if (!array) {
        return;
    }
        
	git_diff_options_init(options, GIT_DIFF_OPTIONS_VERSION);

	options->version = php_git2_read_arrval_long(array, ZEND_STRL("version") TSRMLS_CC);
	options->flags = php_git2_read_arrval_long(array, ZEND_STRL("flags") TSRMLS_CC);
	options->ignore_submodules = php_git2_read_arrval_long(array, ZEND_STRL("ignore_submodules") TSRMLS_CC);

	php_git2_array_to_git_strarray(&options->pathspec, php_git2_read_arrval(array, ZEND_STRL("pathspec") TSRMLS_CC) TSRMLS_CC);
	// TODO(chobie): support notify cb

	options->context_lines = php_git2_read_arrval_long(array, ZEND_STRL("context_lines") TSRMLS_CC);
	options->interhunk_lines = php_git2_read_arrval_long(array, ZEND_STRL("interhunk_lines") TSRMLS_CC);
	options->oid_abbrev = php_git2_read_arrval_long(array, ZEND_STRL("oid_abbrev") TSRMLS_CC);
	options->max_size = php_git2_read_arrval_long(array, ZEND_STRL("max_size") TSRMLS_CC);
	options->old_prefix = php_git2_read_arrval_string(array, ZEND_STRL("old_prefix") TSRMLS_CC);
	options->new_prefix = php_git2_read_arrval_string(array, ZEND_STRL("new_prefix") TSRMLS_CC);
}

void php_git2_git_diff_options_free(git_diff_options *options)
{
    // todo rework needed

	/* if (options != NULL && options->pathspec.count > 0) {
		efree(options->pathspec.strings);
	} */
}

void php_git2_git_diff_options_to_array(const git_diff_options *options, zval *out TSRMLS_DC)
{
	zval *pathspec;


	array_init(out);
	add_assoc_long_ex(out, ZEND_STRL("version"), options->version);
	add_assoc_long_ex(out, ZEND_STRL("flags"), options->flags);
	add_assoc_long_ex(out, ZEND_STRL("ignore_submodules"), options->ignore_submodules);

	array_init(pathspec);
	if (options->pathspec.count > 0) {
	} else {
		add_assoc_zval_ex(out, ZEND_STRL("pathspec"), pathspec);
	}

	if (options->notify_cb) {
	} else {
		add_assoc_null_ex(out, ZEND_STRL("notify_cb"));
	}

	add_assoc_long_ex(out, ZEND_STRL("context_lines"), options->context_lines);
	add_assoc_long_ex(out, ZEND_STRL("interhunk_lines"), options->interhunk_lines);
	add_assoc_long_ex(out, ZEND_STRL("oid_abbrev"), options->oid_abbrev);
	add_assoc_long_ex(out, ZEND_STRL("max_size"), options->max_size);
	if (options->notify_payload) {
	} else {
		add_assoc_null_ex(out, ZEND_STRL("notify_payload"));
	}
	if (options->old_prefix) {
		add_assoc_string_ex(out, ZEND_STRL("old_prefix"), strdup(options->old_prefix));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("old_prefix"));
	}
	if (options->new_prefix) {
		add_assoc_string_ex(out, ZEND_STRL("new_prefix"), strdup(options->new_prefix));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("new_prefix"));
	}
}

int php_git2_git_diff_file_cb( const git_diff_delta *delta, float progress, void *payload)
{
	zval *param_delta = NULL, *param_progress = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = (php_git2_multi_cb_t*)payload;
	int retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	ZVAL_DOUBLE(param_progress, progress);
	php_git2_git_diff_delta_to_array(delta, param_delta TSRMLS_CC);

	if (php_git2_call_function_v(&p->callbacks[0].fci, &p->callbacks[0].fcc TSRMLS_CC, retval_ptr, 3, param_delta, param_progress, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);

	return retval;
}

int php_git2_git_diff_hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
{
	zval *param_delta = NULL, *param_hunk = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = (php_git2_multi_cb_t*)payload;
	int retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	php_git2_git_diff_delta_to_array(delta, param_delta TSRMLS_CC);
	php_git2_diff_hunk_to_array(hunk, param_hunk TSRMLS_CC);

	if (php_git2_call_function_v(&p->callbacks[1].fci, &p->callbacks[1].fcc TSRMLS_CC, retval_ptr, 3, param_delta, param_hunk, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_diff_line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
{
	zval *param_delta = NULL, *param_hunk = NULL, *param_line = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = (php_git2_multi_cb_t*)payload;
	int retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	php_git2_git_diff_delta_to_array(delta, param_delta TSRMLS_CC);
	php_git2_diff_hunk_to_array(hunk, param_hunk TSRMLS_CC);
	php_git2_diff_line_to_array(line, param_line TSRMLS_CC);

	if (php_git2_call_function_v(&p->callbacks[2].fci, &p->callbacks[2].fcc TSRMLS_CC, retval_ptr, 4, param_delta, param_hunk, param_line, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_blob_chunk_cb(char *content, size_t max_length, void *payload)
{
    // todo need to implementated

    return 0;
}

void php_git2_git_transfer_progress_to_array(const git_transfer_progress *progress, zval *out TSRMLS_DC)
{
	array_init(out);

	add_assoc_long_ex(out, ZEND_STRL("total_objects"), progress->total_objects);
	add_assoc_long_ex(out, ZEND_STRL("indexed_objects"), progress->indexed_objects);
	add_assoc_long_ex(out, ZEND_STRL("received_objects"), progress->received_objects);
	add_assoc_long_ex(out, ZEND_STRL("local_objects"), progress->local_objects);
	add_assoc_long_ex(out, ZEND_STRL("total_deltas"), progress->total_deltas);
	add_assoc_long_ex(out, ZEND_STRL("indexed_deltas"), progress->indexed_deltas);
	add_assoc_long_ex(out, ZEND_STRL("received_bytes"), progress->received_bytes);
}

void php_git2_array_to_git_blame_options(git_blame_options *options, zval *array TSRMLS_DC)
{
	zval *tmp;

	options->version = php_git2_read_arrval_long(array, ZEND_STRL("version") TSRMLS_CC);
	options->flags = php_git2_read_arrval_long(array, ZEND_STRL("flags") TSRMLS_CC);
	options->min_match_characters = php_git2_read_arrval_long(array, ZEND_STRL("min_match_characters") TSRMLS_CC);
	tmp = php_git2_read_arrval(array, ZEND_STRL("newest_commit") TSRMLS_CC);
	if (Z_ISREF_P(tmp)) {
		if (Z_TYPE_P(tmp) != IS_STRING) {
			convert_to_string(tmp);
		}
		if (git_oid_fromstrn(&options->newest_commit, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp)) != GIT_OK) {
			return;
		}
	}

	tmp = php_git2_read_arrval(array, ZEND_STRL("oldest_commit") TSRMLS_CC);
	if (Z_ISREF_P(tmp)) {
		if (Z_TYPE_P(tmp) != IS_STRING) {
			convert_to_string(tmp);
		}
		if (git_oid_fromstrn(&options->newest_commit, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp)) != GIT_OK) {
			return;
		}
	}

	options->min_line = php_git2_read_arrval_long(array, ZEND_STRL("min_line") TSRMLS_CC);
	options->max_line = php_git2_read_arrval_long(array, ZEND_STRL("max_line") TSRMLS_CC);

}

void php_git2_git_blame_options_to_array(git_blame_options *options, zval *out TSRMLS_DC)
{
	char buf[41] = {0};

	array_init(out);

	add_assoc_long_ex(out, ZEND_STRL("version"), options->version);
	add_assoc_long_ex(out, ZEND_STRL("flags"), options->flags);
	add_assoc_long_ex(out, ZEND_STRL("min_match_characters"), options->min_match_characters);

	if (git_oid_iszero(&options->newest_commit) != 1) {
		git_oid_fmt(buf, &options->newest_commit);
		add_assoc_str_ex(out, ZEND_STRL("newest_commit"), zval_get_string(out));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("newest_commit"));
	}

	if (git_oid_iszero(&options->oldest_commit) != 1) {
		git_oid_fmt(buf, &options->oldest_commit);
		add_assoc_str_ex(out, ZEND_STRL("oldest_commit"), zval_get_string(out));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("oldest_commit"));
	}

	add_assoc_long_ex(out, ZEND_STRL("min_line"), options->min_line);
	add_assoc_long_ex(out, ZEND_STRL("max_line"), options->max_line);
}

void php_git2_git_blame_hunk_to_array(const git_blame_hunk *hunk, zval *out TSRMLS_DC)
{
	zval *final = NULL, *orig = NULL;
	char buf[41] = {0};

	array_init(out);

	add_assoc_long_ex(out, ZEND_STRL("lines_in_hunk"), hunk->lines_in_hunk);

	git_oid_fmt(buf, &hunk->final_commit_id);
	add_assoc_string_ex(out, ZEND_STRL("final_commit_id"), buf);

	php_git2_git_signature_to_array(hunk->final_signature, final TSRMLS_CC);
	add_assoc_zval_ex(out, ZEND_STRL("final_signature"), final);

	add_assoc_long_ex(out, ZEND_STRL("final_start_line_number"), hunk->final_start_line_number);

	git_oid_fmt(buf, &hunk->orig_commit_id);
	add_assoc_string_ex(out, ZEND_STRL("orig_commit_id"), buf);
	add_assoc_string_ex(out, ZEND_STRL("orig_path"), strdup(hunk->orig_path));

	add_assoc_long_ex(out, ZEND_STRL("orig_start_line_number"), hunk->orig_start_line_number);
	if (hunk->orig_signature) {
		php_git2_git_signature_to_array(hunk->orig_signature, orig TSRMLS_CC);
	} else {
		ZVAL_NULL(orig);
	}
	add_assoc_zval_ex(out, ZEND_STRL("orig_signature"), orig);

	add_assoc_stringl_ex(out, ZEND_STRL("boundary"), strdup(&hunk->boundary), 1);
}

void php_git2_git_clone_options_to_array(git_clone_options *options, zval *out TSRMLS_DC)
{
    array_init(out);

    add_assoc_long_ex(out, ZEND_STRL("version"), options->version);
    add_assoc_long_ex(out, ZEND_STRL("bare"), options->bare);
    add_assoc_long_ex(out, ZEND_STRL("ignore_cert_errors"), options->ignore_cert_errors);

    /* TODO: make other options available */
}

void php_git2_array_to_git_clone_options(git_clone_options *options, zval *array TSRMLS_DC)
{
    options->version = php_git2_read_arrval_long2(array, ZEND_STRL("version"), 1 TSRMLS_CC);
    options->bare = php_git2_read_arrval_long2(array, ZEND_STRL("bare"), 0 TSRMLS_CC);
    options->ignore_cert_errors = php_git2_read_arrval_long2(array, ZEND_STRL("ignore_cert_errors"), 0 TSRMLS_CC);
}

void php_git2_git_diff_find_options_to_array(git_diff_find_options *options, zval *out TSRMLS_DC)
{
    // todo need to implementated
}

void php_git2_array_to_git_diff_find_options(git_diff_find_options *options, zval *array TSRMLS_DC)
{
    // todo need to implementated
}

void php_git2_git_diff_find_options_free(git_diff_find_options *options)
{
    // todo need to implementated
}

void php_git2_git_push_options_to_array(git_push_options *options, zval *out TSRMLS_DC)
{
    // todo need to implementated
}

void php_git2_array_to_git_push_options(git_push_options *options, zval *array TSRMLS_DC)
{
    // todo need to implementated
}

void php_git2_git_push_options_free(git_push_options *options)
{
    // todo need to implementated
}

int php_git2_git_transport_cb(git_transport **out, git_remote *owner, void *param)
{
	zval *param_owner, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)param;
	php_git2_t *_param_owner;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	if (php_git2_make_resource(&_param_owner, PHP_GIT2_TYPE_REMOTE, owner, 0 TSRMLS_CC)) {
		return 0;
	}

	ZVAL_RES(param_owner, GIT2_RVAL_P(_param_owner));

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_owner, p->payload)) {
		return GIT_EUSER;
	}

	/* TODO(chobie): implement this */

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_index_matched_path_cb(const char *path, const char *matched_pathspec, void *payload)
{
	zval *param_path, *param_matched_pathspec, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	ZVAL_STRING(param_path, path);
	ZVAL_STRING(param_matched_pathspec, matched_pathspec);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3, param_path, param_matched_pathspec, p->payload)) {
		zval_ptr_dtor(param_path);
		zval_ptr_dtor(param_matched_pathspec);
		zval_ptr_dtor(p->payload);
		return 0;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_array_to_git_index_entry(git_index_entry *entry, zval *array TSRMLS_DC)
{
	zval *ctime, *mtime, *oid;
	memset(entry, '\0', sizeof(git_index_entry));

	ctime = php_git2_read_arrval(array, ZEND_STRL("ctime") TSRMLS_CC);
	mtime = php_git2_read_arrval(array, ZEND_STRL("mtime") TSRMLS_CC);

	oid = php_git2_read_arrval(array, ZEND_STRL("oid") TSRMLS_CC);
	if (git_oid_fromstrn(&entry->oid, Z_STRVAL_P(oid), Z_STRLEN_P(oid)) != GIT_OK) {
		return 0;
	}

	entry->ctime.seconds = php_git2_read_arrval_long(ctime, ZEND_STRL("seconds") TSRMLS_CC);
	entry->ctime.nanoseconds = php_git2_read_arrval_long(ctime, ZEND_STRL("nanoseconds") TSRMLS_CC);
	entry->mtime.seconds = php_git2_read_arrval_long(mtime, ZEND_STRL("seconds") TSRMLS_CC);
	entry->mtime.nanoseconds = php_git2_read_arrval_long(mtime, ZEND_STRL("nanoseconds") TSRMLS_CC);
	entry->dev = php_git2_read_arrval_long(array, ZEND_STRL("dev") TSRMLS_CC);
	entry->ino = php_git2_read_arrval_long(array, ZEND_STRL("ino") TSRMLS_CC);
	entry->mode = php_git2_read_arrval_long(array, ZEND_STRL("mode") TSRMLS_CC);
	entry->uid = php_git2_read_arrval_long(array, ZEND_STRL("uid") TSRMLS_CC);
	entry->gid = php_git2_read_arrval_long(array, ZEND_STRL("gid") TSRMLS_CC);
	entry->file_size = php_git2_read_arrval_long(array, ZEND_STRL("file_size") TSRMLS_CC);
	entry->flags = php_git2_read_arrval_long(array, ZEND_STRL("flags") TSRMLS_CC);
	entry->flags_extended = php_git2_read_arrval_long(array, ZEND_STRL("flags_extended") TSRMLS_CC);
	entry->path = php_git2_read_arrval_string(array, ZEND_STRL("path") TSRMLS_CC);

	return 1;
}

void php_git2_git_index_entry_to_array(const git_index_entry *entry, zval *out TSRMLS_DC)
{
	zval *ctime, *mtime;
	char buf[GIT2_OID_HEXSIZE] = {0};

	array_init(out);
	array_init(ctime);
	array_init(mtime);

	git_oid_fmt(buf, &entry->oid);

	add_assoc_long_ex(ctime, ZEND_STRL("seconds"), entry->ctime.seconds);
	add_assoc_long_ex(ctime, ZEND_STRL("nanoseconds"), entry->ctime.nanoseconds);
	add_assoc_long_ex(mtime, ZEND_STRL("seconds"), entry->mtime.seconds);
	add_assoc_long_ex(mtime, ZEND_STRL("nanoseconds"), entry->mtime.nanoseconds);

	add_assoc_zval_ex(out, ZEND_STRL("ctime"), ctime);
	add_assoc_zval_ex(out, ZEND_STRL("mtime"), mtime);

	add_assoc_long_ex(out, ZEND_STRL("dev"), entry->dev);
	add_assoc_long_ex(out, ZEND_STRL("ino"), entry->ino);
	add_assoc_long_ex(out, ZEND_STRL("mode"), entry->mode);
	add_assoc_long_ex(out, ZEND_STRL("uid"), entry->uid);
	add_assoc_long_ex(out, ZEND_STRL("gid"), entry->gid);
	add_assoc_long_ex(out, ZEND_STRL("file_size"), entry->file_size);
	add_assoc_string_ex(out, ZEND_STRL("oid"), buf);
	add_assoc_long_ex(out, ZEND_STRL("flags"), entry->flags);
	add_assoc_long_ex(out, ZEND_STRL("flags_extended"), entry->flags_extended);
	add_assoc_string_ex(out, ZEND_STRL("path"), entry->path);
}

int php_git2_git_tag_foreach_cb(const char *name, git_oid *oid, void *payload)
{
	php_git2_t *result;
	zval *param_name, *param_oid, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	char buffer[GIT2_OID_HEXSIZE] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls)

	git_oid_fmt(buffer, oid);

	Z_ADDREF_P(p->payload);
	ZVAL_STRING(param_name, name);
	ZVAL_STRING(param_oid, buffer);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3, param_name, param_oid, p->payload)) {
		zval_ptr_dtor(retval_ptr);
		zend_list_delete(result->resource_id);
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	zend_list_delete(result->resource_id);

	return retval;
}

int php_git2_git_attr_foreach_cb(const char *name, const char *value, void *payload)
{
    // todo need to implementated

    return 0;
}

int php_git2_git_submodule_foreach_cb(git_submodule *sm, const char *name, void *payload)
{
	php_git2_t *submodule;
	zval *param_sm, *param_name, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	if (php_git2_make_resource(&submodule, PHP_GIT2_TYPE_SUBMODULE, sm, 0 TSRMLS_CC)) {
		return GIT_EUSER;
	}

	ZVAL_RES(param_sm, GIT2_RVAL_P(submodule));

	ZVAL_STRING(param_name, name);
	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3,
		param_sm, param_name, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

void php_git2_git_error_to_array(const git_error *error, zval *out)
{
	array_init(out);
	if (error == NULL) {
		add_next_index_string(out, "");
		add_next_index_long(out, 0);
	} else {
		add_next_index_string(out, error->message);
		add_next_index_long(out, error->klass);
	}
}

int php_git2_git_treebuilder_filter_cb(const git_tree_entry *entry, void *payload)
{
	php_git2_t *result;
	zval *param_tree_entry, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_TREE_ENTRY, entry, 0 TSRMLS_CC)) {
		return 0;
	}

	ZVAL_RES(param_tree_entry, GIT2_RVAL_P(result));
	GIT2_RVAL_P(result)->gc.refcount++;
	Z_ADDREF_P(p->payload);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_tree_entry, p->payload)) {
		zval_ptr_dtor(param_tree_entry);
		zval_ptr_dtor(p->payload);
		zend_list_delete(result->resource_id);
		retval = 0;
		return 0;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	zend_list_delete(result->resource_id);

	return retval;
}

int php_git2_git_stash_cb(size_t index, const char* message, const git_oid *stash_id, void *payload)
{
	zval *param_index, *param_message,*param_stash_id, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	char _oid[GIT2_OID_HEXSIZE] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls)

	git_oid_fmt(_oid, stash_id);

	Z_ADDREF_P(p->payload);
	ZVAL_LONG(param_index, index);
	ZVAL_STRING(param_message, message);
	ZVAL_STRING(param_stash_id, _oid);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 4, param_index, param_message, param_stash_id, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_note_foreach_cb(const git_oid *blob_id, const git_oid *annotated_object_id, void *payload)
{
    // todo need to implementated

    return 0;
}

void php_git2_array_to_git_transfer_progress(git_transfer_progress *progress, zval *array TSRMLS_DC)
{
    // todo need to implementated
}

int php_git2_git_cred_cb(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *data)
{
	php_git2_t *result;
	zval *param_url = NULL, *param_username_from_url = NULL, *param_allowed_types = NULL, *retval_ptr;
	php_git2_remote_cb_t *cb = (php_git2_remote_cb_t*)data;
	int retval = 1;
	GIT2_TSRMLS_SET(cb->tsrm_ls);

	if (cb != NULL) {
		ZVAL_NULL(param_url);
		ZVAL_NULL(param_username_from_url);

		if (url != NULL) {
			ZVAL_STRING(param_url, url);
		}
		if (username_from_url != NULL) {
			ZVAL_STRING(param_username_from_url, username_from_url);
		}
		ZVAL_LONG(param_allowed_types, allowed_types);
		Z_ADDREF_P(cb->payload);
        SEPARATE_ZVAL(cb->payload);
        ZVAL_MAKE_REF(cb->payload);

		if (php_git2_call_function_v(&cb->callbacks[0].fci, &cb->callbacks[0].fcc TSRMLS_CC, retval_ptr, 4,
			param_url, param_username_from_url, param_allowed_types, cb->payload)) {
				fprintf(stderr, "CALL FUNCTION ERROR");
		}
	}

	if (retval_ptr && Z_TYPE_P(retval_ptr) == IS_RESOURCE) {
        result = (php_git2_t *) zend_fetch_resource(Z_RES_P(retval_ptr), PHP_GIT2_RESOURCE_NAME, git2_resource_handle);
		*cred = PHP_GIT2_V(result, cred);
		zval_ptr_dtor(retval_ptr);
	}
	return retval;
}

void php_git2_git_remote_head_to_array(const git_remote_head *head, zval *out TSRMLS_DC)
{
	char oid[41] = {0}, loid[41] = {0};


	git_oid_fmt(oid, &head->oid);
	git_oid_fmt(loid, &head->loid);

	array_init(out);
	add_assoc_long_ex(out, ZEND_STRL("local"), head->local);
	add_assoc_string_ex(out, ZEND_STRL("oid"), oid);
	add_assoc_string_ex(out, ZEND_STRL("loid"), loid);
	add_assoc_string_ex(out, ZEND_STRL("name"), head->name);
}

int php_git2_git_reference_foreach_cb(git_reference *reference, void *payload)
{
	php_git2_t *result;
	zval *param_reference, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	php_git2_make_resource(&result, PHP_GIT2_TYPE_REFERENCE, reference, 0 TSRMLS_CC);
	GIT2_RVAL_P(result)->gc.refcount++;
	ZVAL_RES(param_reference, GIT2_RVAL_P(result));

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_reference, p->payload)) {
		zend_list_delete(result->resource_id);
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	zend_list_delete(result->resource_id);
	return retval;
}

int php_git2_git_reference_foreach_name_cb(const char *name, void *payload)
{
	php_git2_t *result;
	zval *param_name, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	ZVAL_STRING(param_name, name);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_name, p->payload)) {
		zval_ptr_dtor(retval_ptr);
		zend_list_delete(result->resource_id);
		return GIT_EUSER;
	}
	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	zend_list_delete(result->resource_id);

	return retval;
}

int php_git2_git_repository_fetchhead_foreach_cb(const char *ref_name, const char *remote_url, const git_oid *oid,unsigned int is_merge, void *payload)
{
	zval *param_ref_name, *param_remote_url, *param_oid, *param_is_merge, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	char _oid[GIT2_OID_HEXSIZE] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls)

	git_oid_fmt(_oid, oid);

	Z_ADDREF_P(p->payload);
	ZVAL_STRING(param_ref_name, ref_name);
	ZVAL_STRING(param_remote_url, remote_url);
	ZVAL_STRING(param_oid, _oid);
	ZVAL_BOOL(param_is_merge, is_merge);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 5,
	 	param_ref_name,
	 	param_remote_url,
	 	param_oid,
	 	param_is_merge,
	 	p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_repository_mergehead_foreach_cb(const git_oid *oid, void *payload)
{
	zval *param_oid, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	char _oid[GIT2_OID_HEXSIZE] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls)

	git_oid_fmt(_oid, oid);

	Z_ADDREF_P(p->payload);
	ZVAL_STRING(param_oid, _oid);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_oid, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_remote_rename_problem_cb(const char *problematic_refspec, void *payload)
{
    // todo need to implementated

	return 0;
}

void php_git2_array_to_git_remote_callbacks(git_remote_callbacks *cb, zval *callbacks TSRMLS_DC)
{
    zval *credentials_cb = NULL;
    struct git_remote_callbacks _cb = GIT_REMOTE_CALLBACKS_INIT;
    php_git2_remote_cb_t *_payload = NULL;

    cb = &_cb;

	/* TODO(chobie): support other callbacks */
    cb->credentials = php_git2_git_cred_cb;
    credentials_cb = php_git2_read_arrval(callbacks, ZEND_STRL("credentials") TSRMLS_CC);

    /* TODO(chobie): can we free payload? */
    _payload = ecalloc(1, sizeof(php_git2_remote_cb_t));
    GIT2_TSRMLS_SET2(_payload, TSRMLS_C);

    if (credentials_cb != NULL) {
    	char *is_callable_error;

    	if(zend_fcall_info_init(credentials_cb, 0, &(_payload->callbacks[0].fci), &(_payload->callbacks[0].fcc), NULL, &is_callable_error TSRMLS_CC) == SUCCESS) {
    		if (is_callable_error) {
    			efree(is_callable_error);
   			}
   		}

    	Z_ADDREF_P(credentials_cb);
    }

    cb->payload = _payload;
}

int php_git2_git_treewalk_cb(const char *root, const git_tree_entry *entry, void *payload)
{
	php_git2_t *result;
	zval *param_root, *param_rsrc, *retval_ptr = NULL;
	struct tree_walk_cb_t *p = (struct tree_walk_cb_t*)payload;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	ZVAL_STRING(param_root, root);
	php_git2_make_resource(&result, PHP_GIT2_TYPE_TREE_ENTRY, entry, 0 TSRMLS_CC);
	GIT2_RVAL_P(result)->gc.refcount++;
	ZVAL_RES(param_rsrc, GIT2_RVAL_P(result));

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3, param_root, param_rsrc, p->payload)) {
		zend_list_delete(result->resource_id);
		return 0;
	}
	zval_ptr_dtor(retval_ptr);
	zend_list_delete(result->resource_id);

	return 1;
}

void php_git2_git_status_options_to_array(git_status_options *options, zval *out TSRMLS_DC)
{
	zval *pathspec;

	array_init(out);

	add_assoc_long_ex(out, ZEND_STRL("version"), options->version);
	add_assoc_long_ex(out, ZEND_STRL("show"), options->show);
	add_assoc_long_ex(out, ZEND_STRL("flags"), options->flags);
	php_git2_git_strarray_to_array(&options->pathspec, pathspec TSRMLS_CC);
	add_assoc_zval_ex(out, ZEND_STRL("pathspec"), pathspec);
}

void php_git2_array_to_git_status_options(git_status_options *options, zval *array TSRMLS_DC)
{
	options->version = php_git2_read_arrval_long2(array, ZEND_STRL("version"), 1 TSRMLS_CC);
	options->show = php_git2_read_arrval_long2(array, ZEND_STRL("version"), 0 TSRMLS_CC);
	options->flags = php_git2_read_arrval_long2(array, ZEND_STRL("version"), 0 TSRMLS_CC);

	php_git2_array_to_git_strarray(&options->pathspec, php_git2_read_arrval(array, ZEND_STRL("pathspec") TSRMLS_CC) TSRMLS_CC);
}

void php_git2_git_status_entry_to_array(const git_status_entry *entry, zval *out TSRMLS_DC)
{
	zval *head_to_index, *index_to_workdir;

	array_init(out);

	if (entry->head_to_index) {
		php_git2_git_diff_delta_to_array(entry->head_to_index, head_to_index TSRMLS_CC);
	} else {
		ZVAL_NULL(head_to_index);
	}

	if (entry->index_to_workdir) {
		php_git2_git_diff_delta_to_array(entry->index_to_workdir, index_to_workdir TSRMLS_CC);
	} else {
		ZVAL_NULL(index_to_workdir);
	}

	add_assoc_long_ex(out, ZEND_STRL("status"), entry->status);
	add_assoc_zval_ex(out, ZEND_STRL("head_to_index"), head_to_index);
	add_assoc_zval_ex(out, ZEND_STRL("index_to_workdir"), index_to_workdir);
}

int php_git2_git_status_cb(const char *path, unsigned int status_flags, void *payload)
{
	zval *param_path, *param_status_flags, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	ZVAL_STRING(param_path, path);
	ZVAL_LONG(param_status_flags, status_flags);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3,
		param_path, param_status_flags, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_cred_sign_callback(void *payload, ...)
{
    // todo need to implementated

	return 0;
}

void php_git2_array_to_git_repository_init_options(git_repository_init_options *opts, zval *array TSRMLS_DC)
{
	long lval;

	lval = php_git2_read_arrval_long(array, ZEND_STRL("version") TSRMLS_CC);
	if (lval > 0) {
		opts->version = lval;
	}
	opts->flags = php_git2_read_arrval_long(array, ZEND_STRL("flags") TSRMLS_CC);
	opts->mode = php_git2_read_arrval_long(array, ZEND_STRL("mode") TSRMLS_CC);

	opts->workdir_path = php_git2_read_arrval_string(array, ZEND_STRL("workdir_path") TSRMLS_CC);
	opts->description = php_git2_read_arrval_string(array, ZEND_STRL("description") TSRMLS_CC);
	opts->template_path = php_git2_read_arrval_string(array, ZEND_STRL("template_path") TSRMLS_CC);
	opts->initial_head = php_git2_read_arrval_string(array, ZEND_STRL("initial_head") TSRMLS_CC);
	opts->origin_url = php_git2_read_arrval_string(array, ZEND_STRL("origin_url") TSRMLS_CC);
}

void php_git2_git_repository_init_options_to_array(git_repository_init_options *opts, zval *out TSRMLS_DC)
{
	array_init(out);

	add_assoc_long_ex(out, ZEND_STRL("version"), opts->version);
	add_assoc_long_ex(out, ZEND_STRL("flags"), opts->flags);
	add_assoc_long_ex(out, ZEND_STRL("mode"), opts->mode);

	if (opts->workdir_path != NULL) {
		add_assoc_string_ex(out, ZEND_STRL("workdir_path"), strdup(opts->workdir_path));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("workdir_path"));
	}

	if (opts->workdir_path != NULL) {
		add_assoc_string_ex(out, ZEND_STRL("description"), strdup(opts->description));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("description"));
	}

	if (opts->workdir_path != NULL) {
		add_assoc_string_ex(out, ZEND_STRL("template_path"), strdup(opts->template_path));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("template_path"));
	}

	if (opts->workdir_path != NULL) {
		add_assoc_string_ex(out, ZEND_STRL("initial_head"), strdup(opts->initial_head));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("initial_head"));
	}

	if (opts->workdir_path != NULL) {
		add_assoc_string_ex(out, ZEND_STRL("origin_url"), strdup(opts->origin_url));
	} else {
		add_assoc_null_ex(out, ZEND_STRL("origin_url"));
	}
}

int php_git2_git_odb_foreach_cb(const git_oid *id, void *payload)
{
	zval *param_oid, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	char buf[41] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	git_oid_fmt(buf, id);
	ZVAL_STRING(param_oid, buf);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_oid, p->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_filter_check_fn(git_filter *self, void **payload, const git_filter_source *src, const char **attr_values)
{
	php_git2_filter *filter = (php_git2_filter*)self;
	php_git2_t *filter_source;
	zval *param_payload = NULL, *param_source = NULL, *param_attr = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = filter->multi;
	const char *ptr = self->attributes;
	int result, last_is_space = 0;
	int attribute_count = 0, len = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	if (self->attributes != NULL) {
		while (*ptr != '\0') {
			if (*ptr == 0x09 || *ptr == 0x20 && last_is_space == 0) {
				attribute_count++;
			}

			if (*ptr == 0x09 || *ptr == 0x20) {
				last_is_space = 1;
			} else {
				last_is_space = 0;
			}
			ptr++;
			len++;
		}
		if (len > 0) {
			attribute_count++;
		}
	}

	ZVAL_NULL(param_payload);
	if (php_git2_make_resource(&filter_source, PHP_GIT2_TYPE_FILTER_SOURCE, src, 0 TSRMLS_CC)) {
		return GIT_EUSER;
	}

	ZVAL_RES(param_source, GIT2_RVAL_P(filter_source));

	array_init(param_attr);

	if (attribute_count > 0) {
		int y = 0;
		for (y = 0; y < attribute_count; y++) {
			if (GIT_ATTR_TRUE(attr_values[y])) {
				add_next_index_bool(param_attr, 1);
			} else if (GIT_ATTR_FALSE(attr_values[y])) {
				add_next_index_bool(param_attr, 0);
			} else if (GIT_ATTR_UNSPECIFIED(attr_values[y])) {
				add_next_index_null(param_attr);
			} else {
				add_next_index_string(param_attr, attr_values[y]);
			}
		}
	} else {
	}

	if (php_git2_call_function_v(&p->callbacks[2].fci, &p->callbacks[2].fcc TSRMLS_CC, retval_ptr, 3,
		param_payload, param_source, param_attr)) {
		return GIT_EUSER;
	}

	if (retval_ptr) {
		result = Z_LVAL_P(retval_ptr);

		/* NOTE(chobie): adjust return value */
		if (result == 0) {
			result = GIT_PASSTHROUGH;
		} else if (result == 1) {
			result = 0;
		}
	}
	zval_ptr_dtor(retval_ptr);

	return result;
}

void php_git2_git_filter_shutdown_fn(git_filter *self)
{
	zval *param_self = NULL, *param_payload = NULL, *retval_ptr = NULL;
	php_git2_filter *filter = (php_git2_filter*)self;
	php_git2_multi_cb_t *p = filter->multi;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	ZVAL_NULL(param_self);
	ZVAL_NULL(param_payload);

	if (php_git2_call_function_v(&p->callbacks[1].fci, &p->callbacks[1].fcc TSRMLS_CC, retval_ptr, 2, param_self, param_payload)) {
	}
	zval_ptr_dtor(retval_ptr);
}

int php_git2_git_filter_init_fn(git_filter *self)
{
	zval *param_self = NULL, *param_payload = NULL, *retval_ptr = NULL;
	php_git2_filter *filter = (php_git2_filter*)self;
	php_git2_multi_cb_t *p = filter->multi;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	ZVAL_NULL(param_self);
	ZVAL_NULL(param_payload);

	if (php_git2_call_function_v(&p->callbacks[0].fci, &p->callbacks[0].fcc TSRMLS_CC, retval_ptr, 2, param_self, param_payload)) {
	}
	zval_ptr_dtor(retval_ptr);
}

int php_git2_git_filter_apply_fn(git_filter *self, void **payload, git_buf *to, const git_buf *from, const git_filter_source *src)
{
	php_git2_filter *filter = (php_git2_filter*)self;
	php_git2_t *filter_source;
	zval *param_payload = NULL, *param_from = NULL, *param_src = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = filter->multi;
	int retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	ZVAL_NULL(param_payload);
	ZVAL_STRINGL(param_from, from->ptr, from->size);

	if (php_git2_make_resource(&filter_source, PHP_GIT2_TYPE_FILTER_SOURCE, src, 0 TSRMLS_CC)) {
		return GIT_EUSER;
	}

	ZVAL_RES(param_src, GIT2_RVAL_P(filter_source));

	if (php_git2_call_function_v(&p->callbacks[3].fci, &p->callbacks[3].fcc TSRMLS_CC, retval_ptr, 3,
		param_payload, param_from, param_src)) {
		return GIT_EUSER;
	}

	if (retval_ptr) {
		if (Z_TYPE_P(retval_ptr) == IS_LONG) {
			retval = Z_LVAL_P(retval_ptr);
		} else {
			if (Z_TYPE_P(retval_ptr) != IS_STRING) {
				convert_to_string(retval_ptr);
			}

			git_buf_set(to, Z_STRVAL_P(retval_ptr), Z_STRLEN_P(retval_ptr));
		}
	}

	zval_ptr_dtor(retval_ptr);
	return retval;
}

void php_git2_git_filter_cleanup_fn(git_filter *self, void *payload)
{
	zval *param_self = NULL, *param_payload = NULL, *retval_ptr = NULL;
	php_git2_filter *filter = (php_git2_filter*)self;
	php_git2_multi_cb_t *p = filter->multi;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	ZVAL_NULL(param_self);
	ZVAL_NULL(param_payload);

	if (php_git2_call_function_v(&p->callbacks[4].fci, &p->callbacks[4].fcc TSRMLS_CC, retval_ptr, 2, param_self, param_payload)) {
	}
}

int php_git2_git_packbuilder_progress(int stage, unsigned int current, unsigned int total, void *payload)
{
	zval *param_stage = NULL, *param_current = NULL, *param_total = NULL, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	ZVAL_LONG(param_stage, stage);
	ZVAL_LONG(param_current, current);
	ZVAL_LONG(param_total, total);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 4,
		param_stage, param_current, param_total, p->payload)) {
		return GIT_EUSER;
	}

	if (retval_ptr) {
		retval = Z_LVAL_P(retval_ptr);
		zval_ptr_dtor(retval_ptr);
	}
	return retval;
}

int php_git2_git_packbuilder_foreach_cb(void *buf, size_t size, void *payload)
{
	zval *param_buf= NULL, *param_size = NULL, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	ZVAL_STRINGL(param_buf, buf, size);
	ZVAL_LONG(param_size, size);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3,
		param_buf, param_size, p->payload)) {
		return GIT_EUSER;
	}

	if (retval_ptr) {
		retval = Z_LVAL_P(retval_ptr);
		zval_ptr_dtor(retval_ptr);
	}
	return retval;
}

int php_git2_git_transfer_progress_callback(const git_transfer_progress *stats, void *payload)
{
	zval *param_stats = NULL, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);
	php_git2_git_transfer_progress_to_array(stats, param_stats TSRMLS_CC);


	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2,
		param_stats, p->payload)) {
		return GIT_EUSER;
	}

	if (retval_ptr) {
		retval = Z_LVAL_P(retval_ptr);
		zval_ptr_dtor(retval_ptr);
	}
	return retval;
}

int php_git2_git_push_status_foreach_cb(const char *ref, const char *msg, void *data)
{
	php_git2_t *result;
	zval *param_ref, *param_msg, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)data;
	int retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	Z_ADDREF_P(p->payload);

	ZVAL_NULL(param_ref);
	ZVAL_NULL(param_msg);

	if (ref != NULL) {
		ZVAL_STRING(param_ref, ref);
	}
	if (msg != NULL) {
		ZVAL_STRING(param_msg, msg);
	}

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 3, param_ref, param_msg, p->payload)) {
		zend_list_delete(result->resource_id);
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

void php_git2_git_revspec_to_array(const git_revspec *revspec, zval *out TSRMLS_DC)
{
	php_git2_t *to = NULL, *from = NULL;

	array_init(out);

	if (php_git2_make_resource(&from, PHP_GIT2_TYPE_OBJECT, revspec->from, 1 TSRMLS_CC)) {
		return;
	}

    if (php_git2_make_resource(&to, PHP_GIT2_TYPE_OBJECT, revspec->to, 1 TSRMLS_CC)) {
		return;
	}

	add_assoc_resource_ex(out, ZEND_STRL("from"), GIT2_RVAL_P(from));
	add_assoc_resource_ex(out, ZEND_STRL("to"), GIT2_RVAL_P(to));
	add_assoc_long_ex(out, ZEND_STRL("flags"), revspec->flags);
}

int php_git2_odb_backend_foreach(git_odb_backend *backend, git_odb_foreach_cb cb, void *data)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)backend;
	zval *param_callback = NULL, *callback = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	zend_function function = {0};
	php_git2_odb_backend_foreach_callback *_callback;
	php_git2_cb_t *__cb = (php_git2_cb_t*)data;
	int retval = 0;
	zend_arg_info common_arg_info[] = {
	    {zend_string_init("oid", sizeof("oid") - 1, 0), NULL, 0, 0, 0, 0 },
	    {zend_string_init("payload", sizeof("payload") - 1, 0), NULL, 0, 1, 0, 0 },
	};

	GIT2_TSRMLS_SET(p->tsrm_ls);

	object_init_ex(callback, php_git2_odb_backend_foreach_callback_class_entry);
	_callback = (php_git2_odb_backend_foreach_callback*) Z_OBJ_P(callback TSRMLS_CC);
	_callback->callback = cb;
	_callback->payload = __cb;
	Z_ADDREF_P(callback);

    // todo
	function.type = ZEND_INTERNAL_FUNCTION;
	function.common.function_name = zend_string_init("callback", sizeof("callback") - 1, 0);
	function.common.fn_flags = ZEND_ACC_CLOSURE;
	function.common.num_args = 2;
	function.common.required_num_args = 2;
	function.common.arg_info = common_arg_info;
	function.common.scope = php_git2_odb_backend_foreach_callback_class_entry;
	function.internal_function.type = ZEND_INTERNAL_FUNCTION;
	function.internal_function.scope = php_git2_odb_backend_foreach_callback_class_entry;
	function.internal_function.fn_flags = ZEND_ACC_CLOSURE;
	function.internal_function.handler = git_ex_cb;
	function.internal_function.module = &git2_module_entry;
	function.internal_function.num_args = 2;
	function.internal_function.required_num_args = 2;
	function.internal_function.arg_info = NULL;


    zend_create_closure(param_callback, &function, function.common.scope, function.common.scope, callback TSRMLS_CC);
    Z_ADDREF_P(__cb->payload);

	if (php_git2_call_function_v(&p->callbacks[6].fci, &p->callbacks[6].fcc TSRMLS_CC, retval_ptr, 2,
		param_callback, __cb->payload)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_odb_backend_read(void **buffer, size_t *size, git_otype *type, git_odb_backend *backend, const git_oid *oid)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)backend;
	zval *param_oid = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	int retval = 0;
	char buf[41] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls);

	git_oid_fmt(buf, oid);

	ZVAL_STRING(param_oid, buf);

	if (php_git2_call_function_v(&p->callbacks[0].fci, &p->callbacks[0].fcc TSRMLS_CC, retval_ptr, 1, param_oid)) {
		return GIT_EUSER;
	}
	if (Z_TYPE_P(retval_ptr) == IS_ARRAY) {
		zval *value, *otype;
		char *pp;

		if (zend_hash_num_elements(Z_ARRVAL_P(retval_ptr)) != 2) {
			return GIT_ENOTFOUND;
		}

		value = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		pp = git_odb_backend_malloc(backend, Z_STRLEN_P(value));
		memset(pp, '\0', Z_STRLEN_P(value));
		memcpy(pp, Z_STRVAL_P(value), Z_STRLEN_P(value));

		*buffer = pp;
		size = &Z_STRLEN_P(value);

		zend_hash_move_forward(Z_ARRVAL_P(retval_ptr));
		otype = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		*type = Z_LVAL_P(otype);
	} else {
		retval = GIT_ENOTFOUND;
	}

	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_odb_backend_write(git_odb_backend *backend, const git_oid *oid, const void *buffer, size_t size, git_otype type)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)backend;
	zval *param_oid = NULL, *param_buffer = NULL, *param_otype = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	int retval = 0;
	char buf[41] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls);

	git_oid_fmt(buf, oid);

	ZVAL_STRING(param_oid, buf);
	ZVAL_STRINGL(param_buffer, buffer, size);
	ZVAL_LONG(param_otype, type);

	if (php_git2_call_function_v(&p->callbacks[1].fci, &p->callbacks[1].fcc TSRMLS_CC, retval_ptr, 3,
		param_oid, param_buffer, param_otype)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_odb_backend_read_prefix(git_oid *out_oid, void **buffer_p, size_t *len_p, git_otype *type_p, git_odb_backend *backend, const git_oid *short_oid, size_t len)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)backend;
	zval *param_short_oid = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	int retval = 0;
	char buf[41] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls);

	git_oid_nfmt(buf, len, short_oid);

	ZVAL_STRING(param_short_oid, buf);

	if (php_git2_call_function_v(&p->callbacks[2].fci, &p->callbacks[2].fcc TSRMLS_CC, retval_ptr, 1, param_short_oid)) {
		return GIT_EUSER;
	}

	if (Z_TYPE_P(retval_ptr) == IS_ARRAY) {
		zval *value, *otype, *_oid;
		char *pp;

		if (zend_hash_num_elements(Z_ARRVAL_P(retval_ptr)) != 3) {
			return GIT_ENOTFOUND;
		}

		value = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		pp = git_odb_backend_malloc(backend, Z_STRLEN_P(value));
		memset(pp, '\0', Z_STRLEN_P(value));
		memcpy(pp, Z_STRVAL_P(value), Z_STRLEN_P(value));

		*buffer_p = pp;
		len_p = &Z_STRLEN_P(value);

		zend_hash_move_forward(Z_ARRVAL_P(retval_ptr));
		otype = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		*type_p = Z_LVAL_P(otype);

		zend_hash_move_forward(Z_ARRVAL_P(retval_ptr));
		_oid = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		if (git_oid_fromstrn(out_oid, Z_STRVAL_P(_oid), Z_STRLEN_P(_oid)) != GIT_OK) {
			return GIT_EUSER;
		}
	} else {
		retval = GIT_ENOTFOUND;
	}

	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_odb_backend_read_header(size_t *len_p, git_otype *type_p, git_odb_backend *backend, const git_oid *oid)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)backend;
	zval *param_oid = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	int retval = 0;
	char buf[41] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls);

	git_oid_fmt(buf, oid);

	ZVAL_STRING(param_oid, buf);

	if (php_git2_call_function_v(&p->callbacks[3].fci, &p->callbacks[3].fcc TSRMLS_CC, retval_ptr, 1, param_oid)) {
		return GIT_EUSER;
	}
	if (Z_TYPE_P(retval_ptr) == IS_ARRAY) {
		zval *value, *otype;

		if (zend_hash_num_elements(Z_ARRVAL_P(retval_ptr)) != 2) {
			return GIT_ENOTFOUND;
		}

		value = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		*len_p = Z_LVAL_P(value);
		zend_hash_move_forward(Z_ARRVAL_P(retval_ptr));
		otype = zend_hash_get_current_data(Z_ARRVAL_P(retval_ptr));
		*type_p = Z_LVAL_P(otype);
	} else {
		retval = GIT_ENOTFOUND;
	}

	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_odb_backend_writestream(git_odb_stream **stream_out, git_odb_backend *_backend, size_t length, git_otype type)
{
	fprintf(stderr, "WRITES");
}

int php_git2_odb_backend_exists(git_odb_backend *backend, const git_oid *oid)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)backend;
	zval *param_oid = NULL, *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	int retval = 0;
	char buf[41] = {0};
	GIT2_TSRMLS_SET(p->tsrm_ls);

	git_oid_fmt(buf, oid);

	ZVAL_STRING(param_oid, buf);

	if (php_git2_call_function_v(&p->callbacks[5].fci, &p->callbacks[5].fcc TSRMLS_CC, retval_ptr, 1,
		param_oid)) {
		return GIT_EUSER;
	}

	retval = Z_LVAL_P(retval_ptr);
	if (retval == 1) {
		retval = 0;
	} else {
		retval = GIT_ENOTFOUND;
	}
	zval_ptr_dtor(retval_ptr);
	return !retval;
}

void git_ex_cb(INTERNAL_FUNCTION_PARAMETERS)
{
	zval *payload;
	php_git2_odb_backend_foreach_callback *_callback;
	char *oid;
	int oid_len, retval = 0;
	git_oid _oid;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sz", &oid, &oid_len, &payload) == FAILURE) {
		return;
	}

	if (git_oid_fromstrn(&_oid, oid, oid_len) != GIT_OK) {
		return;
	}

	_callback = (php_git2_odb_backend_foreach_callback *) Z_OBJ_P(getThis());
	_callback->payload->payload = payload;
	retval = _callback->callback(&_oid, _callback->payload);
	RETURN_LONG(retval);
}

void php_git2_odb_backend_free(git_odb_backend *_backend)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)_backend;
	zval *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	GIT2_TSRMLS_SET(p->tsrm_ls);

	if (php_git2_call_function_v(&p->callbacks[7].fci, &p->callbacks[7].fcc TSRMLS_CC, retval_ptr, 0)) {
		return;
	}

	zval_ptr_dtor(retval_ptr);
	return;
}

int php_git2_odb_refresh(git_odb_backend *_backend)
{
	php_git2_odb_backend *php_backend = (php_git2_odb_backend*)_backend;
	zval *retval_ptr = NULL;
	php_git2_multi_cb_t *p = php_backend->multi;
	GIT2_TSRMLS_SET(p->tsrm_ls);

	if (php_git2_call_function_v(&p->callbacks[8].fci, &p->callbacks[8].fcc TSRMLS_CC, retval_ptr, 0)) {
		return;
	}

	zval_ptr_dtor(retval_ptr);
	return;
}

int php_git2_git_push_transfer_progress(unsigned int current, unsigned int total, size_t bytes, void* payload)
{
    // todo need to implementated

	return 0;
}

void php_git2_git_config_entry_to_array(const git_config_entry *entry, zval *result)
{
	array_init(result);

	add_assoc_string_ex(result, ZEND_STRL("name"), strdup(entry->name));
	add_assoc_string_ex(result, ZEND_STRL("value"), strdup(entry->value));
	add_assoc_long_ex(result, ZEND_STRL("level"), entry->level);
}

int php_git2_git_config_foreach_cb(const git_config_entry *entry, void *payload)
{
	php_git2_t *result;
	zval *param_config_entry, *retval_ptr = NULL;
	php_git2_cb_t *p = (php_git2_cb_t*)payload;
	long retval = 0;
	GIT2_TSRMLS_SET(p->tsrm_ls)

	php_git2_git_config_entry_to_array(entry, param_config_entry);
	Z_ADDREF_P(p->payload);

	if (php_git2_call_function_v(p->fci, p->fcc TSRMLS_CC, retval_ptr, 2, param_config_entry, p->payload)) {
		zval_ptr_dtor(param_config_entry);
		zval_ptr_dtor(p->payload);
		zend_list_delete(result->resource_id);
		retval = 0;
		return 0;
	}

	retval = Z_LVAL_P(retval_ptr);
	zval_ptr_dtor(retval_ptr);
	return retval;
}

int php_git2_git_config_backend_foreach_match_cb(const git_config_entry *entry, void *payload)
{
    // todo need to implementated

    return 0;
}

void php_git2_array_to_git_cvar_map(git_cvar_map *map, zval *out TSRMLS_DC)
{
    // todo need to implementated
}

void php_git2_git_repository_init_options_free(git_repository_init_options *options)
{
    // todo need to implementated
}

void php_git2_git_clone_options_free(git_clone_options *options)
{
    // todo need to implementated
}

void php_git2_git_status_options_free(git_status_options *options)
{
    // todo need to implementated
}

void php_git2_array_to_git_merge_tree_opts(git_merge_tree_opts *options, zval *array TSRMLS_DC)
{
    // todo need to implementated
}

void php_git2_git_merge_tree_opts_free(git_merge_tree_opts *options)
{
    // todo need to implementated
}

void php_git2_git_blame_options_free(git_blame_options *options)
{
    // todo need to implementated
}
