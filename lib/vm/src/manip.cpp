/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/manip.h>
#include <cyng/value_cast.hpp>
#include <iterator>

namespace cyng 
{
	namespace detail 
	{
		//stream_policy<vector_t>::stream_policy(vector_t& vec)
		//: vec_(vec)
		//{}
		//	
		//vector_t& stream_policy<vector_t>::operator()(vector_t&& v)
		//{
		//	vec_.insert(vec_.end(), v.begin(), v.end());
		//	return vec_;
		//}
	}

	invoke::invoke(std::string const& name)
	: name_(name)
	{}
	
	invoke::invoke(invoke const& other)
	: name_(other.name_)
	{}
		
	vector_t& operator<<(vector_t& vec, invoke&& m)
	{
		vec
			<< m.name_
			<< code::INVOKE
			;
		return vec;
	}
	
	defer::defer(code c)
	: code_(c)
	{}
	
	defer::defer(defer const& other)
	: code_(other.code_)
	{}
		
	vector_t& operator<<(vector_t& vec, defer&& m)
	{
		vec
			<< code::PUSH
			<< m.code_
			;
		return vec;
	}
		
	reflect::reflect(code c)
	: code_(c)
	{}
	
	reflect::reflect(reflect const& other)
	: code_(other.code_)
	{}
		
	vector_t& operator<<(vector_t& vec, reflect&& m)
	{
		vec
			<< defer(code::PUSH)
			<< defer(m.code_)
			;
		return vec;
	}

	invoke_remote::invoke_remote(std::string const& name)
	: name_(name)
	{}
	
	invoke_remote::invoke_remote(invoke_remote const& other)
	: name_(other.name_)
	{}
		
	vector_t& operator<<(vector_t& vec, invoke_remote&& m)
	{
		vec
			<< m.name_
			<< defer(code::INVOKE)
			;
		return vec;
	}
	
	invoke_reflect::invoke_reflect(std::string const& name)
	: name_(name)
	{}
	
	invoke_reflect::invoke_reflect(invoke_reflect const& other)
	: name_(other.name_)
	{}
	
	vector_t& operator<<(vector_t& vec, invoke_reflect&& m)
	{
		vec
			<< m.name_
			<< reflect(code::INVOKE)
			;
		return vec;
	}

	
	vector_t& operator<<(vector_t& vec, clear&&)
	{
		vec.clear();
		return vec;
	}
		
	vector_t& operator<<(vector_t& vec, size&&)
	{
		vec << vec.size();
		return vec;
	}
	
	
	call_frame::call_frame(vector_t& prg)
	: prg_(prg)
	{
		prg_ << code::ESBA;
	}
		
	call_frame::~call_frame()
	{
		prg_ << code::REBA;
	}
	
	vector_t& operator<<(vector_t& vec, reloc&&)
	{
		std::map<std::string, std::size_t>	tbl;

		//
		//	1. collect labels and remove them
		//
		for (auto pos = vec.begin(); pos != vec.end(); /* empty */)
		{
			if (pos->get_class().tag() == TC_LABEL)
			{
				label lbl;
				tbl[value_cast(*pos, lbl).name_] = std::distance(vec.begin(), pos);
				pos = vec.erase(pos);
			}
			else
			{
				++pos;
			}
		}

		//
		//	2. substitute labels by address
		//
		for (auto pos = vec.begin(); pos != vec.end(); ++pos)
		{
			if (pos->get_class().tag() == TC_CODE)
			{
				switch (value_cast(*pos, code::NOOP))
				{
				case code::JA:
				case code::JE:
				case code::JNE:
					BOOST_ASSERT_MSG(pos != vec.begin(), "cannot substitute label");
					if ((pos != vec.begin()) && ((pos - 1)->get_class().tag() == TC_STRING))
					{
						//
						//	get jump address
						//
						auto it = tbl.find(value_cast<std::string>(*(pos - 1), ""));
						BOOST_ASSERT_MSG(it != tbl.end(), "no jump address");
						if (it != tbl.end())
						{
							*(pos - 1) = make_object(it->second);
						}
					}
				default:
					break;
				}
			}
		}

		return vec;
	}

	unwind_vec::unwind_vec(std::size_t n)
		: n_(n)
	{}

	vector_t& operator<<(vector_t& vec, unwind_vec&& e)
	{
		vector_t res;
		res.reserve((e.n_ == 0) ? (vec.size() * 2) : e.n_);	//	empirical value
		for (auto& v : vec)
		{
			if (v.get_class().tag() == TC_VECTOR)
			{
				auto tmp = value_cast(v, res);
				tmp << unwind_vec(tmp.size());	//	recursion
				res.insert(res.end(), tmp.begin(), tmp.end());
			}
			else
			{
				res.push_back(v);
			}
		}
		std::swap(vec, res);
		return vec;
	}

}







