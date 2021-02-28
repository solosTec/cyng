/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_PUB_H
#define CYNG_STORE_PUB_H

#include <cyng/store/key.hpp>
#include <cyng/store/slot.h>

#include <vector>
#include <forward_list>

#include <boost/uuid/uuid.hpp>

namespace cyng {

	class table;
	class pub
	{
	//public:
	//	using insert_signal = std::function<void(table const*
	//		, key_t const&
	//		, data_t const&
	//		, std::uint64_t
	//		, boost::uuids::uuid)>;
	//	using remove_signal = std::function<void(table const*
	//		, key_t const&
	//		, boost::uuids::uuid)>;
	//	using clear_signal = std::function<void(table const*, boost::uuids::uuid)>;
	//	using modify_signal = std::function<void(table const*
	//		, key_t const&
	//		, attr_t const&
	//		, std::uint64_t
	//		, boost::uuids::uuid)>;

	public:
		pub();

		void connect(slot);
		void connect_insert(slot);
		void connect_modify(slot);
		void connect_remove(slot);
		void connect_clear(slot);

		void disconnect(slot);

	protected:
		/**
		 * signal an insert event
		 */
		void forward(table const*
			, key_t const&
			, data_t const&
			, std::uint64_t
			, boost::uuids::uuid);

		/**
		 * signal an modify event
		 */
		void forward(table const*
			, key_t const&
			, attr_t const&
			, std::uint64_t
			, boost::uuids::uuid);

		/**
		 * signal an remove event
		 */
		void forward(table const*
			, key_t const&
			, boost::uuids::uuid);

		/**
		 * signal an clear event
		 */
		void forward(table const*
			, boost::uuids::uuid);

	private:
		std::vector<slot>	slots_insert_;
		std::vector<slot>	slots_modify_;
		std::vector<slot>	slots_remove_;
		std::vector<slot>	slots_clear_;
	};

	/**
	 * remove a slot from the container
	 */
	bool disconnect(std::vector<slot>&, slot);

	/**
	 * remove a list of slots from the container
	 */
	void tidy(std::vector<slot>&, std::forward_list<slot>&);
}
#endif

