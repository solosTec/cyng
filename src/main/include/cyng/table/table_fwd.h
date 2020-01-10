/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TABLE_FWD_H
#define CYNG_TABLE_FWD_H

#include <cyng/intrinsics/sets.h>
#include <cyng/intrinsics/lockable.h>

namespace cyng 
{
    namespace table
    {
        class record;
        using key_type = vector_t;
        using data_type = key_type;
        using body_type = lockable;
        using key_list_t = std::list<key_type>;	//	not unique
    }
}

#endif	//	CYNG_TABLE_FWD_H





