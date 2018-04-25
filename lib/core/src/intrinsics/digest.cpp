/*
* The MIT License (MIT)
*
* Copyright (c) 2017 Sylko Olzscher
*
*/

#include <cyng/intrinsics/digest.h>

namespace cyng
{
	namespace crypto
	{

		digest_md5::digest_md5()
			: data_({ { 0 } })
		{}
		digest_md5::digest_md5(value_type const& v)
			: data_(v)
		{}
		digest_md5::digest_md5(value_type&& v)
			: data_(std::forward<value_type>(v))
		{}
		digest_md5::digest_md5(digest_md5 const& v)
			: data_(v.data_)
		{}

		digest_sha1::digest_sha1()
			: data_({ { 0 } })
		{}
		digest_sha1::digest_sha1(value_type const& v)
			: data_(v)
		{}
		digest_sha1::digest_sha1(value_type&& v)
			: data_(std::forward<value_type>(v))
		{}
		digest_sha1::digest_sha1(digest_sha1 const& v)
			: data_(v.data_)
		{}

		digest_sha256::digest_sha256()
			: data_({ { 0 } })
		{}
		digest_sha256::digest_sha256(value_type const& v)
			: data_(v)
		{}
		digest_sha256::digest_sha256(value_type&& v)
			: data_(std::forward<value_type>(v))
		{}
		digest_sha256::digest_sha256(digest_sha256 const& v)
			: data_(v.data_)
		{}

		digest_sha512::digest_sha512()
			: data_({ { 0 } })
		{}
		digest_sha512::digest_sha512(value_type const& v)
			: data_(v)
		{}
		digest_sha512::digest_sha512(value_type&& v)
			: data_(std::forward<value_type>(v))
		{}
		digest_sha512::digest_sha512(digest_sha512 const& v)
			: data_(v.data_)
		{}
	}

	/**
	* Compare digests
	*/
	bool operator==(crypto::digest_sha512 const&, crypto::digest_sha512 const&)
	{
		return false;
	}

	bool operator!=(crypto::digest_sha512 const&, crypto::digest_sha512 const&)
	{
		return false;
	}
}

namespace std
{
	bool equal_to<cyng::crypto::digest_md5>::operator()(cyng::crypto::digest_md5 const& c1, cyng::crypto::digest_md5 const& c2) const noexcept
	{
		return c1.data_ == c2.data_;
	}

	bool equal_to<cyng::crypto::digest_sha1>::operator()(cyng::crypto::digest_sha1 const& c1, cyng::crypto::digest_sha1 const& c2) const noexcept
	{
		return c1.data_ == c2.data_;
	}

	bool equal_to<cyng::crypto::digest_sha256>::operator()(cyng::crypto::digest_sha256 const& c1, cyng::crypto::digest_sha256 const& c2) const noexcept
	{
		return c1.data_ == c2.data_;
	}

	bool equal_to<cyng::crypto::digest_sha512>::operator()(cyng::crypto::digest_sha512 const& c1, cyng::crypto::digest_sha512 const& c2) const noexcept
	{
		return c1.data_ == c2.data_;
	}
}

