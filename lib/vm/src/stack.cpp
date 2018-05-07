/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/stack.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>
#include <cyng/io/serializer.h>

#include <iomanip>
#include <boost/assert.hpp>

namespace cyng 
{

	stack::stack()
	: vt::stack_t()
	, bp_(0)
	{
		//ebp();
	}
	
	void stack::ebp()
	{
		//	Push the old base pointer onto the stack,
		//	such that it can be restored later
		vt::stack_t::push(make_object(bp_));
		BOOST_ASSERT_MSG(!vt::stack_t::empty(), "ebp failed" );
		
		//	bp_ points to the element one above the saved
		//	base pointer.
		bp_ = vt::stack_t::size();
	}

	void stack::rbp()
	{
		//	save base pointer
		const std::size_t bp = bp_;

		BOOST_ASSERT_MSG(!vt::stack_t::empty(), "stack is empty" );
		BOOST_ASSERT_MSG(bp != 0, "invalid bp (rbp)");

		//	Pops the base pointer off the stack, so it is restored
		//	to its value before
		bp_ = saved_bp();
		BOOST_ASSERT_MSG(bp_ < bp, "invalid pase pointer (2)");

		//	Restore old stack size:
		//	This is the position of the saved BP less 1 for the position
		//	of the stored bp itself.
		c.resize(bp - 1);
	}
	
	std::size_t stack::frame_size() const noexcept
	{
		BOOST_ASSERT_MSG(bp_ <= size(), "invalid base pointer" );
		return (vt::stack_t::empty())
		? 0 
		: (vt::stack_t::size() - bp_)
		;
	}
	
	vector_t stack::get_frame() const
	{
		const std::size_t fsize = frame_size();
		BOOST_ASSERT_MSG(fsize <= vt::stack_t::size(), "frame size to big");
		return vector_t(c.end() - fsize, c.end());
	}
	
	std::size_t stack::saved_bp() const
	{
		BOOST_ASSERT_MSG(!vt::stack_t::empty(), "stack is empty" );
		BOOST_ASSERT_MSG(bp_ != 0, "invalid base pointer (1)");
		BOOST_ASSERT_MSG((bp_ - 1) < vt::stack_t::size(), "invalid base pointer (2)");
		//BOOST_ASSERT_MSG(bp_ < vt::stack_t::size(), "invalid base pointer");
		//std::cerr << c[bp_ - 1].get_class().type_name() << std::endl;
		BOOST_ASSERT_MSG(c[bp_ - 1].get_class().tag() == TC_UINT64, "not a base pointer");
		return value_cast<std::size_t>(this->vt::stack_t::c[bp_ -  1], 0);
	}
	
	void stack::dump(std::ostream& os) const
	{
		for (std::size_t idx = c.size(); idx != 0; idx--)
		{
			if ((idx - 1) == bp_)
			{
				os << "=> ";
			}
			else if (is_bp(idx - 1))
			{
				os << "-> ";
			}
			else 
			{
				os 
					<< std::setw(2)
					<< std::setfill(' ')
					<< (idx - 1)
					<< ">";
			}
			
			io::serialize_plain(os, c[idx - 1]);
			os << std::endl ;
		}
	}
	
	bool stack::is_bp(std::size_t pos) const
	{
		if (!empty() && bp_ < size())
// 		if (!empty() && bp_ < size() && type_code_test<types::CYNG_INDEX>(c[bp_]))
		{
			//	start with last saved base pointer
			std::size_t bp = stack::saved_bp();
			for (;;)
			{
				if (pos == bp)	return true;
				if (bp == 0)	break;

				BOOST_ASSERT_MSG(bp < size(), "invalid base pointer");
// 				BOOST_ASSERT_MSG(type_code_test< types::CYNG_INDEX >(c[bp]), "not a base pointer (data type index expected)");

				//
				//	get next bp
				//
 				bp = value_cast< std::size_t >(this->c[bp], 0u);
			}
		}
		return false;
	}
	
	void stack::setr(object const& obj, std::size_t idx)
	{
		BOOST_ASSERT_MSG(bp_ > 0, "bp out of range (setr)");
		c[(bp_ -  1) + idx] = obj;
	}
	
	void stack::setr()
	{
		BOOST_ASSERT_MSG(c.size() > bp_, "bp out of range (setr)");
		BOOST_ASSERT_MSG(bp_ != 0, "not a function call (setr)");
		setr(top(), 0);
	}
	
	void stack::assemble_attr()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (attr)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_UINT64, "size_t expected (attr)");
		const auto idx = value_cast<std::size_t>(top(), 0u);
		pop();
		push(set_factory(idx, top()));
		swap();
		pop();
	}

	void stack::assemble_param()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (param)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_STRING, "string expected (param)");
		const std::string key = value_cast<std::string>(top(), "");
		BOOST_ASSERT_MSG(!key.empty(), "key is empty (param)");
		pop();
		push(set_factory(key, top()));
		swap();
		pop();
	}

	void stack::assemble_attr_map()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (attr_map)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_UINT64, "size_t expected (attr_map)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		attr_map_t map;
		while (size != 0)
		{
			BOOST_ASSERT_MSG(top().get_class().tag() == TC_ATTR, "attr expected (attr_map)");
			auto ap = object_cast< attr_t >(top());
			BOOST_ASSERT_MSG(ap != nullptr, "wrong data type (attr_map)");
			if (ap != nullptr)
			{
				map[ap->first] = ap->second;
			}
			else
			{
				map[ap->first] = make_object();
			}
			pop();
			--size;
		}
		push(make_object(map));
	}

	void stack::assemble_param_map()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (param_map)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_UINT64, "size_t expected (param_map)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		param_map_t map;
		while (size != 0)
		{
			BOOST_ASSERT_MSG(top().get_class().tag() == TC_PARAM, "param expected (param_map)");
			auto pp = object_cast< param_t >(top());
			BOOST_ASSERT_MSG(pp != nullptr, "wrong data type (param_map)");
			if (pp != nullptr)
			{
				map[pp->first] = pp->second;
			}
			else
			{
				map[pp->first] = make_object();
			}
			pop();
			size--;
		}
		push(make_object(map));
	}

	void stack::assemble_tuple()
	{
		BOOST_ASSERT_MSG(!c.empty(), "not enough parameters (tuple)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_UINT64, "size_t expected (tuple)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();
		if (size > c.size())
		{
			throw std::runtime_error("not enough parameters (tuple)");
		}

		tuple_t tpl;
		while (size != 0)
		{
			tpl.push_back(top());
			pop();
			--size;
		}

		push(make_object(tpl));
	}

	void stack::assemble_vector()
	{
		BOOST_ASSERT_MSG(!c.empty(), "not enough parameters (vector)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_UINT64, "size_t expected (vector)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();
		if (size > c.size())
		{
			throw std::runtime_error("not enough parameters (vector)");
		}

		vector_t vec;
		while (size != 0)
		{
			vec.push_back(top());
			pop();
			--size;
		}

		push(make_object(vec));
	}

	void stack::assemble_set()
	{
		BOOST_ASSERT_MSG(!c.empty(), "not enough parameters (set)");
		BOOST_ASSERT_MSG(top().get_class().tag() == TC_UINT64, "size_t expected (set)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();
		//BOOST_ASSERT_MSG(size < c.size(), "not enough parameters (set)");
		if (size > c.size())
		{
			throw std::runtime_error("not enough parameters (set)");
		}

		set_t set;
		while (size != 0)
		{
			set.insert(top());
			pop();
			--size;
		}

		push(make_object(set));
	}

	void stack::swap()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "missing parameter (swap)");
		auto last = c.rbegin();
		auto before_last = std::next(last);
		std::iter_swap(last, before_last);
	}


	activation::activation(stack& s)
		: stack_(s)
	{
		//	save stack pointer
		stack_.ebp();
	}

	activation::~activation()
	{
		//	restore stack pointer
		stack_.rbp();	//	remove elements from stack
	}
}






