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
			virtual void visit(object const&, type_code, std::size_t depth, walker_state, bool is_vector) override;
			virtual void open(type_code, std::size_t depth, std::size_t size) override;
			virtual void close(type_code, std::size_t depth, walker_state) override;
			virtual void pair(std::size_t, std::size_t depth) override;
			virtual void pair(std::string, std::size_t depth) override;

		private:
			std::ostream& os_;
			bool nl_;	//	last out was an NL
		};
	}
}
#endif
