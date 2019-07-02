/* blurhash extension for PHP */

#ifndef PHP_BLURHASH_H
# define PHP_BLURHASH_H

extern zend_module_entry blurhash_module_entry;
# define phpext_blurhash_ptr &blurhash_module_entry

# define PHP_BLURHASH_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_BLURHASH)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_BLURHASH_H */

