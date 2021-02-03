#include <cyng/sys/host.h>
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
#include <Windows.h>
#else
#include <filesystem>
#include <cyng/sys/linux.hpp>
#endif

#include <iostream>

namespace cyng {
	namespace sys {

		std::chrono::milliseconds get_uptime() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			return std::chrono::milliseconds(GetTickCount64());
#else
			//	"/proc/uptime"
			//	 1. number of seconds the system has been up
			//	 2. time each core has spent idle, in seconds
			double uptime{0}, idletime{0};
			read_virtual_file("/proc/uptime", uptime, idletime);

			// std::cout << uptime << ", " << idletime << std::endl;
			auto const d = std::chrono::duration<double>(uptime * 100.0);
			return std::chrono::duration_cast<std::chrono::milliseconds>(d);
#endif
		}

	}
}
