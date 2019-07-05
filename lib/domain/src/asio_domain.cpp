/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/vm/domain/asio_domain.h>
#include <cyng/vm/controller.h>
#include <cyng/vm/generator.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>
#include <boost/asio/version.hpp>

namespace cyng 
{
	void ip_tcp_socket_shutdown(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		boost::system::error_code ec;
		s.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		ctx.set_register(ec);
	}

	void ip_tcp_socket_close(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Any asynchronous send, receive or connect operations will be cancelled immediately, 
		//	and will complete with the boost::asio::error::operation_aborted error.
		//
		boost::system::error_code ec;
		s.close(ec);
		ctx.set_register(ec);
	}

	void ip_tcp_socket_cancel(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	This function causes all outstanding asynchronous connect, send and receive operations 
		//	to finish immediately, and the handlers for cancelled operations will be passed 
		//	the boost::asio::error::operation_aborted error.
		//
		boost::system::error_code ec;
		s.cancel(ec);
		ctx.set_register(ec);
	}

	void ip_tcp_socket_is_open(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		ctx.set_return_value(make_object(s.is_open()), 0);
	}

	void ip_tcp_socket_ep_local(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Get the local endpoint of the socket.
		//
		boost::system::error_code ec;
		ctx.push(make_object(s.local_endpoint(ec)));
		ctx.set_register(ec);
	}

	void ip_tcp_socket_ep_local_port(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Get the local endpoint port of the socket.
		//
		boost::system::error_code ec;
		ctx.push(make_object(s.local_endpoint(ec).port()));
		ctx.set_register(ec);
	}

	void ip_tcp_socket_ep_remote(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Get the remote endpoint of the socket.
		//
		boost::system::error_code ec;
		ctx.push(make_object(s.remote_endpoint(ec)));
		ctx.set_register(ec);
	}

	void ip_tcp_socket_ep_remote_port(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Get the remote endpoint of the socket.
		//
		boost::system::error_code ec;
		ctx.push(make_object(s.remote_endpoint(ec).port()));
		ctx.set_register(ec);
	}

	void ip_tcp_socket_connect(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Connect the socket to the specified endpoint.
		//
		const vector_t frame = ctx.get_frame();
		boost::system::error_code ec;
		s.connect(value_cast(frame.at(0), boost::asio::ip::tcp::endpoint()), ec);
		BOOST_ASSERT(s.is_open());
		ctx.set_register(ec);
	}

	void ip_tcp_socket_resolve(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Connect the socket to the specified endpoint using the resolver
		//
		const vector_t frame = ctx.get_frame();
		boost::system::error_code ec;

		const std::string address = value_cast<std::string>(frame.at(0), "");
		const std::string service = value_cast<std::string>(frame.at(1), "");

#if (BOOST_ASIO_VERSION < 101200)
		//	< Boost 1.66
		boost::asio::ip::tcp::resolver resolver(s.get_io_service());
		boost::asio::ip::tcp::resolver::query query(address, service);
		boost::asio::connect(s, resolver.resolve(query), ec);

#elif (BOOST_ASIO_VERSION < 101400)

		//	< Boost 1.69
		boost::asio::ip::tcp::resolver resolver(s.get_executor().context());
		boost::asio::connect(s, resolver.resolve(address, service), ec);

#else
		//	> Boost 1.69
		//
		//	There is a problem that Boost.Asio in Boost 1.69.0 und 1.70.0 labelled with the same version
		//
		boost::asio::ip::tcp::resolver resolver(s.get_executor());
		boost::asio::connect(s, resolver.resolve(address, service), ec);

#endif
		BOOST_ASSERT(s.is_open());
		ctx.set_register(ec);
	}

	void ip_tcp_socket_available(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Determine the number of bytes available for reading.
		//
		boost::system::error_code ec;
		ctx.set_return_value(make_object(s.available(ec)), 0);
		ctx.set_register(ec);
	}

	void ip_tcp_socket_write(boost::asio::ip::tcp::socket& s, context& ctx)
	{
		//
		//	Send a buffer_t object synchronously
		//
		const vector_t frame = ctx.get_frame();
		boost::system::error_code ec;
		buffer_t buf;
		buf = value_cast(frame.at(0), buf);

		//
		//	returns the number of bytes written
		//
		ctx.set_return_value(make_object(boost::asio::write(s, boost::asio::buffer(buf.data(), buf.size()), ec)), 0);
		ctx.set_register(ec);
	}

	void register_socket(boost::asio::ip::tcp::socket& s, controller& vm)
	{
		vm.register_function("ip.tcp.socket.shutdown", 0, std::bind(&ip_tcp_socket_shutdown, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.close", 0, std::bind(&ip_tcp_socket_close, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.cancel", 0, std::bind(&ip_tcp_socket_cancel, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.is_open", 0, std::bind(&ip_tcp_socket_is_open, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.ep.local", 0, std::bind(&ip_tcp_socket_ep_local, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.ep.local.port", 0, std::bind(&ip_tcp_socket_ep_local_port, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.ep.remote", 0, std::bind(&ip_tcp_socket_ep_remote, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.ep.remote.port", 0, std::bind(&ip_tcp_socket_ep_local_port, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.connect", 1, std::bind(&ip_tcp_socket_connect, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.resolve", 2, std::bind(&ip_tcp_socket_resolve, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.available", 0, std::bind(&ip_tcp_socket_available, std::ref(s), std::placeholders::_1));
		vm.register_function("ip.tcp.socket.write", 1, std::bind(&ip_tcp_socket_write, std::ref(s), std::placeholders::_1));
	}
}
