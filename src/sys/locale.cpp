#include <cyng/sys/locale.h>

#include <iostream>

#include <boost/locale.hpp>

namespace cyng {
	namespace sys {

		std::array<std::string, 4> get_system_locale() {

			boost::locale::generator gen;
			auto loc = gen("");

			//std::cout << std::use_facet<boost::locale::info>(loc).name() << std::endl;
			//std::cout << std::use_facet<boost::locale::info>(loc).country() << std::endl;
			//std::cout << std::use_facet<boost::locale::info>(loc).language() << std::endl;
			//std::cout << std::use_facet<boost::locale::info>(loc).encoding() << std::endl;

			return { std::use_facet<boost::locale::info>(loc).name() 
				, std::use_facet<boost::locale::info>(loc).country()
				, std::use_facet<boost::locale::info>(loc).language()
				, std::use_facet<boost::locale::info>(loc).encoding() };

		}

	}
}
