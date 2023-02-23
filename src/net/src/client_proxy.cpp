#include <cyng/net/client_proxy.h>

namespace cyng {
    namespace net {
        client_proxy::client_proxy()
            : client_() {}

        client_proxy::client_proxy(channel_ptr ptr)
            : client_(ptr) {
            BOOST_ASSERT(ptr);
        }

        client_proxy &client_proxy::operator=(channel_ptr channel) {
            BOOST_ASSERT(channel);
            //
            //  close running client
            //
            if (client_ && client_->is_open()) {
                client_->stop();
            }
            client_ = channel;
            return *this;
        }

        client_proxy &client_proxy::operator=(client_proxy &&proxy) noexcept {
            //
            //  close running client
            //
            if (client_ && client_->is_open()) {
                client_->stop();
            }
            client_ = std::move(proxy.client_);
            return *this;
        }

        void client_proxy::stop() {
            if (client_) {
                client_->stop();
            }
        }

        void client_proxy::connect(std::string host, std::string service) {
            if (client_) {
                client_->dispatch(0, host, service);
            }
        }

        void client_proxy::close() {
            if (client_) {
                //
                //  close socket
                //
                client_->dispatch(4);
            }
        }

        void client_proxy::send(cyng::buffer_t &&data) {
            if (client_) {
                client_->dispatch(1, std::move(data));
            }
        }

        void client_proxy::send(std::string const &data) { send(cyng::make_buffer(data)); }

        void client_proxy::send(std::deque<buffer_t> &&msg) {
            if (client_) {
                deque_t deq;
                std::transform(msg.begin(), msg.end(), std::back_inserter(deq), [](buffer_t data) -> object {
                    //
                    return make_object(data);
                });
                // for (auto &data : msg) {
                //     client_->dispatch(1, std::move(data));
                // }
                client_->dispatch(5, deq);
            }
        }

    } // namespace net
} // namespace cyng
