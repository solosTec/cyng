/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_TEST_DEMO_TASK_H
#define CYNG_TEST_DEMO_TASK_H

#include <cyng/obj/intrinsics/eod.h>

#include <functional>
#include <string>
#include <tuple>
#include <chrono>

namespace cyng {

    class demo_task {
        template <typename T> friend class task;

        using signatures_t = std::tuple<
            std::function<int(void)>,
            std::function<void(int)>,
            std::function<void(int, std::string, float)>,
            std::function<void(int)>,
            std::function<void(std::chrono::seconds)>,
            std::function<void(eod)>>;

      public:
        demo_task();
        ~demo_task();

        int demo0();
        void demo1(int);
        void demo2(int, std::string, float);
        void demo3(int);
        void demo4(std::chrono::seconds);

        void stop(eod);

      private:
        signatures_t sigs_;
    };
} // namespace cyng

#endif
