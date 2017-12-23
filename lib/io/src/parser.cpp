/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include <cyng/io/parser/parser.h>
#include <cyng/factory.h>
#include <cyng/io/parser/deserializer.h>

#include <iostream>
#include <ios>

namespace cyng 
{


	//
	//	parser
	//
	parser::parser(parser_callback cb)
	: state_(NP_TYPE)
	, cb_(cb)
	, code_()
	, type_()
	, length_()
	, stream_buffer_()
	, input_(&stream_buffer_)
	{
		BOOST_ASSERT_MSG(cb_, "no callback specified");
	}
	
	parser::~parser()
	{}

	
	void parser::put(char c)
	{
		//std::cout << std::hex << (unsigned(c) & 0xff) << std::endl;
		switch (state_)
		{
			case NP_ERROR:
				//	ToDo: error recovery
				break;
			case NP_TYPE:
				state_ = parse_type(c);
				break;
			case NP_LENGTH:
				if (parse_length(c))
				{
					//	read value
					state_ = NP_VALUE;
				}
				break;
			case NP_VALUE:
				state_ = parse_value(c);
				break;

			default:
				break;
		}
	}
		
	void parser::post_processing()
	{
		post_processing(stream_buffer_.size());
	}

	void parser::post_processing(std::size_t count)
	{
		//	trigger callback
		if (cb_)	cb_(std::move(code_));
	}
	
	void parser::next(object&& obj)
	{
#ifdef DEBUG_CYY_IO
		std::stringstream ss;
		//const bool b = 
		serialize_stream(ss, obj, io::custom_callback());
		std::cout << ss.str() << std::endl;
#endif
		code_.push_back(std::move(obj));
	}

	parser::state parser::parse_type(char c)
	{
		type_.put(c);
		if (!type_.is_complete())
		{
			return state_;
		}
		
#ifdef DEBUG_CYY_IO
		std::cout
			<< "type: "
			//<< types::name(type_.target_)
			<< types::name(type_.type())
			<< " ("
			<< type_.type()
			<< ")"
			<< std::endl
		;
#endif

		//
		//	There is always a length field even for types
		//	with a length of null or a known fixed size like
		//	integers. This simplifies the handling of customer 
		//	data types and makes the parsing more robust.
		//
		return NP_LENGTH;
		
	}
	
 	object parser::create_custom_object(std::istream&, std::uint32_t tag, std::uint64_t size)
 	{
 		BOOST_ASSERT_MSG(false, "missing overwrite for create_custom_object()");
 		return make_object();
 	}

	bool parser::parse_length(char c)
	{
#ifdef DEBUG_CYY_IO
		std::cout
			<< "length: "
			<< length_.length()
			<< std::endl
			;
#endif

		return length_.put(c);
	}
	
	parser::state parser::parse_value(char c)
	{
		//BOOST_ASSERT_MSG(length_.target_ != 0, "input get lost");
		if (stream_buffer_.size() < length_.length())
		{
			input_.put(c);
		}
		
		if (stream_buffer_.size() == length_.length())
		{
			// 				std::cout
			// 				<< &stream_buffer_
			// 				<< std::endl
			// 				;

			//	buffer complete - synthesize  object.
			if (built_in_tag(type_.type()))
			{
				next(io::deserialize(input_, type_.type(), length_.length()));
			}
			else
			{
				next(create_custom_object(input_, type_.type(), length_.length()));
			}
			
			//
			//	reset type  and length field
			//
			type_.reset();
			length_.reset();

			//
			//	next object
			//
			return NP_TYPE;
		}

		return state_;
	}
		

	namespace io
	{

		//
		//	implementation of length field
		//

		length_field::length_field()
			: source_{ 0 }
			, pos_(0)
		{}

		std::size_t length_field::size_of_length_field() const
		{
			BOOST_ASSERT_MSG(pos_ != 0, "no length field available");

			if (source_[0] == 126)
			{
				//	16 bit length
				//	the following 2 bytes interpreted as a 16
				//	bit unsigned integer are the payload length
				return 3u;
			}
			else if (source_[0] == 127)
			{
				//	64 bit length
				//	the following 8 bytes interpreted as a 64 - bit unsigned integer(the
				//	most significant bit MUST be 0) are the payload length
				return 9u;
			}
			//	complete (1 byte)
			//	length is less than 126 bytes
			return 1u;
		}

		bool length_field::put(char c)
		{
			if (pos_ == size_)
			{
				return true;
			}
			source_[pos_++] = c;
			return is_complete();
		}

		void length_field::reset()
		{
			std::fill(std::begin(source_), std::end(source_), '\0');
			pos_ = 0;
		}

		std::uint64_t length_field::length() const
		{
			if (source_[0] == 126)
			{
				//	16 bit length
				return *reinterpret_cast<std::uint16_t const*>(&source_[1]);
// 				return brute_cast<std::uint16_t, 1>(source_);
			}
			else if (source_[0] == 127)
			{
				//	64 bit length
				return *reinterpret_cast<std::uint64_t const*>(&source_[1]);
// 				return brute_cast<std::uint64_t, 1>(source_);
			}
			return source_[0];
		}

		bool length_field::is_complete() const
		{
			return !(pos_ < size_of_length_field());
		}

		bool length_field::is_null() const
		{
			return source_[0] == 0;
		}

		void length_field::set(std::size_t size)
		{
			if (size < 126)
			{
				source_[0] = size;
				pos_ = 1;
			}
			else if (size < 0xffff)
			{
				//	little hackish
				source_[0] = 126;
				pos_ = 5;
				*reinterpret_cast<std::uint16_t*>(&source_[1]) = size;
			}
			else
			{
				//	little hackish
				source_[0] = 127;
				pos_ = 9;
				*reinterpret_cast<std::uint64_t*>(&source_[1]) = size;
			}
		}

		//
		//	implementation of type field
		//

		type_field::type_field()
			: source_{ 0 }
			, pos_(0)
		{}

		bool type_field::put(char c)
		{
			if (pos_ == size_)
			{
				return true;
			}
			source_[pos_++] = c;
			return is_complete();
		}

		void type_field::reset()
		{
			std::fill(std::begin(source_), std::end(source_), '\0');
			pos_ = 0;
		}

		bool type_field::is_complete() const
		{
			return !(pos_ < size_);
		}

		std::size_t type_field::type() const
		{
			//	32 bit length
			return *reinterpret_cast<std::uint32_t const*>(source_);
// 			return brute_cast<std::uint32_t, 0>(source_);
		}


	}
}	//	cyng

