/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/appender/rolling_file.h>
#include <cyng/log/appender.h>
#include <cyng/io/ostream.h>

#include <sstream>

namespace cyng {

	namespace logging {

		rolling_file::rolling_file(std::filesystem::path const& p, std::size_t rotation_size)
			: path_(p)
			, rotation_size_(rotation_size)
			, log_stream_(path_.string(), std::fstream::app)
			, uncommitted_{ 0 }
		{
			BOOST_ASSERT_MSG(log_stream_.is_open(), "cannot open log file");
		} 

		rolling_file::~rolling_file()
		{
			if (log_stream_.is_open()) {
				log_stream_.close();
			}
		}

		void rolling_file::write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg) {
			//
			//	To write simple '\n' instead of std::endl
			//	speeds up writing by factor > 20
			//
			log_stream_
				<< format(ts, lev, tip, msg)
				<< '\n'
				;

			//
			//	64 is an empirical value
			//
			if (uncommitted_++ > 64) {
				//
				//	reset uncommited counter
				//
				uncommitted_ = 0;

				//
				//	write cache to disc
				//
				log_stream_.flush();

				//
				//	test file size
				//
				test_file_size();

			}
		}

		void rolling_file::test_file_size() {
			const auto fs = std::filesystem::file_size(path_);
			if (fs > rotation_size_)
			{	//	32 MB

				//
				//	close file
				//
				log_stream_.close();

				//
				//	create backup file
				//
				create_backup_file();

				//
				//	reopen file
				//
				log_stream_.open(path_.string());
			}

		}

		void rolling_file::create_backup_file()
		{
			//std::pair<std::time_t, double> r = chrono::to_dbl_time_point(std::chrono::system_clock::now());
			//std::tm tm = cyng::chrono::convert_utc(r.first);

			auto const now = std::chrono::system_clock::now();
			auto tt = std::chrono::system_clock::to_time_t(now);

			//	build a filename for backup file
			std::stringstream ss;
			ss
				//<< chrono::year(tm)
				<< 'T'
				<< std::setw(12)
				<< std::setfill('0')
				<< tt
				//<< chrono::day_of_year(tm)
				<< '_'
				//<< chrono::time_of_day(tm)	// in seconds
				;

			auto const tag = ss.str();
			auto const backup = path_.parent_path() / ((path_.stem().string() + "_backup_" + tag) + path_.extension().string());

			std::filesystem::rename(path_, backup);
		}
	}
}

