#include <cyng/vm/vm.h>
#include <cyng/vm/mesh.h>
#include <cyng/vm/proxy.h>

#include <cyng/io/ostream.h>

#ifdef _DEBUG_VM
#include <iostream>
#endif


namespace cyng {

	vm_base::vm_base(channel_weak wp, mesh& fab)
		: vm_base(wp, fab, boost::uuids::nil_uuid())	//	delegate
	{}

	vm_base::vm_base(channel_weak wp, mesh& fab, boost::uuids::uuid tag)
	: channel_(wp)
		, mesh_(fab)
		, ctx_()
		, parent_(tag)
	{}


	void vm_base::stop(eod) {
		ctx_.tidy();
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
		case op::IDENT:
			ident();
			break;
		case op::INVOKE:	
			//	call function over channel
			invoke();
			break;
		case op::INVOKE_R:	
			//	call function over channel with return value
			invoke_r();
			break;
		case op::FORWARD:
			forward();
			break;
		case op::RESOLVE:
			resolve();
			break;

			//	assembly
		case op::MAKE_ATTR:		
			ctx_.make_attr();
			break;
		case op::MAKE_PARAM:
			ctx_.make_param();
			break;
		case op::MAKE_PROP:
			ctx_.make_prop();
			break;
		case op::MAKE_ATTR_MAP:
			ctx_.make_attr_map();
			break;
		case op::MAKE_PARAM_MAP:
			ctx_.make_param_map();
			break;
		case op::MAKE_PROP_MAP:
			ctx_.make_prop_map();
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
		case op::MAKE_PRG:
			ctx_.make_prg();
			break;

		case op::SPLIT:
			ctx_.split();
			break;

		case op::ASSERT_TYPE:
			ctx_.assert_type();
			break;
		case op::ASSERT_VALUE:
			ctx_.assert_value();
			break;


		case op::ESBA:
			ctx_.esba();
			break;
		case op::REBA:
			ctx_.reba();
			break;
		case op::PULL:
			ctx_.pull();
			break;
		case op::FRM:
			ctx_.frm();
			break;

		case op::HALT:
			break;

		default:
			BOOST_ASSERT_MSG(false, "unknown op code");
			break;
		}

	}

	boost::uuids::uuid vm_base::get_channel_tag() const {
		auto sp = channel_.lock();
		return (sp)
			? get_tag(sp)
			: boost::uuids::nil_uuid()
			;
	}

	void vm_base::ident() {
		ctx_.push(cyng::make_object(get_channel_tag()));
	}

	void vm_base::forward() {
		//
		//	get target UUID
		// 
		auto const tag = ctx_.forward();
		auto [slot, msg] = ctx_.invoke();
		if (get_channel_tag() == tag) {
			//
			//	Forward to same VM.
			//	No lookup required.
			//
			//std::cerr << "***warning: forward to same vm " << tag << std::endl;
			auto channel = channel_.lock();
			if (channel)	channel->dispatch(slot, std::move(msg));
		}
		else {
			//
			//	forward to other VM
			//
			mesh_.lookup(tag, [=, this](std::vector<channel_ptr> channels) mutable {
				BOOST_ASSERT(channels.size() < 2);
				if (!channels.empty()) {
					channels.front()->dispatch(slot, std::move(msg));
				}
#ifdef _DEBUG
				else {
					std::cerr << "***warning: channel " << tag << " not found" << std::endl;
				}
#endif
			});
		}
	}

	void vm_base::resolve() {
		auto const name = ctx_.pop_value(std::string());
		auto sp = channel_.lock();
		if (sp) {
			ctx_.push(cyng::make_object(sp->lookup(name)));
		}
	}


}
