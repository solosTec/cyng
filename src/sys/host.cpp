#include <cyng/sys/host.h>
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
#include <Windows.h>
#else
#include <fstream>
#include <filesystem>
#endif

namespace cyng {
	namespace sys {

		std::chrono::milliseconds get_uptime() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			return std::chrono::milliseconds(GetTickCount64());
#else
			//	"/proc/uptime"
			//	 1. number of seconds the system has been up
			//	 2. time each core has spent idle, in seconds
			auto const proc_path = std::filesystem::path("proc/uptime");
			std::ifstream ifs(p.string(), std::ios::in);
			double uptime, idletime;
			ifs >> uptime >> idletime;
			return uptime * 100.0;
#endif
		}

	}
}
