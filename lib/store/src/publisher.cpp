/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/store/publisher.h>

namespace cyng 
{
	namespace store 
	{

		publisher::publisher()
		: insert_signal_()
		, remove_signal_()
		, clear_signal_()
		, modify_signal_()
		{}
		
		publisher::publisher(publisher&& pub)
		: insert_signal_(std::move(pub.insert_signal_))
		, remove_signal_(std::move(pub.remove_signal_))
		, clear_signal_(std::move(pub.clear_signal_))
		, modify_signal_(std::move(pub.modify_signal_))
		{}
		
		publisher::~publisher()
		{
			//disconnect();
		}
		
		void publisher::disconnect()
		{
			insert_signal_.disconnect_all_slots();
			remove_signal_.disconnect_all_slots();
			clear_signal_.disconnect_all_slots();
			modify_signal_.disconnect_all_slots();
		}
		
		boost::signals2::connection publisher::get_insert_listener(const insert_signal::slot_type& sig)
		{
			return insert_signal_.connect(sig);
		}
		
		boost::signals2::connection publisher::get_remove_listener(const remove_signal::slot_type& sig)
		{
			return remove_signal_.connect(sig);
		}
		
		boost::signals2::connection publisher::get_clear_listener(const clear_signal::slot_type& sig)
		{
			return clear_signal_.connect(sig);
		}
		
		boost::signals2::connection publisher::get_modify_listener(const modify_signal::slot_type& sig)
		{
			return modify_signal_.connect(sig);
		}
		
		connections_t publisher::get_listener(insert_signal::slot_type const& isig
			, remove_signal::slot_type const& rsig
			, clear_signal::slot_type const& csig
			, modify_signal::slot_type const& msig)
		{
			return connections_t{
				get_insert_listener(isig),
				get_remove_listener(rsig),
				get_clear_listener(csig),
				get_modify_listener(msig)
			};
		}


		void disconnect(connections_t& cs)
		{
			for (auto c : cs)
			{
				c.disconnect();
			}
		}
		
	}	//	store	
}

