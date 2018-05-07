/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_STACK_H
#define CYNG_VM_STACK_H


#include <cyng/object.h>
#include <cyng/intrinsics/sets.h>
#include <stack>
#include <deque>

namespace cyng 
{
	namespace vt
	{
		//	define the stack base type
 		typedef std::deque< object >	container_t;
		typedef std::stack< object, container_t >	stack_t;
	}
	
	class context;	
	
	/**
	 * Only inheritance grant access to the underlying template 
	 * member c.
	 * @see C++ standard 23.6.5.2 stack definition
	 */
	class stack
	: public vt::stack_t
	{
		friend class context;
		
	public:
		/**
		 * Default constructor
		 */
		stack();
		
		/**
		 * Establish base pointer
		 */
		void ebp();
		
		/**
		 * restore base pointer and optional previous stack size
		 */
		void rbp();
		
		/**
		 * The frame size is the difference
		 * of the actual stack size and the current
		 * base pointer.
		 * 
		 * @return frame size
		 */
		std::size_t frame_size() const noexcept;
		
		/**
		 * @remove if true the call frame will removed from the stack
		 * @return all elements of the call frame.
		 */
		vector_t get_frame() const;
		
		/**
		 * Set return value.
		 * Compare with ASP
		 */
		void setr(object const&, std::size_t idx);
		
		/**
		 * Set return value.
		 * Take the top element of stack and store it in the element before bp.
		 * Compare with ASP
		 */
		void setr();

		/**
		 * dump all elements from data stack 
		 * to an output stream.
		 */
		void dump(std::ostream& os) const;
		
		void assemble_attr();
		void assemble_param();
		void assemble_attr_map();
		void assemble_param_map();
		void assemble_tuple();
		void assemble_vector();
		void assemble_set();

		/**
		 * swap the two values on top of the stack
		 */
		void swap();

	private:
		/**
		 * @return last stored bp on stack.
		 */
		std::size_t saved_bp() const;
		
		/**
		 * @return true if n is any of the stored base pointer values.
		 */
		bool is_bp(std::size_t n) const;
		
		
	private:
		/**
		 * (Saved) base pointer.
		 * Also known as frame pointer.
		 */
		std::size_t	bp_;
		
	};
	
	/**
	 * Save stack pointer when entering the activation class 
	 * and restore frame pointer when leaving.
	 */
	class activation
	{
	public:
		activation(stack&);
		virtual ~activation();

	private:
		stack& stack_;
	};
	
}

#endif	//	CYNG_VM_STACK_H



