#include <cyng/net/server_factory.hpp>
namespace cyng {
	namespace net {
		server_factory::server_factory(controller& ctl) noexcept
			: ctl_(ctl)
			, uuid_rgn_()
		{}
	}
}