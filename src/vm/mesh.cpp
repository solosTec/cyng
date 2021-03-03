#include <cyng/vm/mesh.h>
//#include <cyng/task/controller.h>
#include <cyng/vm/vm.h>
#include <cyng/obj/factory.hpp>

#include <boost/uuid/string_generator.hpp>


namespace cyng {

	mesh::mesh(controller& ctl) noexcept
		: ctl_(ctl)
		, uuid_rgn_()
	{}

	controller& mesh::get_ctl() {
		return ctl_;
	}

	channel_ptr mesh::lookup(boost::uuids::uuid tag) {
		auto res = ctl_.get_registry().lookup(boost::uuids::to_string(tag));
		return res.empty()
			? channel_ptr()
			: res.front()
			;
	}

	boost::uuids::uuid get_tag(channel_ptr cp) {
		boost::uuids::string_generator gen;
		try {
			return gen(cp->get_name());
		}
		catch(std::runtime_error const&) {
		}
		return boost::uuids::nil_uuid();
	}

	vm_proxy::vm_proxy(channel_ptr ptr)
		: vm_(ptr)
	{
		BOOST_ASSERT(ptr);
	}

	void vm_proxy::run() {
		//	slot 2
		vm_->dispatch(2, make_tuple());
	}

	void vm_proxy::load(object&& obj) {
		//	slot 0
		//	ctx_.load(obj)
		vm_->dispatch(0, make_tuple(std::move(obj)));

	}

	void vm_proxy::load(deque_t&& deq) {
		//	slot 1
		vm_->dispatch(1, cyng::make_tuple(std::move(deq)));
	}

	void vm_proxy::stop() {
		vm_->stop();
	}

	boost::uuids::uuid vm_proxy::get_tag() const {
		return cyng::get_tag(vm_);
	}

}

