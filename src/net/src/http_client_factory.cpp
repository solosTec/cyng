#include <cyng/net/http_client_factory.h>
namespace cyng {
    namespace net {

        http_client_factory::http_client_factory(controller &ctl) noexcept
            : ctl_(ctl)
            , uuid_rgn_() {}

        http_client_proxy http_client_factory::create_proxy(
            std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::system::error_code)> cb_failed,
            std::function<void(endpoint_t, channel_ptr)> cb_connect,
            cb_receive_t cb_receive,
            cb_disconnect_t on_disconnect) {

            return {create_channel(cb_failed, cb_connect, cb_receive, on_disconnect)};
        }

        channel_ptr http_client_factory::create_channel(
            std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::system::error_code)> cb_failed,
            std::function<void(endpoint_t, channel_ptr)> cb_connect,
            cb_receive_t cb_receive,
            cb_disconnect_t on_disconnect) {

            //
            //	create an uuid
            //
            std::string const tag = boost::uuids::to_string(uuid_rgn_());

            return ctl_.create_named_channel_with_ref<http_client>(tag, ctl_, cb_failed, cb_connect, cb_receive, on_disconnect)
                .first;
        }

    } // namespace net
} // namespace cyng
