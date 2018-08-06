/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#if (CYNG_ODBC_INSTALLED == 1)
#ifndef CYNG_DB_ODBC_EXCEPTION_HPP
#define CYNG_DB_ODBC_EXCEPTION_HPP

#include "odbc_defs.h"
#include <boost/exception/all.hpp>

namespace cyng
{
	namespace db	
	{
		namespace odbc	
		{

			struct odbc_exception_base
				: virtual public std::exception
				, virtual public boost::exception
			{};

			class odbc_error
				: virtual public odbc_exception_base
			{
			public:
				odbc_error(std::string const& what)
					: what_("ODBC: " + what)
				{}

				virtual const char *what() const throw()	
				{
					return what_.c_str();
				}

			private:
				const std::string what_;
			};

		}	//	odbc
	}	//	db
}	//	cyng


#endif	//	CYNG_DB_ODBC_EXCEPTION_HPP
#endif