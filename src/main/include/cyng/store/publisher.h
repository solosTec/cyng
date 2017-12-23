/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_PUBLISHER_H
#define CYNG_STORE_PUBLISHER_H


#include <boost/signals2.hpp>
#include <cyng/store/key.hpp>
#include <cyng/store/body.hpp>

#include <array>

namespace cyng 
{
	namespace store 
	{
		
		/**
		 * Forward declaration of derived class
		 */
		class table;
		
		/**
		 * Define a data type (array) to hold all 4 (typical) table connections.
		 */
		using connections_t = std::array<boost::signals2::connection, 4>;

		/**
		 * Implementation of signal/event handling. This is intended to be a base class of 
		 * the table implementation.
		 */
		class publisher
		{
		public:   
			using insert_signal = boost::signals2::signal<void(table const*, key_type const&, data_type const&)>;
			using remove_signal = boost::signals2::signal<void(table const*, key_type const&)>;
			using clear_signal  = boost::signals2::signal<void(table const*)>;
			using modify_signal = boost::signals2::signal<void(table const*, key_type const&, attr_t const&)>;
			
		public:
			publisher();
			publisher(publisher const&) = delete;
			publisher(publisher&&);
			virtual ~publisher();
			
			/**
			 * Disconnect all slots from
			 * all signals
			 */
			void disconnect();
			
			/**
			 * Connect to insert record slot 
			 *
			 *	Now would be a good time to transfer all available records to the listener,
			 *	because this is the reason it makes a connect. And it would be a perfect solution from
			 *	a low level view. But there are several problems with this approach. The first
			 *	is, data are nor available here (This problem could be solved). But another problem is
			 *	the handling of the first data initialisation is very different from a continous handling
			 *	of incoming inserts. To give the user a chance to optimize things, data would be send
			 *	otherwise.
			 *
			 *	Consider the following steps:
			 <ol>
			 <li>lock table</li>
			 <li>add the insert listener</li>
			 <li>optionally add other listeners (modify, delete, clear)</li>
			 <li>handle the bulk of the initial (and maybe huge) dataset</li>
			 <li>unlock table</li>
			 </ol>
			 *
			 * After this the complete table is synchronized and yoe are informed
			 * about all future changes.
			 */
			boost::signals2::connection get_insert_listener(const insert_signal::slot_type&);
			
			/**
			 * Connect to remove record slot 
			 */
			boost::signals2::connection get_remove_listener(const remove_signal::slot_type&);
			
			/**
			 * Connect to clear table slot 
			 */
			boost::signals2::connection get_clear_listener(const clear_signal::slot_type&);
			
			/**
			 * Connect to modify record slot 
			 */
			boost::signals2::connection get_modify_listener(const modify_signal::slot_type&);

			/**
			 * Create all connections in one call
			 */
			connections_t get_listener(insert_signal::slot_type const& isig
				, remove_signal::slot_type const& rsig
				, clear_signal::slot_type const& csig
				, modify_signal::slot_type const& msig);
			
		protected:
			insert_signal	insert_signal_;
			remove_signal	remove_signal_;
			clear_signal	clear_signal_;
			modify_signal	modify_signal_;
			
		};

		/**
		 * disconnect all connections
		 */
		void disconnect(connections_t&);

		
	}	//	store	
}

#endif	//	CYNG_STORE_PUBLISHER_H




