/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_LOG_DOMAIN_H
#define CYNG_VM_LOG_DOMAIN_H

#include <cyng/vm/controller.h>
#include <cyng/log.h>
#include <cyng/vm/generator.h>

namespace cyng 
{
	
	void register_logger(logging::log_ptr, controller& vm);
	
}

#endif	//	CYNG_VM_LOG_DOMAIN_H



