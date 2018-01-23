/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_TYPED_HPP
#define CYNG_IO_SERIALIZE_TYPED_HPP

#include <cyng/io/serializer/plain.hpp>
#include <cyng/intrinsics/traits/tag_names.hpp>

namespace cyng 
{	
	namespace io
	{

		template <typename T>
		struct serializer<T, SERIALIZE_TYPED>
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
		struct serializer <version, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, version const& v);
		};
		
		template <>
		struct serializer <revision, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, revision const& v);
		};

		template <>
		struct serializer <std::chrono::system_clock::time_point, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::chrono::system_clock::time_point tp);
		};
 		
 		template <>
 		struct serializer <std::string, SERIALIZE_TYPED>
 		{
 			static std::ostream& write(std::ostream& os, std::string const& v);
 		};
 		
// 		template <>
// 		struct serializer <boost::uuids::uuid, SERIALIZE_TYPED>
// 		{
// 			static std::ostream& write(std::ostream& os, boost::uuids::uuid const& v);
// 		};
// 
// 		template <>
// 		struct serializer <tuple_t, SERIALIZE_TYPED>
// 		{
// 			static std::ostream& write(std::ostream& os, tuple_t const& v);
// 		};
// 
// 		template <>
// 		struct serializer <vector_t, SERIALIZE_TYPED>
// 		{
// 			static std::ostream& write(std::ostream& os, vector_t const& v);
// 		};
// 
// 		template <>
// 		struct serializer <set_t, SERIALIZE_TYPED>
// 		{
// 			static std::ostream& write(std::ostream& os, set_t const& v);
// 		};


		// 	template <>
	// 	struct serializer <boost::filesystem::path, SERIALIZE_TYPED>
	// 	{
	// 		static std::ostream& write(std::ostream& os, boost::filesystem::path const& v);
	// 	};
		
	}
}

#endif // CYNG_IO_SERIALIZE_TYPED_HPP

