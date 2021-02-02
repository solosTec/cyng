/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
 /*
  * The MIT License (MIT)
  *
  * Copyright (c) 2021 Sylko Olzscher
  *
  */
#ifndef DOCC_LOG_APPENDER_ROLLING_FILE_H
#define DOCC_LOG_APPENDER_ROLLING_FILE_H

#include <cyng/log/appender.h>
#include <filesystem>
#include <fstream>

namespace docscript {
	namespace logging {

		class rolling_file
		{
		public:
			rolling_file(std::filesystem::path const&, std::size_t);
			~rolling_file();

			void write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg);

		private:
			void test_file_size();
			void create_backup_file();

		private:
			std::filesystem::path const path_;
			std::size_t	const rotation_size_;
			std::ofstream log_stream_;
			std::size_t uncommitted_;
		};
	}
}

#endif

