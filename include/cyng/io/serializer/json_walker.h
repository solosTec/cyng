/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_JSON_WALKER_H
#define CYNG_IO_JSON_WALKER_H

#include <cyng/obj/algorithm/dom_walker.h>
#include <ostream>

namespace cyng {
	namespace io {
		class json_walker : public dom_walker {
		public:
			json_walker(std::ostream&);
			virtual void visit(object const&, type_code, std::size_t depth, walker_state) override;
			virtual void open(type_code, std::size_t depth, std::size_t size) override;
			virtual void close(type_code, std::size_t depth, walker_state, type_code parent_type) override;
			virtual void pair(std::size_t, std::size_t depth) override;
			virtual void pair(std::string const&, std::size_t depth) override;
			virtual void pair(obis const&, std::size_t depth) override;
			virtual void simple(attr_t const&, std::size_t depth, walker_state) override;
			virtual void simple(param_t const&, std::size_t depth, walker_state) override;
			virtual void simple(prop_t const&, std::size_t depth, walker_state) override;

		private:
			static std::string indentation(std::size_t depth);

		private:
			std::ostream& os_;
			bool nl_;	//	last out was an NL
		};
	}
}
#endif
