/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_PUB_H
#define CYNG_STORE_PUB_H

#include <cyng/store/key.hpp>
#include <cyng/store/slot_interface.h>

#include <vector>
#include <forward_list>

#include <boost/uuid/uuid.hpp>

namespace cyng {

	class table;
	class pub
	{
	public:
		pub();

		/**
		 * During the connection, all data will be sent as inserts.
		 */
		void connect(slot_ptr);

		/**
		 * No inserts will be sent.
		 */
		void connect_only(slot_ptr);
		void connect_insert(slot_ptr);
		void connect_modify(slot_ptr);
		void connect_remove(slot_ptr);
		void connect_clear(slot_ptr);

		/**
		 * @return true if insert-slot was active
		 */
		bool disconnect(slot_ptr);

	protected:
		virtual void charge(slot_ptr) = 0;

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
			, data_t const&
			, boost::uuids::uuid);

		/**
		 * signal an clear event
		 */
		void forward(table const*
			, boost::uuids::uuid);

	private:
		std::vector<slot_ptr>	slots_insert_;
		std::vector<slot_ptr>	slots_modify_;
		std::vector<slot_ptr>	slots_remove_;
		std::vector<slot_ptr>	slots_clear_;
	};

	/**
	 * remove a slot from the container
	 */
	bool disconnect(std::vector<slot_ptr>&, slot_ptr);

	/**
	 * remove a list of slots from the container
	 */
	void tidy(std::vector<slot_ptr>&, std::forward_list<slot_ptr>&);
}
#endif

