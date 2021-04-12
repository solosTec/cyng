/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/vm/proxy.h>
#include <cyng/obj/container_factory.hpp>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng {

	boost::uuids::uuid get_tag(channel_ptr cp) {
		boost::uuids::string_generator gen;
		try {
			return gen(cp->get_name());
		}
		catch(std::runtime_error const&) {}
		return boost::uuids::nil_uuid();
	}

	vm_proxy::vm_proxy()
		: vm_()
	{}

	vm_proxy::vm_proxy(channel_ptr ptr)
		: vm_(ptr)
	{
		BOOST_ASSERT(ptr);
	}

	vm_proxy& vm_proxy::operator=(channel_ptr channel) {
		vm_ = channel;
		return *this;
	}

	void vm_proxy::run() {
		//	slot 2
		//	run()
		vm_->dispatch(2, make_tuple());
	}

	void vm_proxy::load(object&& obj) {
		//	slot 0
		//	ctx_.load(obj)
		auto const execute = is_same_type<op>(obj);
		vm_->dispatch(0, make_tuple(std::move(obj)));
		if (execute)	run();
	}

	void vm_proxy::load(deque_t&& deq) {
		//	slot 1
		//	ctx_.load(deq)
		vm_->dispatch(1, cyng::make_tuple(std::move(deq)));
	}

	void vm_proxy::stop() {
		vm_->stop();
	}

	void vm_proxy::set_channel_name(std::string name, std::size_t idx) {
		//	slot 3
		//	set_channel_name(name, index)
		vm_->dispatch(3, cyng::make_tuple(name, idx));
	}

	boost::uuids::uuid vm_proxy::get_tag() const {
		return cyng::get_tag(vm_);
	}

	boost::asio::io_context::strand& expose_dispatcher(vm_proxy& pr) {
		BOOST_ASSERT(pr.vm_);
		return expose_dispatcher(*pr.vm_);
	}

}

