/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_STORE_AUTO_TABLE_H
#define DOCC_STORE_AUTO_TABLE_H

#include <cyng/store/table.h>

namespace docscript {

	class auto_table : public table {

	public:
		/**
		 * calculate next key based on the current key.
		 */
		using auto_key_f = std::function<key_t(key_t const&)>;

	public:
		auto_table() = delete;

		/**
		 * create a in-memory table from meta info
		 */
		auto_table(meta_store const&, key_t const& start_key, auto_key_f f);

		virtual ~auto_table();

		virtual bool is_auto() const override;

		/**
		 * The next key is calculated via the provided function. The generation is always 1.
		 *
		 * @param data the body to insert
		 * @param source identifier for data source
		 * @return true if the record was actually inserted.
		 */
		std::pair< key_t, bool > insert(data_t&& data
			, boost::uuids::uuid source);

	private:
		key_t current_key_;
		auto_key_f next_key_;
	};
}
#endif

