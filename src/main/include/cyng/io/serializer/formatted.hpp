/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_FORMATTED_HPP
#define CYNG_IO_SERIALIZE_FORMATTED_HPP

#include <cyng/io/serializer/plain.hpp>
#include <cyng/intrinsics/traits/tag_names.hpp>

namespace cyng 
{	
	namespace io
	{

		template <typename T>
		struct serializer<T, SERIALIZE_FORMATTED>
		{		
			static std::ostream& write(std::ostream& os, T const& v)
			{
				//
				//	take the plain format as default
				//
				serializer<T, SERIALIZE_PLAIN>::write(os, v);
				os << cyng::traits::get_tag_name<T>();
				return os;
			}
		};
		
		template <>
		struct serializer <version, SERIALIZE_FORMATTED>
		{
			static std::ostream& write(std::ostream& os, version const& v);
		};
		
		template <>
		struct serializer <revision, SERIALIZE_FORMATTED>
		{
			static std::ostream& write(std::ostream& os, revision const& v);
		};

		// 		template <>
// 		struct serializer <bool, SERIALIZE_FORMATTED>
// 		{
// 			static std::ostream& write(std::ostream& os, bool v);
// 		};
// 		
// 		template <>
// 		struct serializer <buffer_t, SERIALIZE_FORMATTED>
// 		{
// 			static std::ostream& write(std::ostream& os, buffer_t const& v);
// 		};
// 		
// 		template <>
// 		struct serializer <boost::uuids::uuid, SERIALIZE_FORMATTED>
// 		{
// 			static std::ostream& write(std::ostream& os, boost::uuids::uuid const& v);
// 		};
// 
// 		template <>
// 		struct serializer <tuple_t, SERIALIZE_FORMATTED>
// 		{
// 			static std::ostream& write(std::ostream& os, tuple_t const& v);
// 		};
// 
// 		template <>
// 		struct serializer <vector_t, SERIALIZE_FORMATTED>
// 		{
// 			static std::ostream& write(std::ostream& os, vector_t const& v);
// 		};
// 
// 		template <>
// 		struct serializer <set_t, SERIALIZE_FORMATTED>
// 		{
// 			static std::ostream& write(std::ostream& os, set_t const& v);
// 		};


		// 	template <>
	// 	struct serializer <boost::filesystem::path, SERIALIZE_FORMATTED>
	// 	{
	// 		static std::ostream& write(std::ostream& os, boost::filesystem::path const& v);
	// 	};
		
	}
}

#endif // CYNG_IO_SERIALIZE_FORMATTED_HPP

