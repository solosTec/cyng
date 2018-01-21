/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_STORE_DOMAIN_H
#define CYNG_VM_STORE_DOMAIN_H

namespace cyng 
{
	namespace store
	{
		class db;
	}
	class context;
	void register_store(store::db& db, context& vm);
}

#endif	//	CYNG_VM_STORE_DOMAIN_H




