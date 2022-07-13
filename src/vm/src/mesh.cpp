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

	void mesh::lookup(boost::uuids::uuid tag, std::function<void(std::vector<channel_ptr>)> cb) {
		ctl_.get_registry().lookup(boost::uuids::to_string(tag), cb);
	}
}

