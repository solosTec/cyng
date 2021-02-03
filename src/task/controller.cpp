#include <cyng/task/controller.h>
#include <boost/algorithm/string.hpp>


namespace cyng {

    controller::controller()
        : scheduler()
        , registry_(this->get_ctx())
        , id_(0)
    {}
    controller::controller(std::size_t num_threads)
        : scheduler(num_threads)
        , registry_(this->get_ctx())
        , id_(0)
    {}
 
    registry& controller::get_registry()
    {
        return registry_;
    }

    std::string cleanup_task_name(std::string const& name)
    {
        //	class node::xxxxxxx
        std::vector<std::string> parts;
        boost::split(parts, name, boost::is_any_of("\t :"), boost::token_compress_on);

        return parts.empty()
            ? name
            : parts.back()
            ;
    }

}
