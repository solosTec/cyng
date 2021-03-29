#include <cyng/vm/stack.h>
#include <cyng/obj/factory.hpp>

#include <algorithm>

#include <boost/uuid/nil_generator.hpp>>

namespace cyng {

	stack::stack()
		: s_()
		, bp_{ 0 }	//	base pointer
		//, fp_{ 0 }	//	frame pointer
	{}

	void stack::push(object obj) noexcept
	{
		s_.push_back(obj);
	}

	void stack::pop()
	{
		BOOST_ASSERT(!s_.empty());
		s_.pop_back();
	}

	object& stack::top()
	{
		BOOST_ASSERT(!s_.empty());
		return s_.back();
	}

	object&& stack::remove()
	{
		auto &&obj = std::move(s_.back());
		top();
		return std::move(obj);
	}

	void stack::esba()
	{
		bp_ = s_.size();
		push(make_object(bp_));
	}

	void stack::reba()
	{
		auto const bp = bp_;
		bp_ = saved_bp();
		BOOST_ASSERT(s_.size() > bp_);

		//	Restore old stack size:
		s_.resize(bp_);	//	pop_back
	}

	void stack::make_attr()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (attr)");
		auto const idx = top_value<std::size_t>();
		pop();
		push(make_object(attr_t(idx, remove())));
	}

	/** @brief build a parameter
	 *
	 */
	void stack::make_param()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (param)");
		auto const key = top_value<std::string>();
		pop();
		push(make_object(param_t(key, remove())));
	}

	/** @brief build an attribute map
	 *
	 */
	void stack::make_attr_map()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (attr_map)");
		auto size = top_value<std::size_t>();
		BOOST_ASSERT_MSG(size < s_.size(), "not enough parameters (attr_map)");
		pop();
		attr_map_t amap;
		while (size-- != 0) {
			amap.insert(top_value<attr_t>());
			pop();
		}
		push(make_object(std::move(amap)));
	}


	/** @brief build a parameter map
	 *
	 */
	void stack::make_param_map()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (param_map)");
		auto size = top_value<std::size_t>();
		BOOST_ASSERT_MSG(size < s_.size(), "not enough parameters (param_map)");
		pop();
		param_map_t pmap;
		while (size-- != 0) {
			pmap.insert(top_value<param_t>());
			pop();
		}
		push(make_object(std::move(pmap)));
	}

	/** @brief build a tuple (std::list<object>)
	 *
	 */
	void stack::make_tuple()
	{
		BOOST_ASSERT_MSG(!s_.empty(), "not enough parameters (tuple)");
		auto size = top_value<std::size_t>();
		BOOST_ASSERT_MSG(size < s_.size(), "not enough parameters (tuple)");
		pop();
		tuple_t tpl;
		while (size-- != 0) {
			tpl.push_back(std::move(top()));
			pop();
		}
		push(make_object(std::move(tpl)));
	}

	/** @brief build a vector (std::vector<object>)
	 *
	 */
	void stack::make_vector()
	{
		BOOST_ASSERT_MSG(!s_.empty(), "not enough parameters (vector)");
		auto size = top_value<std::size_t>();
		BOOST_ASSERT_MSG(size < s_.size(), "not enough parameters (vector)");
		pop();
		vector_t vec;
		while (size-- != 0) {
			vec.push_back(std::move(top()));
			pop();
		}
		push(make_object(std::move(vec)));
	}

	/** @brief build a deque (std::deque<object>)
	 *
	 */
	void stack::make_deque()
	{
		BOOST_ASSERT_MSG(!s_.empty(), "not enough parameters (deque)");
		auto size = top_value<std::size_t>();
		BOOST_ASSERT_MSG(size < s_.size(), "not enough parameters (deque)");
		pop();
		deque_t deq;
		while (size-- != 0) {
			deq.push_back(std::move(top()));
			pop();
		}
		push(make_object(std::move(deq)));
	}

	std::size_t stack::saved_bp() const
	{
		BOOST_ASSERT(bp_ < s_.size());
		return value_cast<std::size_t>(s_.at(bp_), 0u);
	}

	void stack::swap()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (swap)");
		auto last = s_.rbegin();
		auto before_last = std::next(last);
		std::iter_swap(last, before_last);
	}

	void stack::assert_type() {
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (assert_type)");
		auto tag = top_value<type_code>();
		pop();
		BOOST_ASSERT_MSG(s_.back().rtti().tag() == tag, "ASSERT_TYPE");
	}

	void stack::assert_value() {
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (assert_value)");
		auto obj = std::move(s_.back());
		pop();
		BOOST_ASSERT_MSG(obj.hash() == s_.back().hash(), "ASSERT_VALUE");
	}

	std::tuple<std::string, cyng::tuple_t> stack::invoke() {
		//
		//	stack :
		// * channel/function name
		// * parameter count
		// * parameters ...
		//
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (invoke)");
		auto const name = top_value<std::string>();
		pop();
		make_tuple();
		auto tpl = top_value<tuple_t>();
		pop();
		std::reverse(tpl.begin(), tpl.end());
		return { name, tpl };
	}

	boost::uuids::uuid stack::forward() {
		auto const tag = top_value(boost::uuids::nil_uuid());
		pop();
		return tag;
	}

	std::tuple<std::size_t, cyng::tuple_t> stack::invoke_r() {
		//
		//	stack :
		// * channel/function id
		// * parameter count
		// * parameters ...
		//
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (invoke)");
		auto const id = top_value<std::size_t>();
		pop();
		make_tuple();
		auto tpl = top_value<tuple_t>();
		pop();
		std::reverse(tpl.begin(), tpl.end());
		return { id, tpl };

	}
}
