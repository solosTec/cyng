/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_CONTEXT_H
#define CYNG_VM_CONTEXT_H

#include <cyng/intrinsics/sets.h>
#include <functional>
#include <cyng/intrinsics/traits.hpp>
#include <cyng/intrinsics/traits/tag.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/system/error_code.hpp>

namespace cyng 
{
	class vm;
	class memory;
	class librarian;
	class context
	{
		friend class librarian;

	public:
		context() = delete;
		context(context const&) = delete;
		context(context&&) = delete;
		context(vm&, memory&, std::string);
		context(context&, memory&);

		/**
		 * @remove if true the call frame will removed from the stack
		 * @return all elements of the call frame.
		 */
		vector_t get_frame() const;
		
		/**
		 * The frame size is the difference
		 * of the actual stack size and the current
		 * base pointer.
		 * 
		 * @return frame size
		 */
		std::size_t frame_size() const noexcept;
		
		/**
		 * Set the return value(s)
		 * @param idx return value index. Starts with 0.
		 * @see SETR
		 */
		void set_return_value(object const&, std::size_t idx);
		
		/**
		 * Set return value.
		 * Take the top element of stack and store it in the element before bp.
		 * Compare with ASP
		 */
		void set_return_value();
		
		/**
		 * Pushes the given element value to the top of the stack.
		 */
		void push(object&&);
		void push(object const&);

		/**
		 * Set the VM "last result" register 
		 */
		void set_register(boost::system::error_code);
		void set_register(boost::system::errc::errc_t);
		
		/**
		 * reset error register to "no error"
		 */
		void clear_register();
		
		/**
		 * @return VM tag
		 */
		boost::uuids::uuid tag() const noexcept;
		
		/**
		 * Execute instructions immediately on a new
		 * frame on the stack.
		 */
		void run(vector_t&& prg);

		/**
		 * Append instructions to the running program
		 * and executes it.
		 */
		context& queue(vector_t&& prg);

		/**
		 * return function name
		 */
		std::string const& get_name() const;

	private:
		vm& vm_;
		memory& mem_;
		const std::string name_;
	};

	/**
	 * Define a datatype VM call 
	 */
	using vm_call = std::function<void(context&)>;
	
	namespace traits
	{
		template <>
		struct type_tag<cyng::vm_call>
		{
			using type = cyng::vm_call;
			using tag =  std::integral_constant<std::size_t, PREDEF_VM_CALL>;
#if defined(CYNG_LEGACY_MODE_ON)
			const static char name[];
#else
			constexpr static char name[] = "vm-call";
#endif
		};
		
		template <>
		struct reverse_type < PREDEF_VM_CALL >
		{
			using type = cyng::vm_call;
		};		
	}
}

namespace std
{
	/**
	 * helper function to obtain target address of function
	 * @see: https://stackoverflow.com/questions/20833453/comparing-stdfunctions-for-equality
	 */
	template<typename T, typename... U>
	size_t get_target_address(function<T(U...)> f) {
		typedef T(fnType)(U...);
		fnType ** fnPointer = f.template target<fnType*>();
		return (size_t) *fnPointer;
	}
	
	template<> 
	struct hash<cyng::vm_call>
	{
		size_t operator()(cyng::vm_call const& c) const noexcept;
	};
	
	template<> 
	struct equal_to<cyng::vm_call>
	{
		//	pre C++17
		using result_type = bool;
		using first_argument_type = cyng::vm_call;
		using second_argument_type = cyng::vm_call;
		
		bool operator()(first_argument_type const& c1, second_argument_type const& c2) const noexcept;
	};	

	template<>
	struct less<cyng::vm_call>
	{
		//	pre C++17
		using result_type = bool;
		using first_argument_type = cyng::vm_call;
		using second_argument_type = cyng::vm_call;

		bool operator()(first_argument_type const& c1, second_argument_type const& c2) const noexcept;
	};

}

#endif	//	CYNG_VM_CONTEXT_H





