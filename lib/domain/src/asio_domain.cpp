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

	void register_socket(boost::asio::ip::tcp::socket& s, controller& vm)
	{
		vm.register_function("ip.tcp.socket.shutdown", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			s.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			ctx.set_register(ec);
		});

		//
		//	Any asynchronous send, receive or connect operations will be cancelled immediately, 
		//	and will complete with the boost::asio::error::operation_aborted error.
		//
		vm.register_function("ip.tcp.socket.close", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			s.close(ec);
			ctx.set_register(ec);
		});

		//
		//	This function causes all outstanding asynchronous connect, send and receive operations 
		//	to finish immediately, and the handlers for cancelled operations will be passed 
		//	the boost::asio::error::operation_aborted error.
		//
		vm.register_function("ip.tcp.socket.cancel", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			s.cancel(ec);
			ctx.set_register(ec);
		});

		//
		//	This function causes all outstanding asynchronous connect, send and receive operations 
		//	to finish immediately, and the handlers for cancelled operations will be passed 
		//	the boost::asio::error::operation_aborted error.
		//
		vm.register_function("ip.tcp.socket.isOpen", 0, [&s](context& ctx) {
			ctx.set_return_value(make_object(s.is_open()), 0);
		});

		//
		//	Get the local endpoint of the socket.
		//
		vm.register_function("ip.tcp.socket.ep.local", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			ctx.push(make_object(s.local_endpoint(ec)));
			ctx.set_register(ec);
		});

		//
		//	Get the local endpoint port of the socket.
		//
		vm.register_function("ip.tcp.socket.ep.local.port", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			ctx.push(make_object(s.local_endpoint(ec).port()));
			ctx.set_register(ec);
		});

		//
		//	Get the remote endpoint of the socket.
		//
		vm.register_function("ip.tcp.socket.ep.remote", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			ctx.push(make_object(s.remote_endpoint(ec)));
			ctx.set_register(ec);
		});

		//
		//	Get the remote endpoint of the socket.
		//
		vm.register_function("ip.tcp.socket.ep.remote.port", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			ctx.push(make_object(s.remote_endpoint(ec).port()));
			ctx.set_register(ec);
		});

		//
		//	Connect the socket to the specified endpoint.
		//
		vm.register_function("ip.tcp.socket.connect", 1, [&s](context& ctx) {
			const vector_t frame = ctx.get_frame();
			boost::system::error_code ec;
			s.connect(value_cast(frame.at(0), boost::asio::ip::tcp::endpoint()), ec);
			BOOST_ASSERT(s.is_open());
			ctx.set_register(ec);
		});

		//
		//	Connect the socket to the specified endpoint using the resolver
		//
		vm.register_function("ip.tcp.socket.resolve", 2, [&s](context& ctx) {
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
		});

		//
		//	Determine the number of bytes available for reading.
		//
		vm.register_function("ip.tcp.socket.available", 0, [&s](context& ctx) {
			boost::system::error_code ec;
			ctx.set_return_value(make_object(s.available(ec)), 0);
			ctx.set_register(ec);
		});

		//
		//	Send a buffer_t object synchronously
		//
		vm.register_function("ip.tcp.socket.write", 1, [&s](context& ctx) {
			const vector_t frame = ctx.get_frame();
			boost::system::error_code ec;
			buffer_t buf;
			buf = value_cast(frame.at(0), buf);

			//
			//	returns the number of bytes written
			//
			ctx.set_return_value(make_object(boost::asio::write(s, boost::asio::buffer(buf.data(), buf.size()), ec)), 0);
			ctx.set_register(ec);
		});
	}
}
