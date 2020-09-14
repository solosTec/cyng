/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/csv.h>
#include <cyng/object.h>
#include <cyng/csv/csv_parser.h>
#include <cyng/factory.h>
#include <cyng/io/serializer.h>
#include <cyng/io/serializer/csv.hpp>
#include <cyng/factory.h>

#include <ios>
#include <iterator>
#include <fstream>

namespace cyng 
{
	namespace csv 
	{
		tuple_t read(std::string const& inp)
		{
			auto const r = parse_csv(inp);
			return (r.second)
				? r.first
				: tuple_t{}
			;
		}
		
		tuple_t read(buffer_t const& inp)
		{
			auto const r = parse_csv(inp);
			return (r.second)
				? r.first
				: tuple_t{}
			;
		}
		
		vector_t read_file(std::string const& p)
		{
			vector_t vec;
			std::ifstream fs(p);
			if (fs.is_open())	
			{
				fs.unsetf(std::ios::skipws);
				
				//
				//	read CSV file line by line
				//
				std::string line;
				while (std::getline(fs, line, '\n')) {
					if (!line.empty()) {
						auto r = parse_csv(line);
						if (r.second)	vec.push_back(make_object(std::move(r.first)));
					}
				}

				//
				//	The double parenthesis are required to stop compiler see the string inp
				//	as a function.
				//
				//const std::string inp((std::istream_iterator<char>(fs)), (std::istream_iterator<char>()));
				//std::string const inp{ std::istream_iterator<char>(fs), std::istream_iterator<char>() };
				//return read(inp);
			}
			return vec;
		}
		
		std::size_t read_file(std::string const& p, std::function<void(param_map_t const&)> cb)
		{

			std::size_t counter{ 0 };
			std::ifstream fs(p);
			if (fs.is_open())
			{
				fs.unsetf(std::ios::skipws);

				std::string line;

				//
				//	read header
				//
				if (std::getline(fs, line, '\n')) {

					auto r = parse_csv(line);
					if (r.second) {

						std::vector<std::string> header;
						std::transform(r.first.begin(), r.first.end(), std::back_inserter(header), [](object obj) {
							return cyng::io::to_str(obj);
							});

						//
						//	read CSV file line by line
						//
						while (std::getline(fs, line, '\n')) {
							if (!line.empty()) {
								r = parse_csv(line);
								if (r.second && r.first.size() <= header.size()) {
									param_map_t pmap;
									std::size_t idx{ 0 };
									for (auto const& obj : r.first) {
										pmap.emplace(header.at(idx), obj);
										++idx;
									}
									cb(pmap);
									++counter;
								}
							}
						}
					}
				}

			}
			return counter;
		}

		bool write_file(std::string const& p, vector_t const& vec)
		{
			std::ofstream fs(p);
			if (fs.is_open())
			{
				write(fs, vec);
				return true;
			}
			return false;
		}

		void write(std::ostream& os, object const& obj)
		{
			io::serialize_csv(os, obj);
		}

		void write(std::ostream& os, vector_t const& vec)
		{
			cyng::io::serializer <vector_t, cyng::io::SERIALIZE_CSV>::write(os, vec);
		}

		std::string to_string(object const& obj)
		{
			std::stringstream ss;
			write(ss, obj);
			return ss.str();
		}

		std::string to_string(tuple_t const& tpl)
		{
			std::stringstream ss;
			cyng::io::serializer <tuple_t, cyng::io::SERIALIZE_CSV>::write(ss, tpl);
			return ss.str();
		}

		std::string to_string(vector_t const& vec)
		{
			std::stringstream ss;
			cyng::io::serializer <vector_t, cyng::io::SERIALIZE_CSV>::write(ss, vec);
			return ss.str();
		}

	}
}

