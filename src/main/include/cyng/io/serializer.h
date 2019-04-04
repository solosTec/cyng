/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZER_H
#define CYNG_IO_SERIALIZER_H


#include <cyng/core/object_interface_fwd.h>
#include <cyng/intrinsics.h>
#include <cyng/io/io_chrono.hpp>
#include <cyng/io/io_buffer.h>
#include <ostream>

namespace cyng 
{	
	namespace io
	{
		//
		//	catch the stream operators from namespace cyng
		using cyng::operator<<;	
		
		std::ostream& operator<<(std::ostream& os, attr_t const&);
		std::ostream& operator<<(std::ostream& os, param_t const&);
		std::ostream& operator<<(std::ostream& os, std::pair<std::size_t, std::string> const&);
		std::ostream& operator<<(std::ostream& os, null const&);
		std::ostream& operator<<(std::ostream& os, eod const&);
		std::ostream& operator<<(std::ostream& os, version const&);
		std::ostream& operator<<(std::ostream& os, revision const&);
		//	enumeration types (both scoped and unscoped) can have overloaded operators
		std::ostream& operator<<(std::ostream& os, code c);
        //	enumeration types (both scoped and unscoped) can have overloaded operators
        std::ostream& operator<<(std::ostream& os, logging::severity s);
        /**
		 *  format is xx:xx:xx:xx:xx:xx (17 bytes)
		 */
		std::ostream& operator<<(std::ostream& os, mac48 const&);
		std::ostream& operator<<(std::ostream& os, mac64 const&);
		
		std::ostream& operator<<(std::ostream& os, crypto::digest_md5 const&);
		std::ostream& operator<<(std::ostream& os, crypto::digest_sha1 const&);
		std::ostream& operator<<(std::ostream& os, crypto::digest_sha256 const&);
		std::ostream& operator<<(std::ostream& os, crypto::digest_sha512 const&);

		std::ostream& operator<<(std::ostream& os, crypto::aes_128_key const&);
		std::ostream& operator<<(std::ostream& os, crypto::aes_192_key const&);
		std::ostream& operator<<(std::ostream& os, crypto::aes_256_key const&);

		/**
		 * serialization tag: SERIALIZE_PLAIN
		 */
		void serialize_plain(std::ostream&, object const&);

		/**
		 * serialization tag: SERIALIZE_TYPED
		 */
		void serialize_typed(std::ostream&, object const&);

		/**
		 * serialization tag: SERIALIZE_BINARY
		 */
		void serialize_binary(std::ostream& os, object const& obj);

		/**
		 * serialization tag: SERIALIZE_JSON
		 */
		void serialize_json(std::ostream& os, object const& obj);

		/**
		 * serialization tag: SERIALIZE_CSV
		 */
		void serialize_csv(std::ostream& os, object const& obj);

		/**
		 * serialize an object with tag SERIALIZE_PLAIN
		 */
		std::string to_str(object const&);

		/**
		 * serialize a vector with tag SERIALIZE_PLAIN
		 */
		std::string to_str(vector_t const&);

		/**
		 * serialize a tuple with tag SERIALIZE_PLAIN
		 */
		std::string to_str(tuple_t const&);
	}
}

#endif // CYNG_IO_SERIALIZER_H

