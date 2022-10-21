/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_SLOT_INTERFACE_H
#define CYNG_STORE_SLOT_INTERFACE_H

#include <cyng/store/key.hpp>

#include <memory>

#include <boost/uuid/uuid.hpp>

namespace cyng {

	//
	//	forward declaration(s)
	//
	class table;

	class slot_interface
	{

	public:

		/**
		 * insert
		 */
		virtual bool forward(table const*
			, key_t const&
			, data_t const&
			, std::uint64_t
			, boost::uuids::uuid) = 0;

		/**
		 * update
		 */
		virtual bool forward(table const* tbl
			, key_t const& key
			, attr_t const& attr
			, data_t const& data
			, std::uint64_t gen
			, boost::uuids::uuid tag) = 0;

		/**
		 * remove
		 */
		virtual bool forward(table const* tbl
			, key_t const& key
			, data_t const& data
			, boost::uuids::uuid tag) = 0;

		/**
		 * clear
		 */
		virtual bool forward(table const*
			, boost::uuids::uuid) = 0;

		/**
		 * transaction
		 */
		virtual bool forward(table const*
			, bool) = 0;

	};

	using slot_ptr = std::shared_ptr<slot_interface>;

	inline slot_ptr make_slot(slot_interface* p) {
		return slot_ptr(p);
	}

}
#endif

