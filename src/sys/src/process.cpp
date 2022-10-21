#include <cyng/sys/process.h>
#include <boost/process/environment.hpp>

namespace cyng {
	namespace sys
	{
		pid get_process_id() {
			return pid(boost::this_process::get_id());
		}
	}
}
