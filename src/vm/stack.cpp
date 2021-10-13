#include <cyng/vm/stack.h>

#include <algorithm>
#include <iterator>

#include <boost/uuid/nil_generator.hpp>
#ifdef _DEBUG
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#endif

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

	object stack::remove()
	{
		auto obj = std::move(s_.back());
		pop();
		return std::move(obj);
	}

	void stack::esba()
	{
		bp_ = s_.size();
		push(make_object(bp_));
	}

	void stack::reba()
	{
		bp_ = saved_bp();
		BOOST_ASSERT(s_.size() > bp_);

		//	Restore old stack size:
		s_.resize(bp_);	//	pop_back
	}

	void stack::pull() {

		auto const bp = bp_;
		bp_ = saved_bp();
		BOOST_ASSERT(s_.size() > bp_);

		auto const pos = s_.begin() + bp;
		s_.erase(pos);
	}

	void stack::make_attr()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (attr)");
		auto const idx = pop_value<std::size_t>();
		push(make_object(attr_t(idx, remove())));
	}

	/** @brief build a parameter
	 *
	 */
	void stack::make_param()
	{
		BOOST_ASSERT_MSG(s_.size() > 1, "not enough parameters (param)");
		auto const key = pop_value<std::string>();
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
			amap.insert(pop_value<attr_t>());
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
			pmap.insert(pop_value<param_t>());
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

	void stack::frm() {
		BOOST_ASSERT_MSG(s_.size() > bp_, "invalid frame");
		auto const size = s_.size() - bp_;
		push(make_object(size - 1ul));
	}

	void stack::split()
	{
		BOOST_ASSERT_MSG(!s_.empty(), "not enough parameters (split)");
		switch (top().rtti().tag()) {
		case TC_TUPLE:
			disassemble<traits::reverse_type<TC_TUPLE>::type>();
			break;
		case TC_VECTOR:
			disassemble<traits::reverse_type<TC_VECTOR>::type>();
			break;
		case TC_DEQUE:
			disassemble<traits::reverse_type<TC_DEQUE>::type>();
			break;

		//case TC_ATTR_MAP:
		//case TC_ATTR:
		//case TC_PARAM_MAP:
		//case TC_PARAM:
		default:
			BOOST_ASSERT_MSG(false, "not a container class (explode)");
			break;
		}
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
		auto const tag = pop_value<type_code>();
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
		auto const name = pop_value<std::string>();
		make_tuple();
		auto tpl = pop_value<tuple_t>();
		std::reverse(tpl.begin(), tpl.end());
		return { name, tpl };
	}

	boost::uuids::uuid stack::forward() {
		auto const tag = top_value(boost::uuids::nil_uuid());
		BOOST_ASSERT_MSG(!tag.is_nil(), "forward tag is nil");
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
