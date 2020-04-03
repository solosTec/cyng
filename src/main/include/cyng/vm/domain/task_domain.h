/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_TASK_DOMAIN_H
#define CYNG_VM_TASK_DOMAIN_H

 //#include <cyng/async/mux.h>

namespace cyng
{
    namespace async {
        class mux;
    }
    class controller;
    void register_task(async::mux& s, controller& vm);
}

#endif	//	CYNG_VM_TASK_DOMAIN_H
