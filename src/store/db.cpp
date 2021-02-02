#include <cyng/store/db.h>


namespace docscript {

	db::db()
		: tables_()
		, m_()
	{}

	bool db::create_table(meta_store const& m) {

		//
		//	write lock
		//
		std::unique_lock<std::shared_mutex> ulock(m_);

		//
		//	insert
		//
		return tables_.emplace(m.get_name(), std::make_shared<table>(m)).second;
	}

	bool db::create_auto_table(meta_store const& m, key_t const& start_key, auto_table::auto_key_f f)
	{
		//
		//	write lock
		//
		std::unique_lock<std::shared_mutex> ulock(m_);

		//
		//	insert
		//
		return tables_.emplace(m.get_name(), std::make_shared<auto_table>(m, start_key, f)).second;
	}


	std::size_t db::size() const noexcept {
		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		return tables_.size();
	}

	void db::clear(std::string const& name, boost::uuids::uuid source) {
		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		//access([&source](table* tbl) {
		//	tbl->clear(source);
		//}, access::write(name));
	}

	namespace access {
		read::read(std::string name)
			: name_(name)
		{}
		write::write(std::string name)
			: name_(name)
		{}
	}

}
