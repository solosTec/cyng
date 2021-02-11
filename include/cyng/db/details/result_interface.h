/*
* The MIT License (MIT)
*
* Copyright (c) 2021 Sylko Olzscher
*
*/

#ifndef CYNG_DB_RESULT_INTERFACE_H
#define CYNG_DB_RESULT_INTERFACE_H

#include <cyng/obj/object.h>
#include <memory>
#include <utility>
#include <boost/system/error_code.hpp>

namespace cyng
{
	namespace db 
	{
	
		/*	@brief abstract SQL select interface.
		 *
		 *	Each implementation of a SQL select must implement at least that
		 *	interface
		 */
		class result_interface
		{
		public:
			/**
			 * @param index columns index (starts with 1)
			 * @param code CYNG data type that is expected
			 * @param size columns size (relevant for some databases)
			 */
			virtual object get(int index, std::size_t code, std::size_t size) = 0;
			virtual std::size_t column_count() = 0;
		};
		
		using result_ptr = std::shared_ptr< result_interface >;
		
	}
}

#endif
