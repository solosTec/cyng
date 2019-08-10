/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_SIGNAL_HANDLER_H
#define CYNG_SIGNAL_HANDLER_H

#include <memory>
#include <boost/assert.hpp>
#include <boost/predef.h>

namespace cyng
{

	struct signal_handler;
	
	/**
	 * A signal handler can only be loaded once. 
	 * 
	 * Usage:
	 * @code
	 * signal_mgr signal;
	 * signal.wait();
	 * @endcode
	 * @see http://solostec.de/anchor/index.php/posts/a-unique-singleton
	 */
	struct signal_mgr 
	{
		signal_mgr();
		virtual ~signal_mgr();
		
		int wait();

		static std::unique_ptr< signal_handler >    impl_;
	};
	
	/**
	 * In some cases we have to overcome the problem of the missing
	 * definition of struct signal_handler. 
	 */
#if BOOST_OS_WINDOWS
	void forward_signal(unsigned long);
#else
	void forward_signal(int);
#endif
    
    /**
     * write a backtrace dump
     */
    void backtrace();
}

#endif	//	CYNG_SIGNAL_HANDLER_H
