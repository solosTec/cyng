/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/tracker.h>
#include <cyng/obj/core/object_interface.h>

#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#include <cyng/obj/type_interface.h>

#endif

namespace docscript {

	tracker::tracker()
	{}

	void tracker::operator()(obj_interface* p) const
	{
#ifdef _DEBUG_OBJECT
		std::cout << "delete #" << counter_ << " <" << p->rtti().type_name() << ">" << std::endl;
#endif

		delete p;
		--counter_;
	}

	std::size_t	tracker::get_counter() const
	{
		return counter_;
	}

	void tracker::inc()
	{
		++counter_;
	}

	std::atomic<std::size_t>	tracker::counter_{0};

}

