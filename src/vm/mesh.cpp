#include <cyng/vm/mesh.h>
//#include <cyng/task/controller.h>
#include <cyng/vm/vm.h>
#include <cyng/obj/factory.hpp>

#include <boost/uuid/string_generator.hpp>


namespace docscript {

	mesh::mesh(controller& ctl) noexcept
		: ctl_(ctl)
		, uuid_rgn_()
	{}

	//channel_ptr mesh::create_vm() {
	//	return create_vm<std::tuple<>>(boost::uuids::nil_uuid());
	//}

	//channel_ptr mesh::create_vm(boost::uuids::uuid parent) {

	//	//
	//	//	create a uuid
	//	//
	//	auto const tag = uuid_rgn_();

	//	//
	//	//	create task channel
	//	//
	//	return ctl_.create_named_channel<vm<std::tuple<>>>(boost::uuids::to_string(tag), *this, parent);

	//}

	//vm_proxy mesh::create_proxy() {
	//	return create_vm();
	//}
	//vm_proxy mesh::create_proxy(boost::uuids::uuid parent) {
	//	return create_vm<std::tuple<>>(parent);
	//}

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
		vm_->dispatch(1, docscript::make_tuple(std::move(deq)));
	}

	void vm_proxy::stop() {
		vm_->stop();
	}

	boost::uuids::uuid vm_proxy::get_tag() const {
		return docscript::get_tag(vm_);
	}

}

