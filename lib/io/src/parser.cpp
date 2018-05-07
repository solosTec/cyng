/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include <cyng/io/parser/parser.h>
#include <cyng/factory.h>
#include <cyng/io/parser/deserializer.h>

#ifdef DEBUG_CYNG_IO
#include <cyng/io/serializer.h>
#endif

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
				state_ = parse_length(c);
				if (state_ == NP_TYPE)
				{
					BOOST_ASSERT(type_.is_complete());
					BOOST_ASSERT(length_.is_null());
					//	no data required to create object
					switch (type_.type())
					{
					case TC_NULL:	next(make_object());	break;
					case TC_EOD:	next(make_object(eod()));	break;
					case TC_STRING:	next(make_object(std::string()));	break;
					case TC_BUFFER:	next(make_object(buffer_t()));	break;

					case TC_TUPLE:	next(tuple_factory()); break;
					case TC_VECTOR:	next(vector_factory()); break;
					case TC_SET:	next(set_factory()); break;
					case TC_ATTR_MAP:	next(attr_map_factory()); break;
					case TC_PARAM_MAP:	next(param_map_factory()); break;

					case TC_FS_PATH:	next(make_object(boost::filesystem::path()));	break;
					case TC_IP_TCP_ENDPOINT:	next(make_object(boost::asio::ip::tcp::endpoint()));	break;
					case TC_IP_UDP_ENDPOINT:	next(make_object(boost::asio::ip::udp::endpoint()));	break;
					case TC_IP_ICMP_ENDPOINT:	next(make_object(boost::asio::ip::icmp::endpoint()));	break;
					case TC_IP_ADDRESS:	next(make_object(boost::asio::ip::address()));	break;

					default:
						BOOST_ASSERT_MSG(false, "type requires data");
						break;
					}
					type_.reset();
					length_.reset();
				}
				break;
			case NP_VALUE:
				state_ = parse_value(c);
				break;

			default:
				break;
		}
	}
		
	std::size_t parser::post_processing()
	{
		return post_processing(stream_buffer_.size());
	}

	std::size_t parser::post_processing(std::size_t count)
	{
		//	trigger callback
		const std::size_t ops = code_.size();
		if (cb_)	cb_(std::move(code_));
		return ops;
	}
	
	void parser::next(object&& obj)
	{
#ifdef DEBUG_CYNG_IO
		std::stringstream ss;
		io::serialize_plain(std::cout, obj);
		std::cout << std::endl;
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
		
#ifdef DEBUG_CYNG_IO
		std::cout
			<< "type: "
			<< traits::get_type_name(type_.type())
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
	
 	object parser::create_custom_object(std::istream& is, std::size_t tag, std::uint64_t size)
 	{
		//
		//	see binary hpp
		//	serializer_custom<SERIALIZE_BINARY>
		//

 		//BOOST_ASSERT_MSG(false, "missing overwrite for create_custom_object()");
		std::string v;
		std::istreambuf_iterator<char> eos;
		std::string s(std::istreambuf_iterator<char>(is), eos);

 		return make_object(s);
 	}

	parser::state parser::parse_length(char c)
	{
		return (length_.put(c))
			? (length_.is_null() ? NP_TYPE : NP_VALUE)
			: NP_LENGTH
			;
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
			BOOST_ASSERT(pos_ != size_);
			source_[pos_++] = c;
#ifdef DEBUG_CYNG_IO
			if (is_complete())
			{
				std::cout
					<< "length: "
					<< length()
					<< std::endl
					;
				return true;
			}
			return false;
#else
			return is_complete();
#endif
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
			}
			else if (source_[0] == 127)
			{
				//	64 bit length
				return *reinterpret_cast<std::uint64_t const*>(&source_[1]);
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
				source_[0] = static_cast<unsigned char>(size & 0xff);
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
		}


	}
}	//	cyng

