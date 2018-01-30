/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_MANIP_H
#define CYNG_VM_MANIP_H

#include <cyng/intrinsics/sets.h>
#include <cyng/intrinsics/op.h>
#include <cyng/factory.h>

namespace cyng 
{
	namespace detail 
	{
		template <typename T >
		struct stream_policy
		{
			vector_t& vec_;
			stream_policy(vector_t& vec)
			: vec_(vec)
			{}
			
			vector_t& operator()(T&& v)
			{
				vec_.push_back(make_object(std::forward<T>(v)));
				return vec_;
			}
			vector_t& operator()(T const& v)
			{
				vec_.push_back(make_object(v));
				return vec_;
			}
		};
		
		/**
		 * function objects will be executed
		 */
		template <typename R>
		struct stream_policy<std::function<R(void)> >
		{
			using f_t = std::function<R(void)>;
			vector_t& vec_;
			stream_policy(vector_t& vec)
				: vec_(vec)
			{}

			vector_t& operator()(f_t const& f)
			{
				vec_ << f();
				return vec_;
			}
			vector_t& operator()(f_t&& f)
			{
				vec_ << f();
				return vec_;
			}
		};

	}
	
	/**
	 * take an arbitray argument, create an object 
	 * and append it to the vector.
	 */
	template <typename T>
	vector_t& operator<<(vector_t& vec, T&& v)
	{
		using type = typename std::decay<T>::type;
		detail::stream_policy<type> sp(vec);
		return sp(std::forward<T>(v));
	}
	

	/**
	 * simple invoke call of a library function.
	 */
	class invoke 
	{
	public:
		invoke(std::string const&);
		invoke(invoke const&);
		
		friend vector_t& operator<<(vector_t&, invoke&&);
		
	private:
		const std::string name_;
	};
		
	 
	/**
	 * call invoke as stream operator
	 */
	vector_t& operator<<(vector_t&, invoke&&);
	
	/**
	 * Don't execute an op but push it on the stack
	 */
	class defer
	{
	public:
		defer(code);
		defer(defer const&);
		
		friend vector_t& operator<<(vector_t&, defer&&);
		
	private:
		const code code_;
	};
	
	/**
	 * call defer as stream operator
	 */
	vector_t& operator<<(vector_t&, defer&&);
	
	/**
	 * This is two repeated defer() operations. The resulting effect
	 * is usefull to transfer code to the remote session and back again
	 * to be executed (reflection).
	 */
	class reflect
	{
	public:
		reflect(code);
		reflect(reflect const&);
		
		friend vector_t& operator<<(vector_t&, reflect&&);
		
	private:
		const code code_;
	};
	
	/**
	 * call reflect as stream operator
	 */
	vector_t& operator<<(vector_t&, reflect&&);

	/**
	 * simple invoke call of a library function.
	 */
	class invoke_remote
	{
	public:
		invoke_remote(std::string const&);
		invoke_remote(invoke_remote const&);
		
		friend vector_t& operator<<(vector_t&, invoke_remote&&);
		
	private:
		const std::string name_;
	};

	/**
	 * call invoke_remote as stream operator
	 */
	vector_t& operator<<(vector_t&, invoke_remote&&);

	/**
	 * Add reflect operation to the INVOKE call.
	 */
	class invoke_reflect
	{
	public:
		invoke_reflect(std::string const&);
		invoke_reflect(invoke_reflect const&);

		friend vector_t& operator<<(vector_t&, invoke_reflect&&);

	private:
		const std::string name_;
	};
	
	/**
	 * call invoke_reflect as stream operator
	 */
	vector_t& operator<<(vector_t&, invoke_reflect&&);

	/**
	 * Clear program vector
	 */
	class clear
	{
	public:
		friend vector_t& operator<<(vector_t&, clear&&);
	};
	
	/**
	 * call clear() as stream operator
	 */
	vector_t& operator<<(vector_t&, clear&&);
	
	/**
	 * Push size of vector at end of program vector
	 */
	class size
	{
	public:
		friend vector_t& operator<<(vector_t&, size&&);
	};
	
	/**
	 * Push size of vector at end of program vector
	 */
	vector_t& operator<<(vector_t&, size&&);
	
	/**
	 * RAII construct to add an ESBA instruction at 
	 * the current position of the program and a REBA
	 * instruction at the end.
	 */
	class call_frame
	{
	public:
		call_frame(vector_t&);
		virtual ~call_frame();
		
	private:
		vector_t& prg_;
	};
	
	/**
	 * Search for labels in the program removes it and substitute 
	 * jump labels with the real address.
	 */
	class reloc
	{
	public:
		friend vector_t& operator<<(vector_t&, reloc&&);
	};
	
	/**
	 * Push size of vector at end of program vector
	 */
	vector_t& operator<<(vector_t&, reloc&&);

	/**
	 * unwind the specified container
	 */
	template <typename T>
	class unwind
	{
	public:
		unwind(T&& c)
		: container_(std::move(c))
		{}
		unwind(unwind&& other)
		: container_(std::move(other.container_))
		{}

		friend vector_t& operator<<(vector_t& vec, unwind&& e)
		{
			vec.insert(vec.end(), e.container_.begin(), e.container_.end());
			return vec;
		}

	private:
		T container_;
	};

	/**
	 * helper to auto detect container type
	 */
	template <typename T>
	unwind<T> unwinder(T&& c)
	{
		return unwind<T>(std::move(c));
	}


	/**
	 * post function.
	 * Scan vector for vectors and enoll them.
	 * Work recursively.
	 */
	class unwind_vec
	{
	public:
		unwind_vec(std::size_t = 0);
		friend vector_t& operator<<(vector_t& vec, unwind_vec&& e);
	private:
		std::size_t n_;
	};

	/**
	 * call unwind_vec as stream operator
	 */
	vector_t& operator<<(vector_t&, unwind_vec&&);

}

#endif	//	CYNG_VM_MANIP_H



