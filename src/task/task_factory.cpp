#include <cyng/task/task_factory.hpp>


namespace docscript {

	task_factory::task_factory(boost::asio::io_context& io)
		: id_(0)
		, registry_(io)
    {}

}
