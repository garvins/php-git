#include "php_git2.h"
#include "php_git2_priv.h"
#include "message.h"

/* {{{ proto string git_message_prettify(string $message, long $strip_comments)
 */
PHP_FUNCTION(git_message_prettify)
{
	char out[GIT2_BUFFER_SIZE] = {0}, *message = NULL;
	size_t out_size = GIT2_BUFFER_SIZE, message_len;
	zend_long strip_comments;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &message, &message_len, &strip_comments) == FAILURE) {
		return;
	}

	error = git_message_prettify(out, out_size, message, strip_comments);

	if (php_git2_check_error(error, "git_message_prettify" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRING(out);
}
/* }}} */

