#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/vm/vm.h>
#include <cyng/vm/memory.h>
#include <cyng/vm/stack.h>
#include <cyng/vm/mesh.h>
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
	docscript::memory mem;
	BOOST_TEST(!mem);
}

BOOST_AUTO_TEST_CASE(stack)
{
}


BOOST_AUTO_TEST_CASE(vm)
{
	docscript::controller ctl;
	docscript::mesh fabric(ctl);
	auto vm = fabric.create_proxy();
	//	ctx_.load(obj)
	vm.load(make_object(docscript::op::TIDY));
	vm.run();

	vm.execute(
		docscript::op::NOW,
		docscript::op::PID,
		docscript::op::TID,
		docscript::make_object<std::size_t>(3),
		docscript::op::MAKE_TUPLE,
		docscript::TC_TUPLE,
		docscript::op::ASSERT_TYPE
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
	using signatures_t = docscript::tmp::tuple_cat_t<
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

	//docscript::tuple_cat_t<
	//	tpl1,
	//	tpl2
	//> test1{ 2, 3.0f, 4 };

	demo d;

	//	--------------------------------------------------------------+

	docscript::controller ctl;
	docscript::mesh fabric(ctl);

	session s;
	//std::function<void(int)> f = std::bind(&session::foo, &s, std::placeholders::_1);
	std::function<void(int)> f = [&s](int i) {
		s.foo(i);
	};

	//auto vm = fabric.create_proxy(f);
	auto vm = fabric.create_proxy(std::move(f));
	vm.load(make_object(docscript::op::TIDY));

	std::this_thread::sleep_for(std::chrono::seconds(4));
	vm.stop();
	ctl.stop();

}

BOOST_AUTO_TEST_CASE(mesh)
{
	docscript::controller ctl;
	docscript::mesh fabric(ctl);
	auto vm = fabric.create_proxy();
	// std::cerr << vm->get_name() << std::endl;

	//
	//	vm::demo()
	//
	//vm->dispatch(3, docscript::make_tuple(1));
	{
		//
		//	ctx_.load(obj);
		//
		vm.load(docscript::make_object(2));
	}
	{
		//
		//	ctx_.load(deq); 
		//
		auto deq = docscript::make_deque( 1, 2, 3, 4 );
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

BOOST_AUTO_TEST_SUITE_END()