/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <boost/predef.h>
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
#include <cyng/sys/windows.h>
#include <vector>
#include <algorithm>
#include <iterator>

namespace cyng {
	namespace sys
	{

		/**
		 * @return UTF8 version of the wide character string
		 */
		std::string convert_to_utf8(PWCHAR name) {
			int const size = WideCharToMultiByte(CP_UTF8, 0, name, -1, NULL, 0, NULL, NULL);

			std::vector<char> utf8(size);
			int const converted = WideCharToMultiByte(CP_UTF8, 0, name, -1, &utf8[0], utf8.size(), NULL, NULL);

			//
			//	don't copy the last '\0'
			//
			std::string r;
			r.reserve(size);
			std::copy_if(std::begin(utf8), std::end(utf8), std::back_inserter(r), [](char c) {
				return c != 0;
				});
			return r;
		}

		/**
		 * old style: dont't forget to free the allocated memory
		 */
		PIP_ADAPTER_ADDRESSES get_adapter_adresses(ULONG family) {

			//
			// Set the flags to pass to GetAdaptersAddresses
			//
			ULONG flags = GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_ALL_INTERFACES;

			//
			//	contains the address info
			//
			PIP_ADAPTER_ADDRESSES pAddresses = NULL;

			//
			//	memory management
			//	
			ULONG outBufLen = 0;

			// Make an initial call to GetAdaptersAddresses to get the 
			// size needed into the outBufLen variable
			if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
				//
				//	allocate memory to hold adapter info
				//
				free(pAddresses);
				pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
			}

			return (NO_ERROR == GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen))
				? pAddresses
				: nullptr
				;
		}

	}
}

#endif

