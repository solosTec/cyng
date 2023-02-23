#include <cyng/net/server_proxy.h>

namespace cyng {
    namespace net {
        server_proxy::server_proxy()
            : server_()
            , protocol_(protocol::OTHER) {}

        server_proxy::server_proxy(channel_ptr ptr, protocol p)
            : server_(ptr)
            , protocol_(p) {
            BOOST_ASSERT(ptr);
        }

        protocol server_proxy::get_protocol() const { return protocol_; }

        // server_proxy &server_proxy::operator=(channel_ptr channel) {
        //     BOOST_ASSERT(channel);
        //     //
        //     //  close running server
        //     //
        //     if (server_ && server_->is_open()) {
        //         server_->stop();
        //     }
        //     server_ = channel;
        //     return *this;
        // }

        server_proxy &server_proxy::operator=(server_proxy &&proxy) noexcept {
            if (server_ && server_->is_open()) {
                server_->stop();
            }
            server_ = std::move(proxy.server_);
            protocol_ = proxy.protocol_;
            return *this;
        }

        void server_proxy::stop() {
            if (server_) {
                server_->stop();
            }
        }

    } // namespace net
} // namespace cyng
