#include <cyng/vm/context.h>
#include <cyng/vm/vm.h>
#include <cyng/obj/factory.hpp>

#include <thread>

#include <boost/assert.hpp>
#include <boost/process/environment.hpp>

#ifdef _DEBUG_VM
#include <cyng/obj/vector_cast.hpp>
#include <cyng/io/ostream.h>
#include <iostream>
#endif

namespace cyng {

	context::context()
	{}

	void context::load(object&& obj)	
	{
#ifdef _DEBUG_VM
		std::cerr << "ctx::load " << obj << std::endl;
#endif
		mem_.push_back(obj);
	}

	void context::load(deque_t&& deq)
	{
#ifdef _DEBUG_VM
		std::cerr << "ctx::load(" << deq.size() << ")" << std::endl;
#endif
		mem_.insert(mem_.end(), std::begin(deq), std::end(deq));
		BOOST_ASSERT(mem_.size() >= deq.size());
	}

	void context::tidy() {

	}

	void context::pid() {
		push(make_object<std::uint64_t>(boost::this_process::get_id()));
	}

	void context::tid() {
		auto const id = std::this_thread::get_id();
		push(make_object<std::uint64_t>(std::hash<std::thread::id>()(id)));
	}

	void context::now() {
		push(make_object(std::chrono::system_clock::now()));
	}

}
