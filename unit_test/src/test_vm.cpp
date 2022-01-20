#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/vm/vm.h>
#include <cyng/vm/memory.h>
#include <cyng/vm/stack.h>
#include <cyng/vm/mesh.h>
#include <cyng/vm/linearize.hpp>
#include <cyng/task/controller.h>
#include <cyng/io/serialize.h>
//#include <cyng/obj/container_factory.hpp>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/util.hpp>

#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#endif

BOOST_AUTO_TEST_SUITE(vm_suite)

BOOST_AUTO_TEST_CASE(memory)
{
	//
	//	empty memory
	//
	cyng::memory mem;
	BOOST_TEST(!mem);
}

BOOST_AUTO_TEST_CASE(stack)
{
}


BOOST_AUTO_TEST_CASE(vm)
{
	cyng::controller ctl;
	cyng::mesh fabric(ctl);
	auto vm = fabric.create_proxy();
	//	ctx_.load(obj)

	vm.execute(
		cyng::op::NOW,
		cyng::op::PID,
		cyng::op::TID,
		cyng::make_object<std::size_t>(3),
		cyng::op::MAKE_TUPLE,
		cyng::TC_TUPLE,
		cyng::op::ASSERT_TYPE
	);

	vm.load(make_object(cyng::op::TIDY));
	vm.run();

	vm.execute(
		cyng::op::NOW,
		cyng::make_object("abc"),
		cyng::op::MAKE_PARAM,
		cyng::TC_PARAM,
		cyng::op::ASSERT_TYPE
	);

	vm.execute(
		cyng::op::NOW,
		cyng::make_object("now-1"),
		cyng::op::MAKE_PARAM,
		cyng::op::NOW,
		cyng::make_object("now-2"),
		cyng::op::MAKE_PARAM,
		cyng::make_object<std::size_t>(2),
		cyng::op::MAKE_PARAM_MAP,
		cyng::TC_PARAM_MAP,
		cyng::op::ASSERT_TYPE
	);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	vm.stop();
	ctl.shutdown();
	ctl.stop();

}

class session
{
public:
	session() {}
	inline std::uint64_t foo(std::string s) {
		return s.size();
	}
};

//	--------------------------------------------------------------+

BOOST_AUTO_TEST_CASE(library)
{
	session s;
	std::function<std::uint64_t(std::string)> f1 = [&s](std::string str) -> std::uint64_t {	//	ok
		//std::cout << str << std::endl;
		BOOST_REQUIRE_EQUAL(str, "a string");
		return s.foo(str);
	};

	std::function<std::uint64_t(cyng::vector_t)> f2 = [&](cyng::vector_t vec) -> std::uint64_t {
		BOOST_REQUIRE_EQUAL(vec.size(), 2);
		return vec.size();
	};

	//	--------------------------------------------------------------+

	cyng::controller ctl;
	cyng::mesh fabric(ctl);

	cyng::description d("f1", f1);
	auto vm2 = fabric.make_proxy(cyng::make_description("foo", f1)
		, cyng::make_description("bar", f2)
	);	//	ok

	vm2.execute(
		cyng::op::ESBA,
		cyng::make_object("a string"),
		cyng::make_object<std::size_t>(1),
		cyng::make_object("foo"),
		cyng::op::RESOLVE,
		cyng::op::INVOKE_R,
		cyng::op::PULL,

		cyng::op::ESBA,
		cyng::make_object("another string (2)"),
		cyng::make_object("and a secret"),
		cyng::make_object<std::size_t>(2),
		cyng::op::MAKE_VECTOR,
		cyng::make_object<std::size_t>(1),	//	make tuple
		cyng::make_object("bar"),
		cyng::op::RESOLVE,
		cyng::op::INVOKE_R,
		cyng::op::PULL

	);


	//	--------------------------------------------------------------+

	//std::function<void(int)> f = std::bind(&session::foo, &s, std::placeholders::_1);	//	ok

	auto vm = fabric.create_proxy(f1, f2);	//	ok
	//auto vm = fabric.create_proxy(std::move(f1));	//	ok

	//auto vm = fabric.create_proxy(std::bind(&session::foo, &s, std::placeholders::_1));	//	not ok

	vm.set_channel_name("foo", 0);
	vm.set_channel_name("bar", 1);

	vm.execute(
		cyng::op::ESBA,
		cyng::make_object("a string"),
		cyng::make_object<std::size_t>(1),
		cyng::make_object("foo"),
		cyng::op::RESOLVE,
		cyng::op::INVOKE_R,
		cyng::op::PULL,

		cyng::op::ESBA,
		cyng::make_object("another string"),
		cyng::make_object("and a secret"),
		cyng::make_object<std::size_t>(2),
		cyng::op::MAKE_VECTOR,
		cyng::make_object<std::size_t>(1),	//	make tuple
		cyng::make_object("bar"),
		cyng::op::RESOLVE,
		cyng::op::INVOKE_R,
		cyng::op::PULL

	);
	vm.load(make_object(cyng::op::TIDY));

	vm.run();

	std::this_thread::sleep_for(std::chrono::seconds(4));
	vm.stop();

	ctl.shutdown();
	ctl.stop();

}

BOOST_AUTO_TEST_CASE(mesh)
{
	cyng::controller ctl;
	cyng::mesh fabric(ctl);
	auto vm = fabric.create_proxy();
	// std::cerr << vm->get_name() << std::endl;

	//
	//	vm::demo()
	//
	//vm->dispatch(3, cyng::make_tuple(1));
	{
		//
		//	ctx_.load(obj);
		//
		vm.load(cyng::make_object(2));
	}
	{
		//
		//	ctx_.load(deq); 
		//
		auto deq = cyng::make_deque( 1, 2, 3, 4 );
#ifdef _DEBUG_OBJECT
		std::cerr << deq << std::endl;
#endif

		vm.load(std::move(deq));
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
	vm.stop();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	ctl.shutdown();
	ctl.stop();

}

BOOST_AUTO_TEST_CASE(linearize)
{
	auto deq_simple = cyng::linearize(1, 3.14758, std::string("boo"));
	BOOST_REQUIRE_EQUAL(deq_simple.size(), 3);
	BOOST_REQUIRE_EQUAL(deq_simple.at(0).size(), 4);
	BOOST_REQUIRE_EQUAL(deq_simple.at(1).size(), 8);
	BOOST_REQUIRE_EQUAL(deq_simple.at(2).size(), 3);

	auto deq_tlv = cyng::convert_to_deque(1, 3.14758, std::string("boo"));
	BOOST_REQUIRE_EQUAL(deq_tlv.size(), 3);
}

//	--------------------------------------------------------------+

class sig
{
	template <typename T> friend class cyng::task;


	using signatures_t = std::tuple<
		std::function<void(std::string)>,
		std::function<void(cyng::eod)>>;

	public:
		sig(cyng::channel_weak wp)
			: sigs_{ std::bind(&sig::hello, this, std::placeholders::_1), std::bind(&sig::stop, this, std::placeholders::_1) }
			, channel_(wp)
		{
			auto sp = channel_.lock();
			if (sp) {
				sp->set_channel_names({ "hello" });
			}
		}

	private:
		void hello(std::string s) {
			//std::cout << "hello" << std::endl;
			BOOST_CHECK_EQUAL(s, "abc");
		}
		void stop(cyng::eod) {
			//std::cout << "STOP" << std::endl;
		}
	private:
		signatures_t sigs_;
		cyng::channel_weak channel_;

};

BOOST_AUTO_TEST_CASE(signature)
{
	cyng::controller ctl;
	//cyng::mesh fabric(ctl);

	auto channel = ctl.create_named_channel_with_ref<sig>("sig");
	BOOST_ASSERT(channel->is_open());
	channel->dispatch("hello", "abc");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	channel->stop();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	ctl.shutdown();
	ctl.stop();
}

BOOST_AUTO_TEST_SUITE_END()