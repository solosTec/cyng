#include <cyng/net/server_proxy.h>
namespace cyng {
	namespace net {
		server_proxy::server_proxy()
			: server_()
		{}

		server_proxy::server_proxy(channel_ptr ptr)
			: server_(ptr)
		{
			BOOST_ASSERT(ptr);
		}

		server_proxy& server_proxy::operator=(channel_ptr channel) {
			BOOST_ASSERT(channel);
			server_ = channel;
			return *this;
		}

		void server_proxy::stop() {
			if (server_) {
				server_->stop();
			}
		}

		void server_proxy::start() {
			if (server_) {
				server_->dispatch(0);
			}
		}
	}
}