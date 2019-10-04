/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include <cyng/crypto/util/x509.h>
#include <cyng/crypto/util/factory.h>
#include <cyng/crypto/bio.h>
#include <cyng/crypto/util/print.h>
#include <boost/predef.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/objects.h>
#include <openssl/ocsp.h>

namespace cyng 
{
	namespace crypto
	{

		/**
		 * sign cert
		 */
		bool sign_x509(X509* cert, EVP_PKEY* pkey, const EVP_MD* md);

		bool generate_x509_cert_request(const char* pC	//	country
			, const char* pST	//	province
			, const char* pL	//	city
			, const char* pO	//	organization
			, const char* pCN	//	common
			, const char* filename
			, int bits)
		{
			// 1. generate rsa key with exponent RSA_F4
			auto bnp = create_bignum_rsa_f4();
			if (!bnp) return false;

			auto rsap = create_rsa_key(bnp.get(), bits);
			if (!rsap) return false;

			// 2. create a x509 request
			auto x509rp = create_x509_request(1);
			if (!x509rp) return false;

			// 3. set subject of x509 req
			auto x509_name = X509_REQ_get_subject_name(x509rp.get());
			if (!x509_name) return false;
			add_entry_by_txt(x509_name, "C", pC);
			add_entry_by_txt(x509_name, "ST", pST);
			add_entry_by_txt(x509_name, "L", pL);
			add_entry_by_txt(x509_name, "O", pO);
			add_entry_by_txt(x509_name, "CN", pCN);

			// 4. set public key of x509 req
			auto evp_pkeyp = create_evp_pkey();
			if (!evp_pkeyp) return false;

			//	rsap will be released by evp_pkeyp
			EVP_PKEY_assign_RSA(evp_pkeyp.get(), rsap.release());

			auto ret = X509_REQ_set_pubkey(x509rp.get(), evp_pkeyp.get());
			if (ret != 1) return false;

			// 5. set sign key of x509 req
			ret = X509_REQ_sign(x509rp.get(), evp_pkeyp.get(), EVP_sha1());
			if (ret <= 0) return false;

			// 6. write certificate request file
			auto biop = create_bio_file(filename, "w+");
			if (!biop) return false;

			ret = PEM_write_bio_X509_REQ(biop.get(), x509rp.get());
			if (ret != 1) return false;

			return true;
		}

		bool add_entry_by_txt(X509_NAME* x509_name, const char* subject, const char* txt)
		{
			auto const ret = X509_NAME_add_entry_by_txt(x509_name, subject, MBSTRING_ASC, (const unsigned char*)txt, -1, -1, 0);
			return ret == 1;
		}

		X509_ptr load_CA(const char* filename)
		{
			X509* x509p = nullptr;
			auto biop = create_bio_file(filename, "r");
			PEM_read_bio_X509(biop.get(), &x509p, NULL, NULL);
			return X509_ptr(x509p, X509_free);
		}

		RSA_ptr load_private_key(const char* filename)
		{
			RSA* rsa = nullptr;
			auto biop = create_bio_file(filename, "r");
			PEM_read_bio_RSAPrivateKey(biop.get(), &rsa, NULL, NULL);
#ifdef _DEBUG
			print_stdout_RSA(rsa);
#endif

			return RSA_ptr(rsa, RSA_free);
		}

		EVP_PKEY_ptr load_CA_private_key(const char* filename)
		{
			auto rsap = load_private_key(filename);
			auto evp_pkeyp = create_evp_pkey();

			//
			//	EVP_PKEY manages lifetime of RSA structure
			//
			EVP_PKEY_assign_RSA(evp_pkeyp.get(), rsap.release());
			return evp_pkeyp;
		}

		X509_REQ_ptr load_x509_request(const char* filename)
		{
			X509_REQ* x509_reqp = nullptr;
			auto biop = create_bio_file(filename, "r");
			PEM_read_bio_X509_REQ(biop.get(), &x509_reqp, NULL, NULL);
			return X509_REQ_ptr(x509_reqp, X509_REQ_free);
		}

		bool sign_x509(X509* cert, EVP_PKEY* pkey, const EVP_MD* md)
		{
			auto mctxp = create_evp_ctx();

			EVP_PKEY_CTX* pkctx = NULL;

			auto rv = EVP_DigestSignInit(mctxp.get(), &pkctx, md, NULL, pkey);

			if (rv > 0) {
				rv = X509_sign_ctx(cert, mctxp.get());
			}
			return rv > 0;
		}

		bool sign_x509_with_CA(const char* caFile //  cacert.pem
			, const char* caPrivateKeyFile //	cakey.pem
			, const char* x509ReqFile	//	x509Req.pem
			, const char* szUserCert	//	cert.pem
			, long days)
		{

			auto cap = load_CA(caFile);
			if (!cap)	return false;

			auto evp_pkeyp = load_CA_private_key(caPrivateKeyFile);
			if (!evp_pkeyp)	return false;


			auto x509_reqp = load_x509_request(x509ReqFile);
			if (!x509_reqp)	return false;

			// set version to X509 v3 certificate
			auto certp = create_x509(2);
			if (!certp)	return false;

			// set serial
			int serial = 1;
			ASN1_INTEGER_set(X509_get_serialNumber(certp.get()), serial);

			// set issuer name from ca
			if (!X509_set_issuer_name(certp.get(), X509_get_subject_name(cap.get()))) {
				return false;
			}

			// set time
			X509_gmtime_adj(X509_get_notBefore(certp.get()), 0);
			X509_gmtime_adj(X509_get_notAfter(certp.get()), 60ul * 60ul * 24ul * days);

			// set subject from req
			auto tmpname = X509_REQ_get_subject_name(x509_reqp.get());
			auto subject = X509_NAME_dup(tmpname);
			if (!X509_set_subject_name(certp.get(), subject)) {
				return false;
			}

			// set pubkey from req
			auto pktmp = X509_REQ_get_pubkey(x509_reqp.get());
			auto ret = X509_set_pubkey(certp.get(), pktmp);
			EVP_PKEY_free(pktmp);
			if (!ret) return false;

			// sign cert
			if (!sign_x509(certp.get(), evp_pkeyp.get(), EVP_sha1())) {
				return false;
			}

			auto biop = create_bio_file(szUserCert, "w+");
			if (!PEM_write_bio_X509(biop.get(), certp.get())) {
				return false;
			}

			return true;
		}

//		EVP_PKEY * generate_key()
//		{
//			/* Allocate memory for the EVP_PKEY structure. */
//			EVP_PKEY * pkey = EVP_PKEY_new();
//			if (!pkey)
//			{
//				std::cerr << "Unable to create EVP_PKEY structure." << std::endl;
//				return NULL;
//			}
//#if OPENSSL_VERSION_NUMBER >= 0x0090800fL
//    
//            RSA  *rsa = RSA_new();
//            if (rsa == NULL) {
//                return NULL;
//            }
//
//            BIGNUM * e = BN_new();
//            if (e == NULL) {
//                RSA_free(rsa);
//                return NULL;
//            }
//
//            if (BN_set_word(e, RSA_F4) == 0) {
//                BN_free(e);
//                RSA_free(rsa);
//                return NULL;
//            }
//            
//            if (RSA_generate_key_ex(rsa, 2048, e, NULL) == 0) {
//                BN_free(e);
//                RSA_free(rsa);
//                return NULL;
//            }
//
//            BN_free(e);            
//#else
//			/* Generate the RSA key and assign it to pkey. */
//			RSA * rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
//#endif
//			if (!EVP_PKEY_assign_RSA(pkey, rsa))
//			{
//				std::cerr << "Unable to generate 2048-bit RSA key." << std::endl;
//				EVP_PKEY_free(pkey);
//				return NULL;
//			}
//
//			/* The key has been generated, return it. */
//			return pkey;
//		}

		//X509 * generate_x509(EVP_PKEY * pkey
		//	, long seconds
		//	, const char* pC
		//	, const char* pO
		//	, const char* pCN)
		//{
		//	/* Allocate memory for the X509 structure. */
		//	X509 * x509 = X509_new();
		//	if (!x509)
		//	{
		//		std::cerr << "Unable to create X509 structure." << std::endl;
		//		return NULL;
		//	}

		//	/* Set the serial number. */
		//	ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

		//	/* This certificate is valid from now until exactly one year from now. */
		//	X509_gmtime_adj(X509_get_notBefore(x509), 0);
		//	//X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);	//	seconds
		//	X509_gmtime_adj(X509_get_notAfter(x509), seconds);	//	seconds

		//	/* Set the public key for our certificate. */
		//	X509_set_pubkey(x509, pkey);

		//	/* We want to copy the subject name to the issuer name. */
		//	X509_NAME * name = X509_get_subject_name(x509);

		//	/* Set the country code and common name. */
		//	//X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"CA", -1, -1, 0);
		//	//X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"MyCompany", -1, -1, 0);
		//	//X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0);
		//	::X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)pC, -1, -1, 0);
		//	::X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)pO, -1, -1, 0);
		//	::X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)pCN, -1, -1, 0);

		//	/* Now set the issuer name. */
		//	X509_set_issuer_name(x509, name);

		//	/* Actually sign the certificate with our key. */
		//	if (!X509_sign(x509, pkey, EVP_sha1()))
		//	{
		//		std::cerr << "Error signing certificate." << std::endl;
		//		X509_free(x509);
		//		return NULL;
		//	}

		//	return x509;
		//}

//		bool write_to_disk(EVP_PKEY * pkey, X509 * x509)
//		{
//            //  open the PEM file for writing the key to disk
//#if BOOST_OS_WINDOWS
//			FILE * pkey_file = nullptr;
//            auto err = fopen_s(&pkey_file, "key.pem", "wb");
//			if (err != 0)
//			{
//				std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
//				return false;
//			}
//#else
//            FILE * pkey_file = fopen("key.pem", "wb");
//            if (pkey_file == nullptr)
//            {
//                std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
//                return false;
//            }
//
//#endif
//            //  Write the key to disk
//			bool ret = ::PEM_write_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
//			fclose(pkey_file);
//
//			if (!ret)
//			{
//				std::cerr << "Unable to write private key to disk." << std::endl;
//				return false;
//			}
//
//#if BOOST_OS_WINDOWS
//            /* Open the PEM file for writing the certificate to disk. */
//			FILE * x509_file;
//			err = ::fopen_s(&x509_file, "cert.pem", "wb");
//			if (err != 0)
//			{
//				std::cerr << "Unable to open \"cert.pem\" for writing." << std::endl;
//				return false;
//			}
//#else
//            //  open the PEM file for writing the certificate to disk
//            FILE * x509_file = fopen("cert.pem", "wb");
//            if (x509_file == nullptr)
//            {
//                std::cerr << "Unable to open \"cert.pem\" for writing." << std::endl;
//                return false;
//            }
//#endif
//
//            //  Write the certificate to disk
//			ret = ::PEM_write_X509(x509_file, x509);
//			fclose(x509_file);
//
//			if (!ret)
//			{
//				std::cerr << "Unable to write certificate to disk." << std::endl;
//				return false;
//			}
//
//			return true;
//		}

	}
}
