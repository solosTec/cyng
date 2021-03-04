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
	vm.load(make_object(cyng::op::TIDY));
	vm.run();

	vm.execute(
		cyng::op::NOW,
		cyng::op::PID,
		cyng::op::TID,
		cyng::make_object<std::size_t>(3),
		cyng::op::MAKE_TUPLE,
		cyng::TC_TUPLE,
		cyng::op::ASSERT_TYPE
	);

	std::this_thread::sleep_for(std::chrono::seconds(20));
	vm.stop();
	ctl.stop();

}

class session
{
public:
	session() {}
	inline void foo(int) {}
};

//	--------------------------------------------------------------+
using tpl1 = std::tuple<int, float>;
//using tpl2 = std::tuple<int>;
using tpl2 = std::tuple < std::function<void(int)>>;

class demo
{
	using signatures_t = cyng::tmp::tuple_cat_t<
		tpl1,
		tpl2
	>;

public:
	demo() 
		: sig_()
	{}
	inline void foo(int) {}

	signatures_t sig_;
};
//	--------------------------------------------------------------+


BOOST_AUTO_TEST_CASE(library)
{
	//	--------------------------------------------------------------+
	//using tpl1 = std::tuple<int, float>;
	//using tpl2 = std::tuple<int>;

	//cyng::tuple_cat_t<
	//	tpl1,
	//	tpl2
	//> test1{ 2, 3.0f, 4 };

	demo d;

	//	--------------------------------------------------------------+

	cyng::controller ctl;
	cyng::mesh fabric(ctl);

	session s;
	//std::function<void(int)> f = std::bind(&session::foo, &s, std::placeholders::_1);	//	ok
	std::function<void(int)> f = [&s](int i) {	//	ok
		s.foo(i);
	};

	auto vm = fabric.create_proxy(f);	//	ok
	//auto vm = fabric.create_proxy(std::move(f));	//	ok

	//auto vm = fabric.create_proxy(std::bind(&session::foo, &s, std::placeholders::_1));	//	not ok

	vm.load(make_object(cyng::op::TIDY));

	std::this_thread::sleep_for(std::chrono::seconds(4));
	vm.stop();
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
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ctl.stop();

}

BOOST_AUTO_TEST_CASE(linearize)
{
	auto deq_simple = cyng::linearize(1, 3.14758, std::string("boo"));

	auto deq_tlv = cyng::convert_to_deque(1, 3.14758, std::string("boo"));
}

BOOST_AUTO_TEST_SUITE_END()