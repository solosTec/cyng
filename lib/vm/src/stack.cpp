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
		ebp();
	}
	
	void stack::ebp()
	{
		//	Push the old base pointer onto the stack,
		//	such that it can be restored later
		vt::stack_t::push(make_object(bp_));
		BOOST_ASSERT_MSG(!vt::stack_t::empty(), "ebp failed" );
		
		//	Assigns the value of the stack pointer into
		//	the base pointer.
		bp_ = vt::stack_t::size() - 1;
	}

	void stack::rbp()
	{
		//	save base pointer
		const std::size_t bp = bp_;

		BOOST_ASSERT_MSG(!vt::stack_t::empty(), "stack is empty" );
// 		BOOST_ASSERT_MSG(bp_ < size(), "invalid base pointer" );
// 		BOOST_ASSERT_MSG(type_code_test< types::CYY_INDEX >(c[bp]), "data type index expected" );
		
		//	Pops the base pointer off the stack, so it is restored
		//	to its value before
		bp_ = saved_bp();

		//	Restore old stack size
		while (bp != vt::stack_t::size())
		{
//			std::cout << " --pop(" << bp << "/" << size() << " " << to_literal(top()) << ")-- ";
			vt::stack_t::pop();
		}
	}
	
	std::size_t stack::frame_size() const noexcept
	{
		BOOST_ASSERT_MSG(bp_ <= size(), "invalid base pointer" );
		return (vt::stack_t::empty())
		? 0 
		: (vt::stack_t::size() - bp_ - 1)
		;
	}
	
	vector_t stack::get_frame(bool remove)
	{
		vector_t frame;
		/*const*/ std::size_t fsize = frame_size();
		frame.reserve(fsize);
		
		this->dump(std::cerr);
		
		for (std::size_t idx = 0; idx < fsize; idx++)
		{
// 			frame[idx] = c[vt::stack_t::size() - idx - 1];
			frame.push_back( c[bp_ + idx + 1] );	//	reverse
			
		}
		
		if (remove)
		{
			while (fsize-- != 0)
			{
				pop();
			}
		}
		return frame;
	}
	
	std::size_t stack::saved_bp() const
	{
		BOOST_ASSERT_MSG(!vt::stack_t::empty(), "stack is empty" );
		BOOST_ASSERT_MSG(bp_ < vt::stack_t::size(), "invalid base pointer");
// 		BOOST_ASSERT_MSG(type_code_test< types::CYY_INDEX >(c[bp_]), "not a base pointer (data type index expected)" );
		return value_cast<std::size_t>(this->vt::stack_t::c[bp_], 0);
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
// 		if (!empty() && bp_ < size() && type_code_test<types::CYY_INDEX>(c[bp_]))
		{
			//	start with last saved base pointer
			std::size_t bp = stack::saved_bp();
			for (;;)
			{
				if (pos == bp)	return true;
				if (bp == 0)	break;

				BOOST_ASSERT_MSG(bp < size(), "invalid base pointer");
// 				BOOST_ASSERT_MSG(type_code_test< types::CYY_INDEX >(c[bp]), "not a base pointer (data type index expected)");

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
		//BOOST_ASSERT_MSG(type_test<index>(top()), "wrong key type (attr)");
		const auto idx = value_cast<std::size_t>(top(), 0u);
		pop();
		push(set_factory(idx, top()));
		swap();
		pop();
	}

	void stack::assemble_param()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (param)");
		//BOOST_ASSERT_MSG(type_test<std::string>(top()), "wrong key type (param)");
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
		//BOOST_ASSERT_MSG(type_test<index>(top()), "wrong data type (attr_map)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		attr_map_t map;
		while (size-- != 0)
		{
			//boost::ignore_unused_variable_warning(p);
			//BOOST_ASSERT_MSG(primary_type_code_test<types::CYY_ATTRIBUTE>(top()), "wrong data type (attr_map)");
			auto ap = object_cast< attr_t >(top());
			BOOST_ASSERT_MSG(ap != nullptr, "wrong data type (attr_map)");
			map[ap->first] = ap->second;
			pop();
		}
		push(make_object(map));
	}

	void stack::assemble_param_map()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (param_map)");
		//BOOST_ASSERT_MSG(type_test<index>(top()), "wrong data type (param_map)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		param_map_t map;
		while (size-- != 0)
		{
			//BOOST_ASSERT_MSG(primary_type_code_test<types::CYY_PARAMETER>(top()), "wrong data type (param_map)");
			auto pp = object_cast< param_t >(top());
			BOOST_ASSERT_MSG(pp != nullptr, "wrong data type (param_map)");
			map[pp->first] = pp->second;
			pop();
		}
		push(make_object(map));
	}

	void stack::assemble_tuple()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (tuple)");
		//BOOST_ASSERT_MSG(type_test<index>(top()), "wrong data type (tuple)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		tuple_t tpl;
		while (size-- != 0)
		{
			tpl.push_back(top());
			pop();
		}

		push(make_object(tpl));
	}

	void stack::assemble_vector()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (vector)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		vector_t vec;
		while (size-- != 0)
		{
			vec.push_back(top());
			pop();
		}

		push(make_object(vec));
	}

	void stack::assemble_set()
	{
		BOOST_ASSERT_MSG(c.size() > 1, "not enough parameters (vector)");
		auto size = value_cast<std::size_t>(top(), 0u);
		pop();

		set_t set;
		while (size-- != 0)
		{
			set.insert(top());
			pop();
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
		stack_.rbp();
	}
	
}






