#include <cyng/net/client_factory.hpp>

namespace cyng {
	namespace net {

		client_factory::client_factory(controller& ctl) noexcept
			: ctl_(ctl)
			, uuid_rgn_()
		{}

	}
}