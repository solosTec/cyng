/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include <cyng/crypto/bio.h>
#include <openssl/ossl_typ.h>
#include <openssl/bio.h>


namespace cyng 
{
	namespace crypto
	{
		BIO_ptr create_bio_file(const char *filename, const char *mode)
		{
			//
			//	Since we provide a deleter it's impossible to use of std::make_unique<>()
			//
			return BIO_ptr(BIO_new_file(filename, mode), BIO_free);
		}

		BIO_ptr create_bio_fp(FILE* stream, int flags)
		{
			return BIO_ptr(BIO_new_fp(stream, flags), BIO_free);
		}


		BIO_ptr_all create_bio_base64()
		{
			return BIO_ptr_all(::BIO_new(BIO_f_base64()), ::BIO_free_all);
		}

		BIO_ptr create_bio_s_mem()
		{
			return BIO_ptr(BIO_new(BIO_s_mem()), BIO_free);
		}

		BIO_ptr create_bio_socket(int sock, int close_flag)
		{
			return BIO_ptr(BIO_new_socket(sock, close_flag), BIO_free);
		}

		BIO_METHOD_ptr create_method_mem()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_mem()), BIO_meth_free);
		}

		BIO_METHOD_ptr create_method_secmem()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_secmem()), BIO_meth_free);
		}

		BIO_METHOD_ptr create_method_socket()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_socket()), BIO_meth_free);
		}

		BIO_METHOD_ptr create_method_connect()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_connect()), BIO_meth_free);
		}

		BIO_METHOD_ptr create_method_accept()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_accept()), BIO_meth_free);
		}

		BIO_METHOD_ptr create_method_fd()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_fd()), BIO_meth_free);
		}

		//BIO_METHOD_ptr create_method_log()
		//{
		//	return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_log()), BIO_meth_free);
		//}

		BIO_METHOD_ptr create_method_bio()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_bio()), BIO_meth_free);
		}

		BIO_METHOD_ptr create_method_null()
		{
			return BIO_METHOD_ptr(const_cast<BIO_METHOD*>(BIO_s_null()), BIO_meth_free);
		}

		BIO* push(BIO_ptr p, BIO_ptr append)
		{
			return BIO_push(p.get(), append.get());
		}

	}
}
