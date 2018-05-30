/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_LIBRARIAN_H
#define CYNG_VM_LIBRARIAN_H

#include <cyng/vm/procedure.h>
#include <unordered_map>

namespace cyng 
{
	class controller;
	class librarian
	{
 		friend class controller;

	public:
		librarian();
		librarian(librarian const&) = delete;
		librarian(librarian&&) = delete;
		
		/**
		 * Serach the specified procedure (by name), check
		 * arity and call procedure.
		 *
		 * @param out_of_band don't use an activtion counter.
		 * @param name procedure name
		 * @param ctx execution context
		 * @return true if function was found and arity check was passed
		 */
		bool invoke(std::string const& name, context& ctx) const;
		
		/**
		 * Remove all stored procedures
		 */
		void clear();
		
		/**
		 * Remove procedure qith the specified name from library
		 */
		bool erase(std::string const&);
		
		/**
		 * Test if function "log.msg.error" is registered. 
		 *
		 * @return true if logger is available
		 */
		bool try_error_log(context& ctx, std::string) const;
		bool try_debug_log(vm& v, std::string) const;
		bool try_halt(vm& v) const;

	private:
		/**
		 * expose internal functions as VM functions 
		 */
		void register_this();
		
		/**
		 * The VM is able to call this procedure after
		 * inserting into the library.
		 * 
		 * @param name procedure name
		 * @param arity parameter count
		 * @param proc procedure to call
		 */
		bool insert(std::string const& name, std::size_t arity, vm_call proc);
		bool insert(std::string const&, procedure&&);
	
	private:
		std::unordered_map<std::string, procedure>	db_;
		
	};

}

#endif	//	CYNG_VM_LIBRARIAN_H




