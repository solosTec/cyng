#include <boost/test/unit_test.hpp>
#include "demo_task.h"

namespace cyng {

    demo_task::demo_task()
        : sigs_{
              std::bind(&demo_task::demo0, this),
              std::bind(&demo_task::demo1, this, std::placeholders::_1),
              std::bind(&demo_task::demo2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
              std::bind(&demo_task::demo3, this, std::placeholders::_1),
              std::bind(&demo_task::demo4, this, std::placeholders::_1),
              std::bind(&demo_task::stop, this, std::placeholders::_1),
          } {}

    demo_task::~demo_task() {
        // std::cout << "demo_task(~)" << std::endl;
    }

    void demo_task::stop(eod) {
        // std::cout << "stop()" << std::endl;
    }

    int demo_task::demo0() {
        // std::cout << "demo0()" << std::endl;
        return 1;
    }

    void demo_task::demo1(int n) {
        // std::cout << "demo1(" << n << ")" << std::endl;
        BOOST_CHECK_EQUAL(n, 2);
    }

    void demo_task::demo2(int a, std::string b, float c) {
        // std::cout << "demo2(" << a << ", " << b << ", " << c << ")" << std::endl;
        BOOST_CHECK_EQUAL(a, 2);
        BOOST_CHECK_EQUAL(b, "dude");
        //	c == 3.14000010
        BOOST_CHECK_CLOSE(c, 3.14, 0.00001);
    }
    void demo_task::demo3(int n) {
        // std::cout << "demo3(" << n << ")" << std::endl;
        BOOST_CHECK_EQUAL(n, 24);
    }
    void demo_task::demo4(std::chrono::seconds s) {
        // std::cout << "demo4(" << s << ")" << std::endl;
        //  compare seconds itself failes with BOOST_CHECK_EQUAL()
        BOOST_CHECK_EQUAL(s.count(), std::chrono::seconds(12).count());
    }

} // namespace cyng
