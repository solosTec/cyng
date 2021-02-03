#include <cyng/vm/vm.h>
#include <cyng/vm/mesh.h>

#include <cyng/io/ostream.h>

#ifdef _DEBUG_VM
#include <iostream>
#endif


namespace cyng {

	vm_base::vm_base(mesh& fab)
		: vm_base(fab, boost::uuids::nil_uuid())	//	delegate
	{}

	vm_base::vm_base(mesh& fab, boost::uuids::uuid tag)
	: mesh_(fab)
		, ctx_()
		, parent_(tag)
	{}


	void vm_base::stop(eod) {
#ifdef _DEBUG_VM
		std::cerr << "vm::stop()" << std::endl;
#endif
	}

	void vm_base::run() {
#ifdef _DEBUG_VM
		std::cerr << "vm::run(" << ctx_.op_count() << ")" << std::endl;
#endif
		while (ctx_) {
			exec(ctx_());
			ctx_.step();
		}
	}

	void vm_base::exec(object const& obj) {
#ifdef _DEBUG_VM
		std::cerr << "vm::exec(" << obj << ")" << std::endl;
#endif
		if (is_same_type<op>(obj)) {
			exec(value_cast(obj, op::NOOP));
		}
		else {
			ctx_.push(obj);
		}
	}

	void vm_base::exec(op code) {
#ifdef _DEBUG_VM
		std::cerr << "vm::exec(" << code << ")" << std::endl;
#endif
		switch (code) {
		case op::NOOP:	break;
		case op::TIDY:	
			ctx_.tidy();
			break;
		case op::NOW:	//	push current timestamp on stack
			ctx_.now();
			break;
		case op::PID:
			ctx_.pid();
			break;
		case op::TID:	//	thread id
			ctx_.tid();
			break;

			//	assembly
		case op::MAKE_ATTR:		
			ctx_.make_attr();
			break;
		case op::MAKE_PARAM:
			ctx_.make_param();
			break;
		case op::MAKE_ATTR_MAP:
			ctx_.make_attr_map();
			break;
		case op::MAKE_PARAM_MAP:
			ctx_.make_param_map();
			break;
		case op::MAKE_TUPLE:
			ctx_.make_tuple();
			break;
		case op::MAKE_VECTOR:
			ctx_.make_vector();
			break;
		case op::MAKE_DEQUE:
			ctx_.make_deque();
			break;

		case op::ASSERT_TYPE:
			ctx_.assert_type();
			break;
		case op::ASSERT_VALUE:
			ctx_.assert_value();
			break;

		default:
			BOOST_ASSERT_MSG(false, "unknown op code");
			break;
		}

	}

}
