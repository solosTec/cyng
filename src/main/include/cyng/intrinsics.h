/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_H
#define CYNG_INTRINSICS_H

#include <cyng/intrinsics/null.h>
#include <cyng/intrinsics/eod.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/intrinsics/version.h>
#include <cyng/intrinsics/op.h>
#include <cyng/intrinsics/label.h>
#include <cyng/chrono.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/mac.h>
#include <cyng/intrinsics/color.hpp>
#include <cyng/log/severity.h>
#include <cyng/intrinsics/lockable.h>
#include <cyng/intrinsics/digest.h>
#include <cyng/crypto/aes_keys.h>
#include <cyng/compatibility/file_system.hpp>

#include <boost/system/error_code.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio.hpp>

#if defined(CYNG_ODBC_INSTALLED)

#include <sql.h> 
#include <functional>

namespace std
{
	template<>
	struct equal_to<SQL_TIMESTAMP_STRUCT>
	{
		using result_type = bool;
		using first_argument_type = SQL_TIMESTAMP_STRUCT;
		using second_argument_type = SQL_TIMESTAMP_STRUCT;
	
		inline bool operator()(SQL_TIMESTAMP_STRUCT const& ts1, SQL_TIMESTAMP_STRUCT const& ts2) const noexcept
		{
			return ts1.year == ts2.year
				&& ts1.month == ts2.month
				&& ts1.day == ts2.day
				&& ts1.hour == ts2.hour
				&& ts1.minute == ts2.minute
				&& ts1.second == ts2.second
				&& ts1.fraction == ts2.fraction;
	
		}
	};

	template<>
	struct less<SQL_TIMESTAMP_STRUCT>
	{
		using result_type = bool;
		using first_argument_type = SQL_TIMESTAMP_STRUCT;
		using second_argument_type = SQL_TIMESTAMP_STRUCT;

		inline bool operator()(SQL_TIMESTAMP_STRUCT const& ts1, SQL_TIMESTAMP_STRUCT const& ts2) const noexcept
		{
			return (ts1.year == ts2.year)
				? ((ts1.month == ts2.month)
					? ((ts1.day == ts2.day)
						? ((ts1.hour == ts2.hour)
							? ((ts1.minute == ts2.minute)
								? ((ts1.second == ts2.second) ? ts1.fraction < ts2.fraction : false)
								: ts1.minute < ts2.minute)
							: ts1.hour < ts2.hour)
						: ts1.day < ts2.day)
					: ts1.month < ts2.month)
				: ts1.year < ts2.year
				;
		}
};

}
#else
namespace cyng
{
	struct dummy_SQL_TIMESTAMP_STRUCT
	{};
}
#endif

#endif 	// CYNG_INTRINSICS_H

