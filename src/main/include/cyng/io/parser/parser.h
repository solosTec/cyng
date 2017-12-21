/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_PARSER_H
#define CYNG_IO_PARSER_H


#include <cyng/compatibility/io_service.h>
#include <cyng/core/object_interface_fwd.h>
#include <cyng/intrinsics.h>

#include <type_traits>
#include <algorithm>
#include <memory>
#include <functional>

namespace cyng 
{
	namespace io
	{
		/**
		 * helper class to justify and calculate the size of
		 * the length field.
		 */
		class length_field
		{
		public:
			length_field();

			/**
			 * Add a new char to the field.
			 *
			 * @return true if length field is complete
			 */
			bool put(char c);

			/**
			 * reset internal values
			 */
			void reset();

			/**
			 * @return the calculated length.
			 */
			std::uint64_t length() const;

			/**
			 * @return true if length field is complete
			 */
			bool is_complete() const;

			/**
			 * @return true if the calculated length is null
			 */
			bool is_null() const;

			/*
			 * set value of length field
			*/
			void set(std::size_t);

		private:
			/**
			 * @return size of length field in bytes.
			 */
			std::size_t size_of_length_field() const;

		private:
			constexpr static std::size_t size_{ sizeof(std::uint64_t) + 1 };
			alignas(std::size_t) unsigned char source_[size_];
			std::size_t pos_;
		};

		/**
		 * Data type is encoded in 4 bytes
		 */
		class type_field
		{
		public:
			type_field();

			/**
			 * Add a new char to the field.
			 *
			 * @return true if length field is complete
			 */
			bool put(char c);

			/**
			 * reset internal values
			 */
			void reset();

			/**
			 * @return the received data type code.
			 */
			std::size_t type() const;

			/**
			 * @return true if type field is complete
			 */
			bool is_complete() const;

		private:
			constexpr static std::size_t size_{ sizeof(std::uint32_t) };
			alignas(std::uint32_t) unsigned char source_[size_];
			std::size_t pos_;
		};

	}

	/**
	 * Native parser for output from native CYY serializer.
	 * 
	 * The parser maintains an internal state and is able
	 * to work on data streams.
	 * 
	 * The native_paser class is designed as base class for 
	 * the real parser. By overwriting the methods
	 * next(object&&) and post_processing(std::size_t count)
	 * all information available to control further
	 * data processing.
	 * 
	 * Keep in mind that composed structures like attributes
	 * and parameters are transfered as such but as an assembly 
	 * instruction. To rebuild the original use the VM provided
	 * by the cyng_vm library or build your own assembly line.
	 * The CYY VM (cyng_vm) library provides such a parser.
	 * 
	 * @note A first implementation used the pimpl idiom. But we
	 * found, that it's more easy to implement custom data types
	 * by simple overloading of class methods than to utilize
	 * a complex callback system.
	 * 
	 * @see parser
	 */
	class parser
	{
	public:
		using parser_callback = std::function<void(vector_t&&)>;
		
	private:
		/**
		 * This enum stores the global state
		 * of the parser. For each state there 
		 * are different helper variables mostly
		 * declared in the private section of this class.
		 */
		enum state 
		{
			NP_ERROR,
			
			//	t-l-v
			NP_TYPE,
			NP_LENGTH,
			NP_VALUE,
			
		}	state_;
		
	public:
		/**
		 * @param cb this function is called, when parsing is complete
		 */
		parser(parser_callback cb);
		
		/**
		 * The destructor is required since the unique_ptr
		 * uses an incomplete type.
		 */
		virtual ~parser();
		
		/**
		 * parse the specified range
		 */
		template < typename I >
		void read(I start, I end)
		{
			//	doesn't work for plain pointers
			//static_assert(std::is_same<typename I::value_type, char>::value, "wrong iterator type");
			std::for_each(start, end, [this](char c)
			{
				this->put(c);
			});
			
			post_processing();
		}
		
	protected:
		/**
		 * Overwrite this method to get each 
		 * parsed object step by step
		 */
		virtual void next(object&&);
		
		/**
		 * Overwrite this method to get informed when
		 * parsing of a data chunk/range is complete.
		 * Variable count contains the number of remaining
		 * bytes in the read buffer and should be in most cases
		 * zero.
		 */
		virtual void post_processing(std::size_t count);
		
		/**
		 * Overwrite this method to support custom data types.
		 */
 		virtual object create_custom_object(std::istream&, std::uint32_t tag, std::uint64_t size);
		
	private:
		
		/**
		 * read a single byte and update 
		 * parser state.
		 * Implements the state machine
		 */
		void put(char);
		
		/**
		 * Probe if parsing is completed and
		 * inform listener.
		 */
		void post_processing();
		
		/**
		* parser data type (4 bytes)
		*/
		state parse_type(char c);

		/**
		 * parser data length (8 bytes)
		 * 
		 * @return true if length is complete
		 */
		bool parse_length(char c);
		
		/**
		 * Parsing the value means to store
		 * all data into a stream and to create
		 * the object later by reading this stream.
		 * The objects will be synthesized by the deserializer.
		 */
		state parse_value(char c);
		
	private:
		/**
		 * call this method if parsing is complete
		 */
		parser_callback	cb_;

		/**
		 * instruction buffer
		 */
 		vector_t	code_;
		
		/**
		 * Data type is encoded in 4 bytes
		 */
		io::type_field	type_;

		/**
		 * Data length is encoded in 1, 3 or 9 bytes.
		 * Since definition of std::size_t is architecture dependend a fixed
		 * size data type is required.
		 */
		io::length_field	length_;

		/**
		 *	input stream buffer
		 */
		boost::asio::streambuf	stream_buffer_;

		/**
		 *	input stream
		 */
		std::iostream			input_;
	};
	
}	//	cyng

#endif // CYY_IO_NATIVE_PARSER_H
