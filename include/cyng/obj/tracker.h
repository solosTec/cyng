/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_TRACKER_H
#define DOCC_OBJ_TRACKER_H

#include <cstddef>
#include <atomic>

namespace docscript {

	class obj_interface;
	class tracker
	{
		template<typename T >
		friend class wrapper;

	public:
		tracker();
		void operator()(obj_interface* p) const;
		std::size_t	get_counter() const;

	private:
		void inc();

	private:
		static std::atomic<std::size_t>	counter_;
		//mutable std::size_t	counter_;
	};
}

#endif //	DOCC_OBJ_TRACKER_H
