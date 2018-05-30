
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-vm-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/vm/manip.h>
#include <cyng/vm/controller.h>
#include <cyng/vm/generator.h>
#include <boost/uuid/random_generator.hpp>
#include <cyng/vm/domain/log_domain.h>
#include <cyng/vm/domain/fs_domain.h>
#include <cyng/async/scheduler.h>
#include <cyng/io/serializer.h>

namespace cyng 
{

	bool test_vm_001()
	{
		vector_t prg = generate_invoke("lib.test", 1, std::chrono::seconds(2), "hello");
		
		std::cout << prg.size() << std::endl;
		io::serialize_plain(std::cout, make_object(prg));
		
		async::scheduler ios;
// 		logging::console_logger_t clog(ios.get_io_service());
		controller ctrl(ios.get_io_service(), boost::uuids::random_generator()());
		ctrl.async_run(std::move(prg));
		
		std::this_thread::sleep_for(std::chrono::seconds(2));
		
		//
		//	testing the logger domain
		//
		auto clog = logging::make_console_logger(ios.get_io_service(), "VM");
// 		logging::console_logger_t clog(ios.get_io_service());
 		register_logger(clog, ctrl);
		
// 		"log.msg.info"
		ctrl.async_run(generate_invoke("log.msg.info", "log domain is running"));
		ctrl.async_run(generate_invoke("log.msg.trace", 42));
		std::this_thread::sleep_for(std::chrono::seconds(2));
		
		//
		//	testing the fs domain
		//
		register_fs(ctrl);

		BOOST_CHECK(prg.empty());
		prg.clear(); 

		prg 
		<< generate_invoke("log.msg.info", invoke("fs.current.path"))
		;
		
// 		[fs.current.path,op:INVOKE,log.msg.info,op:INVOKE]
// 		std::cout << prg.size() << std::endl;
// 		io::serialize_plain(std::cout, make_object(prg));
		
		ctrl.async_run(std::move(prg));
		std::this_thread::sleep_for(std::chrono::seconds(2));
		
		//
		//	test inserting functions
		//

		//	_Func	std::string <lambda>(void){...}	cyng::test_vm_001::__l2::std::string <lambda>(void)

		std::function<std::string(void)> f = []()->std::string { return "hello"; };

		prg
			<< f
			//<< []()->std::string { return " world"; }
			;
		io::serialize_plain(std::cout, make_object(prg));

		//
		//	halt VM, stop execution engine
		//
		//ctrl.halt();
		ctrl.access([](vm& vm) {
			vm.run(vector_t{ make_object(code::HALT) });
		});

		ios.stop();
		
		return true;
	}
	
}

