/* blurhash extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_blurhash.h"
#include "encode.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void blurhash_encode(int $width, int $height, array $rgb, [ int $xComponents = 4, [ int $yComponents = 3 ] ])
 */
PHP_FUNCTION(blurhash_encode)
{
	zend_long xComponents = 4, yComponents = 3, width, height;
	zval *rgb;

	ZEND_PARSE_PARAMETERS_START(3, 5)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_ARRAY(rgb)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(xComponents)
		Z_PARAM_LONG(yComponents)
	ZEND_PARSE_PARAMETERS_END();

	// ピクセル情報のblurhash関数への引渡し用
	uint8_t *nativePixels = ecalloc(width * height * 3, sizeof(uint8_t));
	zend_long index = 0;

	zend_long rgbKey;
	zval *rgbVal;
	ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(rgb), rgbKey, rgbVal) {
		if (Z_TYPE_P(rgbVal) != IS_LONG) {
			zend_throw_exception_ex(NULL, 0, "$rgb[%d] is not valid value.", rgbKey);
			return;
		}

		// zend_long -> uint8_t 変換
		zend_long lval = Z_LVAL_P(rgbVal);
		nativePixels[index + 0] = (lval >> 16) & 0xFF;
		nativePixels[index + 1] = (lval >> 8) & 0xFF;
		nativePixels[index + 2] = (lval) & 0xFF;

		index += 3;
	} ZEND_HASH_FOREACH_END();

	char *hash = blurHashForPixels(xComponents, yComponents, width, height, nativePixels, width * 3);
	efree(nativePixels);

	if (hash == NULL) {
		RETURN_NULL();
	}

	zend_string *result = strpprintf(0, "%s", hash);

	RETURN_STR(result);
}
/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(blurhash)
{
#if defined(ZTS) && defined(COMPILE_DL_BLURHASH)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(blurhash)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "blurhash support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_blurhash_encode, 0, 0, 3)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
	ZEND_ARG_INFO(0, rgb)
	ZEND_ARG_INFO(0, xComponents)
	ZEND_ARG_INFO(0, yComponents)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ blurhash_functions[]
 */
static const zend_function_entry blurhash_functions[] = {
	PHP_FE(blurhash_encode,     arginfo_blurhash_encode)
	PHP_FE_END
};
/* }}} */

/* {{{ blurhash_module_entry
 */
zend_module_entry blurhash_module_entry = {
	STANDARD_MODULE_HEADER,
	"blurhash",					/* Extension name */
	blurhash_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(blurhash),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(blurhash),			/* PHP_MINFO - Module info */
	PHP_BLURHASH_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BLURHASH
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(blurhash)
#endif
