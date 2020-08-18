/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_GENERATOR_H
#define CYNG_VM_GENERATOR_H

#include <cyng/vm/context.h>
#include <cyng/vm/manip.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/meta/meta.hpp>

#include <initializer_list>

namespace cyng 
{
	/**
	 * Register a VM procedure.
	 * 
	 * Signature of vm_call is:
	 * @code
	 * void(context&)
	 * @endcode
	 * 
	 * @param name procedure name
	 * @param arity parameter count
	 * @param proc procedure to call 
	 */
	 vector_t register_function(std::string const& name 
		, std::size_t arity
		, vm_call proc);

	/**
	 * @see http://english.stackexchange.com/questions/25931/unregister-vs-deregister
	 */
	 vector_t deregister_function(std::string const& name);
	
	/**
	 * exchange both procedures. After that the previous procedure is on top of stack.
	 */
	 vector_t swap_functions(vm_call proc);

	/**
	 * Remove all registered procedures from library 
	 */
	 vector_t clear_library();

	namespace workbench
	{
		//
		//	anonymous namespace
		//

		template <typename T>
		void code_builder(vector_t& vec, T&& v)
		{
			vec << std::forward<T>(v);
		}

		template < typename T, typename ...Args >
		void code_builder(vector_t& vec, T&& v, Args&&... args)
		{
			vec << std::forward<T>(v);
			code_builder(vec, std::forward<Args>(args)...);
		}
		
		/**
		 *	fallback for empty sets
		 */
		inline void code_builder(vector_t& vec)
		{}

	}

	/**
	 *	Code generator accepts an arbitrary list of object types.
	 *
	 *	Example:
	 *	@code
	 vector_t vec = cyng::code_generator( "1", 2, std::chrono::seconds(3));
	 *	@endcode
	 */
	template < typename ...Args >
	vector_t code_generator(Args&&... args)
	{
		vector_t vec;
		workbench::code_builder(vec, std::forward<Args>(args)...);
		return vec;
	}

	namespace
	{
		//
		//	anonymous namespace
		//
	
		/**
		 * With C++17 this functor object can be substituted by
		 * a lambda function with [](auto&&... args){ ; }
		 */
		struct code_gen_functor
		{
			vector_t& code_;
			code_gen_functor(vector_t& code)
				: code_(code)
			{}

			template<typename ...Args>
			void operator() (Args&&... args)
			{
				code_builder(code_, std::forward<Args>(args)...);
			}
		};
	}

	template < typename ...Args >
	vector_t code_generator(std::tuple<Args... >&& arg)
	{
		vector_t vec;
		meta::apply(code_gen_functor(vec), arg);
		return vec;
	}

	/**
	 * generate a complete invoke sequence with a call frame 
	 */
	template < typename ...Args >
	vector_t generate_invoke(std::string const& name, Args&&... args)
	{
		vector_t vec;
 		vec << code::ESBA;
		
		workbench::code_builder(vec, std::forward<Args>(args)...);
		
 		vec 
		<< invoke(name)
  		<< code::REBA
 		;

		return vec;		
	}

	/**
	 * generate a complete invoke sequence with a call frame 
	 * as unwind manipulator
	 */
	template < typename ...Args >
	unwind<vector_t> generate_invoke_unwinded(std::string const& name, Args&&... args)
	{
		return unwind<vector_t>(generate_invoke(name, std::forward<Args>(args)...));
	}

	/**
	 * generate a complete invoke sequence with a remote call frame
	 */
	template < typename ...Args >
	vector_t generate_invoke_remote(std::string const& name, Args&&... args)
	{
		vector_t vec;
 		vec << defer(code::ESBA);

		//
		//	special code builder required to defer all included VM ops.
		//
		workbench::code_builder(vec, std::forward<Args>(args)...);

		vec
			<< invoke_remote(name)
 			<< defer(code::REBA)
			;

		return vec;
	}
	
	/**
	 * generate a complete invoke sequence with a remote call frame
	 * as unwind manipulator
	 */
	template < typename ...Args >
	unwind<vector_t> generate_invoke_remote_unwinded(std::string const& name, Args&&... args)
	{
		return unwind<vector_t>(generate_invoke_remote(name, std::forward<Args>(args)...));
	}

	/**
	 * generate a complete invoke sequence that will bounced back
	 */
	template < typename ...Args >
	vector_t generate_invoke_reflect(std::string const& name, Args&&... args)
	{
		vector_t vec;
 		vec << reflect(code::ESBA);

		workbench::code_builder(vec, std::forward<Args>(args)...);

		vec
			<< invoke_reflect(name)
 			<< reflect(code::REBA)
			;

		return vec;
	}

	/**
	* generate a complete invoke sequence that will bounced back
	* as unwind manipulator
	*/
	template < typename ...Args >
	unwind<vector_t> generate_invoke_reflect_unwinded(std::string const& name, Args&&... args)
	{
		return unwind<vector_t>(generate_invoke_reflect(name, std::forward<Args>(args)...));
	}

	/**
	 * Generate a code sequence to build a tuple (heterogeneous data types)
	 */
	template < typename ...Args >
	vector_t tuple_generator(std::tuple<Args... >&& arg)
	{
		vector_t vec;
		workbench::code_builder(vec, std::forward<Args>(args)...);
		vec
			<< static_cast<std::uint64_t>(std::tuple_size<Args>::value)
			<< code::ASSEMBLE_TUPLE
			;
		return vec;
	}

	template < typename ...Args >
	unwind<vector_t> tuple_generator_unwinded(Args&&... args)
	{
		return unwind<vector_t>(tuple_generator(std::forward<Args>(args)...));
	}

	/**
	 * Generate a code sequence to build a vector (uniform data types)
	 */
	template < typename T >
	vector_t vector_generator(std::initializer_list<T> ini)
	{
		vector_t vec;

		for (auto const& val : ini) {
			vec << val;
		}

		vec
			<< static_cast<std::uint64_t>(ini.size())
			<< code::ASSEMBLE_VECTOR
			;

		return vec;
	}

	template < typename T >
	unwind<vector_t> vector_generator_unwinded(std::initializer_list<T> ini)
	{
		return unwind<vector_t>(vector_generator(ini));
	}

}

#endif	//	CYNG_VM_CONTCYNG_VM_GENERATOR_HEXT_H






