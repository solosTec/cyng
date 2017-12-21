/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCC_DRIVER_H
#define CYNG_DOCC_DRIVER_H

#include <cyng/docscript/tokenizer.h>
#include <cyng/docscript/lexer.h>
#include <cyng/docscript/statistics.h>
#include <cyng/intrinsics/sets.h>
#include <chrono>
#include <boost/filesystem.hpp>

namespace cyng
{
	namespace docscript
	{
		class reader;

		/**
		* Controls the process of reading, compiling and generating of docsript files.
		*/
		class driver
		{
			friend class reader;

		public:
			driver(std::vector< std::string >const& inc, int verbose);
			virtual ~driver();

			/**
			* @param master master file
			* @param tmp temporary intermediate file
			* @param out output file (html)
			*/
			int run(boost::filesystem::path const& master
				, boost::filesystem::path const& tmp
				, boost::filesystem::path const& out
				, bool body_only);

		private:
			int run(boost::filesystem::path const& inp, std::size_t);
			int open_and_run(boost::filesystem::path const& inp, std::size_t);
			void finish(boost::filesystem::path const& body, boost::filesystem::path const& out);
			void process(boost::filesystem::path const& in, boost::filesystem::path out, bool body_only);

		private:
			/**
			* Manage a list of include directories.
			* If the driver opens a file it searches in all given directories
			* until the spcified file is found.
			*/
			const std::vector< boost::filesystem::path > includes_;

			/**
			* verbosity level. 
			* 0 == print only errors
			*/
			const int verbose_;

			/**
			* meta data
			*/
			std::chrono::system_clock::time_point last_write_time_;
			uintmax_t file_size_;

			/**
			* Frequency table. Used to calculate shannon entropy 
			* of the text.
			*/
			frequency_t	stats_;

			/**
			* Pre-compiling syntax check by counting open and closed
			* brackets.
			*/
			std::int32_t parentheses_;

			/**
			* The symbol stream is stored in a simple list as input for the compiler.
			* This is memory expensive. An other approach could be to store in a file.
			*/
			std::list<symbol>		buffer_;

			/**
			* create tokens for lexer and detect special entities.
			*/
			docscript::tokenizer	tokenizer_;

			/*
			* Process the stream of input tokens and generate symbols
			*/
			docscript::lexer		lexer_;

			//cyng::tuple_t	doc_;

		};

		/*
		* @return last write time and file size of the specified file
		*/
		std::tuple<std::chrono::system_clock::time_point, uintmax_t> read_meta_data(boost::filesystem::path);
		
		boost::filesystem::path verify_extension(boost::filesystem::path p, std::string const& ext);
	}
}

#endif	//	CYNG_DOCC_DRIVER_H
