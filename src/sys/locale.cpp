#include <cyng/sys/locale.h>
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
#include <Windows.h>
//#include <boost/regex/pending/unicode_iterator.hpp>

#include <iostream>
#else
#include <cyng/sys/linux.hpp>
#include <boost/locale/info.hpp>
#endif

#include <iostream>

namespace cyng {
	namespace sys {

		std::string get_system_locale() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			std::string loc;
			constexpr std::size_t BUFFER_SIZE = 512;
			WCHAR wcBuffer[BUFFER_SIZE];
			//auto r = GetUserDefaultLocaleName(wcBuffer, BUFFER_SIZE);
			auto r = GetSystemDefaultLocaleName(wcBuffer, BUFFER_SIZE);
			if (r > 0) {
				for (auto const& c : wcBuffer) {
					if (c == 0)	break;
					loc.push_back(c);
				}				
			}

			return loc;
#else
			return  boost::locale::name();
#endif

		}

	}
}
