/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_ESSION_WRAPPER_HPP
#define CYNG_DB_ESSION_WRAPPER_HPP

#include <cyng/db/interface_session.h>

namespace cyng
{
	namespace db 
	{
		/**	@brief wrapper for SQL database session implementation
		 *
		 * This template takes the given implementation and wraps the session
		 * interface around.
		 */
		template < typename T >
		class session_wrapper
			: public interface_session
			, public std::enable_shared_from_this< session_wrapper< T > >
		{
			typedef session_wrapper< T >	this_type;
			
		public:
			/**
			 * constructor with variable template pack
			 */
			template < typename ...Args >
			session_wrapper(Args &&... args)
			: impl_(std::forward<Args>(args)...)
			{}
			
			virtual ~session_wrapper()
			{
				if (is_alive())
				{
					close();
				}
			}
			
			template < typename ...Args >
			static session_ptr factory(Args &&... args)	
			{
				return std::make_shared< this_type >(std::forward<Args>(args)...)->get_session_ptr();
			}
			
			/**
			 *	Connect to database
			 */
			virtual std::pair<std::string, bool> connect(param_map_t const& config) override
			{
				return impl_.connect(config);
			}
			
			
			/**
			 *	@return true if session is connected, false otherwise.
			 */
			virtual bool is_alive() const override
			{
				return impl_.is_alive();
			}

			/**
			 *	Closes the session.
			 */
			virtual bool close() override
			{
				return impl_.close();
			}
			
			/**	@brief transaction interface
			 *	Starts a transaction.
			 */
			virtual bool begin() override
			{
				return impl_.begin();
			}

			/**	@brief transaction interface
			 *	Commits and ends a transaction.
			 */
			virtual bool commit() override
			{
				return impl_.commit();
			}

			/**	@brief transaction interface
			 *	Rolls back and ends a transaction.
			 */
			virtual bool rollback() override
			{
				return impl_.rollback();
			}

			/**
			 *	Returns true if a transaction is in progress, false otherwise.
			 */
			virtual bool is_transaction() const override
			{
				return impl_.is_transaction();
			}

			/**
			 *	@return last error code
			 */
			virtual boost::system::error_code last_error() const override
			{
				return impl_.last_error();
			}
			
			/**
			 *	@return an object to execute SQL commands
			 */
 			statement_ptr create_statement()
 			{
 				return impl_.create_statement();
 			}

			/**
			 * Execute a SQL command
			 */
			virtual bool execute(std::string const& sql) override
			{
				return impl_.execute(sql);
			}
			
			/**
			 * @return the SQl dialect MYSQL
			 */
			virtual cyng::sql::dialect get_dialect() const override
			{
				return impl_.get_dialect();
			}
			
		private:
			/**
			 *	Session object must be allocated from heap. Otherwise this method
			 *	throws an exception (bad_weak_ptr)
			 */
			session_ptr get_session_ptr()	
			{
				return this->shared_from_this();
			}
			
		private:
			/**
			 * the implementation class
			 */
			T	impl_;
			
		};
		
	}
}

#endif	//	CYNG_DB_ESSION_WRAPPER_HPP
