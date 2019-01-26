/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#ifndef CYX_DOCSCRIPT_JSON_FILTER_H
#define CYX_DOCSCRIPT_JSON_FILTER_H

//#include <cyng/parser/utf8_parser.h>
#include <cyng/dom/tree_walker.h>

#include <cstdint>
#include <string>
#include <boost/uuid/uuid.hpp>

namespace cyng
{
	namespace filter 
	{
		class json_walker : public tree_walker
		{
		public:
			json_walker(std::ostream&, bool linenumbers, boost::uuids::uuid tag);

			virtual bool enter_node(std::size_t depth, object const&, std::size_t idx, std::size_t total) override;
			virtual void leave_node(std::size_t depth, object const&, std::size_t idx, std::size_t total) override;

		private:
			std::string indentation(std::size_t);
			std::string nl();

		private:
			std::ostream& os_;
			bool const linenumbers_;
			boost::uuids::uuid const tag_;
			std::size_t line_;
		};

		class json
		{
		public:
			json(bool linenumbers, boost::uuids::uuid, std::size_t reserve);
			void put(std::string const&);
			std::string get_result();


		private:
			bool const linenumbers_;
			boost::uuids::uuid const tag_;
			std::string	result_;
		};
	}

}	//	cyng

#endif




