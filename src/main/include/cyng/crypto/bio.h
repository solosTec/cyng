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
		 */
		BIO_ptr create_bio(const char *filename, const char *mode);
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

		BIO_METHOD_ptr create_method_mem();
		BIO_METHOD_ptr create_method_secmem();
		BIO_METHOD_ptr create_method_socket();
		BIO_METHOD_ptr create_method_connect();
		BIO_METHOD_ptr create_method_accept();
		BIO_METHOD_ptr create_method_fd();
		BIO_METHOD_ptr create_method_bio();
		BIO_METHOD_ptr create_method_null();

	}
}

#endif	

