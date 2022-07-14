#include <cyng/sys/filesystem.h>

namespace cyng {
	namespace sys {

		std::chrono::system_clock::time_point get_write_time(std::filesystem::path const& p)
		{
			auto const ftp = std::filesystem::last_write_time(p);
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftp - std::filesystem::file_time_type::clock::now()
				+ std::chrono::system_clock::now());
			auto const tt = std::chrono::system_clock::to_time_t(sctp);
			return std::chrono::system_clock::from_time_t(tt);
		}

	}
}
