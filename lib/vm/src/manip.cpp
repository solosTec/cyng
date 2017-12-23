/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/manip.h>

namespace cyng 
{
	namespace detail 
	{
		stream_policy<vector_t>::stream_policy(vector_t& vec)
		: vec_(vec)
		{}
			
		vector_t& stream_policy<vector_t>::operator()(vector_t&& v)
		{
			vec_.insert(vec_.end(), v.begin(), v.end());
			return vec_;
		}
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
	
	
}






