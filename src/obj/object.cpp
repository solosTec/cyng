
#include <cyng/obj/object.h>
#include <cyng/obj/core/wrapper.hpp>
#include <cyng/obj/intrinsics/null.h>

#include <iterator>
#include <cyng/io/ostream.h>

namespace cyng
{
	object::object()
		: obj_(new wrapper<null>{ null{} }, tracker_)
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "object()" << std::endl;
#endif
	}

	object::object(object&& obj) noexcept
		: obj_(std::move(obj.obj_))
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "object(object&& obj)" << std::endl;
#endif
	}

#ifdef _DEBUG_OBJECT
	object::object(object const& obj)
		: obj_()
	{
		if(!obj) {
			std::cerr << "object(object const& obj)*null" << std::endl;
			// std::unique_ptr<wrapper_t, tracker>(new wrapper_t(make_vector<T>(vec)), object::tracker_)
			using wrapper_t = wrapper<null>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			// auto up = ptr_t(new wrapper_t( null{} ), tracker_);
			obj_ = ptr_t(new wrapper_t( null{} ), tracker_);
		}
		else {
			std::cerr << "object(object const& obj)*clone" << std::endl;
			obj_ = ptr_t(obj.obj_->clone(), tracker_);
		}	
	}
#else
	object::object(object const& obj)
		// : obj_((!obj) ? obj.obj_->clone() : new wrapper<null>{ null{} }, tracker_)
		: obj_((!obj) ? new wrapper<null>{ null{} } : obj.obj_->clone(), tracker_)
	{}
#endif

	object::object(obj_interface* ip)
		: obj_(ip, tracker_)
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "object(obj_interface* ip)" << std::endl;
#endif
	}

	object::object(ptr_t ptr)
		: obj_(std::move(ptr))
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "object(ptr_t ptr)" << std::endl;
#endif
	}

	object::~object()
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "~object()" << std::endl;
#endif

	}

	std::size_t object::hash() const noexcept
	{
		return (!*this)
			? 0
			: obj_->hash()
			;
	}

	object object::clone() const
	{
		return (!*this)
			? object()
			: obj_->clone();
	}

	type_interface const& object::rtti() const
	{
		return obj_->rtti();
	}

	object& object::operator=(object&& r) noexcept
	{
		//	move
		obj_.reset(r.obj_.release());
		return *this;
	}

	object& object::operator=(object const& obj)
	{
		if (obj)	obj_.reset(obj.obj_->clone());
		return *this;
	}

	object::operator bool() const noexcept {
		return obj_.operator bool();
	}


	//
	//	initialize static member
	//
	tracker object::tracker_{};


	void swap(object& o1, object& o2)
	{
		o1.obj_.swap(o2.obj_);
	}

	bool is_same_type(object const& a, object const& b) {
		return a.rtti().rtti() == b.rtti().rtti();
	}

	bool operator==(object const& o1, object const& o2) {
		return o1.hash() == o2.hash();
	}

	bool operator!=(object const& o1, object const& o2) {
		return !(o1 == o2);
	}


} // namespace cyng

namespace std {

	size_t hash<cyng::object>::operator()(cyng::object const& obj) const
	{
		return obj.hash();
	}
}
