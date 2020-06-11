/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/domain/fs_domain.h>
#include <cyng/value_cast.hpp>
#include <cyng/vm/controller.h>
#include <cyng/vm/generator.h>
#include <cyng/factory.h>
#include <cyng/compatibility/file_system.hpp>
#include <chrono>
#include <boost/core/ignore_unused.hpp>

namespace cyng 
{
	void register_fs(controller& vm)
	{
		//
		//	create_directories(const path& p, system::error_code& ec)
		//	Set error register.
		//
		vm.register_function("fs.create.directories", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			const filesystem::path p = value_cast(frame.at(0), filesystem::path());

#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			filesystem::create_directories(p, ec);
#else
			boost::system::error_code ec;
			filesystem::create_directories(p, ec);
			ctx.set_register(ec);
#endif

		});

		//
		//	current_path(system::error_code& ec);
		//	Get the current path and push value onto stack.
		//	Set error register.
		//
		vm.register_function("fs.current.path", 0, [](context& ctx) {

#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			const filesystem::path p = filesystem::current_path(ec);
			ctx.push(make_object(p));
#else
			boost::system::error_code ec;
			const filesystem::path p = filesystem::current_path(ec);
 			ctx.push(make_object(p));
			ctx.set_register(ec);
#endif
		});
		
		//
		//	exists(const path& p, system::error_code& ec) noexcept;
		//	Set error register.
		//
		vm.register_function("fs.exists", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			auto const p = value_cast(frame.at(0), filesystem::path());
#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			const bool b = filesystem::exists(p, ec);
#else
			boost::system::error_code ec;
			const bool b = filesystem::exists(p, ec);
 			ctx.set_register(ec);
#endif
			ctx.push(make_object(b));

		});
		
		//
		//	file_size(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.size", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			auto const p = value_cast(frame.at(0), filesystem::path());
#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			auto const size = filesystem::file_size(p, ec);
#else
			boost::system::error_code ec;
			auto const size = filesystem::file_size(p, ec);
			ctx.set_register(ec);
#endif
			ctx.push(make_object<std::uint64_t>(size));

		});
		
		//
		//	is_directory(const path& p, system::error_code& ec) noexcept;
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.is.directory", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			auto const p = value_cast(frame.at(0), filesystem::path());
#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			const bool b = filesystem::is_directory(p, ec);
#else
			boost::system::error_code ec;
			const bool b = filesystem::is_directory(p, ec);
 			ctx.set_register(ec);
#endif
			ctx.push(make_object(b));

		});
		
		//
		//	is_empty(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.is.empty", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			auto const p = value_cast(frame.at(0), filesystem::path());
#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			const bool b = filesystem::is_empty(p, ec);
#else
			boost::system::error_code ec;
			const bool b = filesystem::is_empty(p, ec);
 			ctx.set_register(ec);
#endif
			ctx.push(make_object(b));

		});
		
		//
		//	is_regular_file(const path& p, system::error_code& ec) noexcept;
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.is.regular", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			auto const p = value_cast(frame.at(0), filesystem::path());
#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			const bool b = filesystem::is_regular_file(p, ec);
#else
			boost::system::error_code ec;
			const bool b = filesystem::is_regular_file(p, ec);
 			ctx.set_register(ec);
#endif
			ctx.push(make_object(b));

		});
		
		//
		//	last_write_time(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
//		vm.register_function("fs.last.write-time", 1, [](context& ctx) {
//
//			auto const frame = ctx.get_frame();
//
//			auto const p = value_cast(frame.at(0), filesystem::path());
//#if defined(__CPP_SUPPORT_P0218R1)
//			std::error_code ec;
//			const auto ftime = filesystem::last_write_time(p, ec);
//			std::time_t tt = decltype(ftime)::clock::to_time_t(ftime);
//			ctx.push(make_object(std::chrono::system_clock::from_time_t(tt)));
//#else
//			boost::system::error_code ec;
//			const auto tt = filesystem::last_write_time(p, ec);
// 			ctx.set_register(ec);
//			ctx.push(make_object(std::chrono::system_clock::from_time_t(tt)));
//#endif
//
//		});
	
		//
		//	remove(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.remove", 1, [](context& ctx) {

			auto const frame = ctx.get_frame();

			auto const p = value_cast(frame.at(0), filesystem::path());
#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			const bool b = filesystem::remove(p, ec);
#else 
			boost::system::error_code ec;
			const bool b = filesystem::remove(p, ec);
 			ctx.set_register(ec);
#endif
			ctx.push(make_object(b));
		});
		
		//
		//	temp_directory_path(system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.temp.dir", 0, [](context& ctx) {

#if defined(__CPP_SUPPORT_P0218R1)
			std::error_code ec;
			auto const p = filesystem::temp_directory_path(ec);
#else
			boost::system::error_code ec;
			auto const p = filesystem::temp_directory_path(ec);
			ctx.set_register(ec);
#endif
			ctx.push(make_object(p));

		});
	}
}







