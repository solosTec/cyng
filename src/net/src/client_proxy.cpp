#include <cyng/net/client_proxy.h>

namespace cyng {
    namespace net {
        client_proxy::client_proxy()
            : client_() {}

        client_proxy::client_proxy(channel_ptr ptr, std::function<void(cyng::buffer_t)> direct_send)
            : client_(ptr)
            , direct_send_(direct_send) {
            BOOST_ASSERT(ptr);
        }

        client_proxy &client_proxy::operator=(client_proxy &&proxy) noexcept {
            //
            //  close running client
            //
            if (client_ && client_->is_open()) {
                client_->stop();
            }
            client_ = std::move(proxy.client_);
            direct_send_ = std::move(proxy.direct_send_);
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

        void client_proxy::send(cyng::buffer_t &&data, bool direct) {
            if (client_) {
                if (direct) {
                    direct_send_(data);
                } else {
                    client_->dispatch(1, std::move(data));
                }
            }
        }

        void client_proxy::send(std::string const &data, bool direct) { send(cyng::make_buffer(data), direct); }

        void client_proxy::send(std::deque<buffer_t> &&msg) {
            if (client_) {
                deque_t deq;
                std::transform(msg.begin(), msg.end(), std::back_inserter(deq), [](buffer_t data) -> object {
                    //
                    return make_object(data);
                });
                client_->dispatch(5, deq);
            }
        }

        channel_ptr client_proxy::get_channel() { return client_; }

    } // namespace net
} // namespace cyng
