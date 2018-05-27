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
			const vector_t frame = ctx.get_frame();
			
			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			boost::filesystem::create_directories(p, ec);
 			ctx.set_register(ec);

		});

		//
		//	current_path(system::error_code& ec);
		//	Get the current path and push value onto stack.
		//	Set error register.
		//
		vm.register_function("fs.current.path", 0, [](context& ctx) {

			boost::system::error_code ec;
			const boost::filesystem::path p = boost::filesystem::current_path(ec);
 			ctx.push(make_object(p));
			ctx.set_register(ec);
		});
		
		//
		//	exists(const path& p, system::error_code& ec) noexcept;
		//	Set error register.
		//
		vm.register_function("fs.exists", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const bool b = boost::filesystem::exists(p, ec);
 			ctx.push(make_object(b));
 			ctx.set_register(ec);

		});
		
		//
		//	file_size(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.size", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const auto size = boost::filesystem::file_size(p, ec);
 			ctx.push(make_object<std::uint64_t>(size));
 			ctx.set_register(ec);

		});
		
		//
		//	is_directory(const path& p, system::error_code& ec) noexcept;
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.is.directory", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const bool b = boost::filesystem::is_directory(p, ec);
 			ctx.push(make_object(b));
 			ctx.set_register(ec);

		});
		
		//
		//	is_empty(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.is.empty", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const bool b = boost::filesystem::is_empty(p, ec);
 			ctx.push(make_object(b));
 			ctx.set_register(ec);

		});
		
		//
		//	is_regular_file(const path& p, system::error_code& ec) noexcept;
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.is.regular", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const bool b = boost::filesystem::is_regular_file(p, ec);
 			ctx.push(make_object(b));
 			ctx.set_register(ec);

		});
		
		//
		//	last_write_time(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.last.write-time", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const auto tt = boost::filesystem::last_write_time(p, ec);
 			ctx.push(make_object(std::chrono::system_clock::from_time_t(tt)));
 			ctx.set_register(ec);

		});
	
		//
		//	remove(const path& p, system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.remove", 1, [](context& ctx) {

			const vector_t frame = ctx.get_frame();

			const boost::filesystem::path p = value_cast(frame.at(0), boost::filesystem::path());
			boost::system::error_code ec;
			const bool b = boost::filesystem::remove(p, ec);
 			ctx.push(make_object(b));
 			ctx.set_register(ec);

		});
		
		//
		//	temp_directory_path(system::error_code& ec);
		//	Push result onto stack.
		//	Set error register.
		//
		vm.register_function("fs.temp.dir", 0, [](context& ctx) {

			boost::system::error_code ec;
			const boost::filesystem::path p = boost::filesystem::temp_directory_path(ec);
 			ctx.push(make_object(p));
 			ctx.set_register(ec);

		});
	}
}







