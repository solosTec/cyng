#include <cyng/store/db.h>


namespace cyng {

	store::store()
		: tables_()
		, m_()
	{}

	bool store::create_table(meta_store const& m) {

		//
		//	write lock
		//
		std::unique_lock<std::shared_mutex> ulock(m_);

		//
		//	insert
		//
		return tables_.emplace(m.get_name(), std::make_shared<table>(m)).second;
	}

	bool store::create_auto_table(meta_store const& m, key_t const& start_key, auto_table::auto_key_f f)
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


	std::size_t store::size() const noexcept {
		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		return tables_.size();
	}

	void store::clear(std::string const& name, boost::uuids::uuid source) {
		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&source](table* tbl) -> void {
			tbl->clear(source);
		}, access::write(name));
	}

	//record store::lookup(std::string const& name, key_t const& key) {

	//	std::shared_lock<std::shared_mutex> sl(m_);
	//	return this->access([&key](table const* tbl) -> record {
	//		return tbl->lookup(key);
	//	}, access::read(name));
	//}

	void store::connect(std::string name, slot s) {

		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&s](table* tbl) -> void {
			tbl->connect(s);
			}, access::write(name));

	}

	void store::connect_insert(std::string name, slot s) {

		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&s](table* tbl) -> void {
			tbl->connect_insert(s);
			}, access::write(name));
	}

	void store::connect_modify(std::string name, slot s) {

		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&s](table* tbl) -> void {
			tbl->connect_modify(s);
			}, access::write(name));
	}

	void store::connect_remove(std::string name, slot s) {

		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&s](table* tbl) -> void {
			tbl->connect_remove(s);
			}, access::write(name));
	}

	void store::connect_clear(std::string name, slot s) {

		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&s](table* tbl) -> void {
			tbl->connect_clear(s);
			}, access::write(name));
	}

	void store::disconnect(std::string name, slot s) {

		//
		//	read lock
		//
		std::shared_lock<std::shared_mutex> sl(m_);
		access([&s](table* tbl) -> void {
			tbl->disconnect(s);
			}, access::write(name));
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
