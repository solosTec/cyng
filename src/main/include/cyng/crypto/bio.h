/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_BIO_H
#define CYNG_CRYPTO_BIO_H

#include <cyng/crypto.h>

namespace cyng
{
	namespace crypto
	{
		/**
		 * Generates a BIO object
		 * Example:
		 @ccode
		 auto bp = create_bio("public.pem", "w+");
		 @endcode
		 */
		BIO_ptr create_bio_file(const char *filename, const char *mode);

		/**
		 *  Flags can be: BIO_CLOSE, BIO_NOCLOSE (the close flag) BIO_FP_TEXT
		 */
		BIO_ptr create_bio_fp(FILE* stream, int flags);

		/**
		 * Doesn't support BIO_gets() or BIO_puts().
		 * The flag BIO_FLAGS_BASE64_NO_NL can be set with BIO_set_flags() 
		 * to encode the data all on one line or expect the data to be all 
		 * on one line.
		 */
		BIO_ptr_all create_bio_base64();

		/**
		 * Be carefull when using a smart pointer with BIO_push().
		 * After BIO_push() the memory is managed by the BIO (free_all) so you have to release
		 * the pointer first:
		 *
		 * example:
		 @code
			auto b64 = create_bio_base64();
			auto bmem = create_bio_s_mem();
			bmem = ::BIO_push(b64.get(), bmem.release());
		 @endcode
		 */
		BIO_ptr create_bio_s_mem();

		/**
		 * example:
		 * @code
		 auto biop = create_bio_socket(stdout, BIO_NOCLOSE);
		 * @endcode
		 */
		BIO_ptr create_bio_socket(int sock, int close_flag);

		BIO_METHOD_ptr create_method_mem();
		BIO_METHOD_ptr create_method_secmem();
		BIO_METHOD_ptr create_method_socket();
		BIO_METHOD_ptr create_method_connect();
		BIO_METHOD_ptr create_method_accept();
		BIO_METHOD_ptr create_method_fd();
		BIO_METHOD_ptr create_method_bio();
		BIO_METHOD_ptr create_method_null();

		/**
		 * Joins two BIO chains.
		 * @return pointer of p
		 */
		BIO* push(BIO_ptr p, BIO_ptr append);
	}
}

#endif	

