#include <cyng/vm/mesh.h>
#include <cyng/task/controller.h>
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

}

