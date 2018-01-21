/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/io/serializer.h>
#include <cyng/io/serializer/serialize.hpp>
#include <sstream>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	namespace io
	{
		void serialize_plain(std::ostream& os, object const& obj)
		{
			serializer_helper<SERIALIZE_PLAIN>::write(os, obj);
		}
		
		void serialize_formatted(std::ostream& os, object const& obj)
		{
			serializer_helper<SERIALIZE_FORMATTED>::write(os, obj);
		}
		
		void serialize_binary(std::ostream& os, object const& obj)
		{
			serializer_helper<SERIALIZE_BINARY>::write(os, obj);
		}
		
		void serialize_json(std::ostream& os, object const& obj)
		{
			serializer_helper<SERIALIZE_JSON>::write(os, obj);
		}
		
		std::string to_str(object const& obj)
		{
			std::stringstream ss;
			serialize_plain(ss, obj);
			return ss.str();
		}
		
		std::string to_str(vector_t const& vec)
		{
			std::stringstream ss;
			serializer <vector_t, SERIALIZE_PLAIN>::write(ss, vec);
			return ss.str();
		}

		std::ostream& operator<<(std::ostream& os, attr_t const& attr)
		{
			boost::io::ios_flags_saver  ifs(os);

			os
			<< '('
			<< std::dec
			<< attr.first
			<< ':'
			;

			serialize_plain(os, attr.second);

			os
			<< ')'
			;
			
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, param_t const& param)
		{
			os
			<< '('
			<< '"'
			<< param.first
			<< '"'
			<< ':'
			;
			
			serialize_plain(os, param.second);
			
			os
			<< ')'
			;
			
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, std::pair<std::size_t, std::string> const&)
		{
			os << "ToDo";
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, null const&)
		{
			os << "null";
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, eod const&)
		{
			os << "eod";
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, version const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
			<< std::dec
			<< v.major()
			<< '.'
			<< v.minor()
			;
			
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, revision const& r)
		{
			boost::io::ios_flags_saver  ifs(os);
			
			const version v_maj(r.major());
			const version v_min(r.minor());
			
			os 
			<< std::dec
			<< v_maj.major()
			<< '.'
			<< v_maj.minor()
			<< '.'
			<< v_min.major()
			<< '.'
			<< v_min.minor()
			;

			return os;
		}
		
	
		std::ostream& operator<<(std::ostream& os, code c)
		{
			os << "op:";
			
			switch(c) 
			{
				case code::PUSH: 	os << "PUSH";	break;
				case code::PC:		os << "PC";		break;
				case code::PR:		os << "PR";		break;
				case code::CORA:	os << "CORA";	break;
				case code::ASP:		os << "ASP";	break;
				case code::CALL:	os << "CALL";	break;
				case code::JA:		os << "JA";		break;
				// 			JCT = 7,	//!< 	jump count, if (--ct) pc = x
				// 			JP = 8,		//!< 	jump positive, if (mem[sp++] > 0) pc = x
				// 			JN = 9,		//!< 	jump negative, if (mem[sp++] < 0) pc = x
				// 			JZ = 0xA,	//!< 	jump zero, if (mem[sp++] == 0) pc = x
				// 			JNZ = 0xB,	//!< 	jump nonzero, if (mem[sp++] != 0) pc = x
				// 			JODD = 0xC,	//!< 	jump odd, if (mem[sp++] % 2 == 1) pc = x
				// 			JZON = 0xD,	//!< 	jump zero or neg, if (mem[sp++] <= 0) pc = x
				// 			JZOP = 0xE,	//!< 	jump zero or pos, if (mem[sp++] >= 0) pc = x
				case code::JE:		os << "JE";		break;
				case code::JNE:		os << "JNE";	break;


				case code::RET:		os << "RET";	break;
			
				case code::ESBA: 	os << "ESBA";	break;
				case code::REBA:	os << "REBA";	break;
				
				case code::ADD:		os << "ADD";	break;
				case code::SUB:		os << "SUB";	break;
				
				case code::INVOKE:	os << "INVOKE";	break; 
				case code::IDENT:	os << "IDENT";	break;
				case code::NOW:		os << "NOW";	break;

					//	assembly
				case code::ASSEMBLE_ATTR:		os << "ATTR";	break;
				case code::ASSEMBLE_PARAM:		os << "PARAM";	break;
				case code::ASSEMBLE_ATTR_MAP:	os << "A_MAP";	break;
				case code::ASSEMBLE_PARAM_MAP:	os << "P_MAP";	break;
				case code::ASSEMBLE_TUPLE:		os << "TPL";	break;
				case code::ASSEMBLE_VECTOR:		os << "VEC";	break;
				case code::ASSEMBLE_SET:		os << "SET";	break;

					//	error register
				case code::LERR:	os << "LERR";	
				case code::TSTERR:	os << "TSTERR";	
				case code::RESERR:	os << "RESERR";	

				case code::HALT:	os << "HALT";	break;
				case code::NOOP:	os << "NOOP";	break;

				default: 
					BOOST_ASSERT_MSG(false, "unknown OP code");
					os.setstate(std::ios_base::failbit);
					break;
			}
			return os;
		}
	
	
		std::ostream& operator<<(std::ostream& os, mac48 const& v)
		{
			//	std::array< std::uint8_t, 6 >
			mac48::address_type const& octet = v.get_octets();

			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
			<< std::setfill('0')
			<< std::hex
			
			<< std::setw(2)
			<< +(octet[0])
			<< ':'
			<< std::setw(2)
			<< +(octet[1])
			<< ':'
			<< std::setw(2)
			<< +(octet[2])
			<< ':'
			<< std::setw(2)
			<< +(octet[3])
			<< ':'
			<< std::setw(2)
			<< +(octet[4])
			<< ':'
			<< std::setw(2)
			<< +(octet[5])
			
			;
			return os;
		}
		
		std::ostream& operator<<(std::ostream& os, mac64 const& v)
		{
			//	std::array< std::uint16_t, 4 >
			mac64::address_type const& words = v.get_words();

			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::setfill('0')
				<< std::hex

				<< std::setw(4)
				<< words[0]
				<< ':'
				<< std::setw(4)
				<< words[1]
				<< ':'
				<< std::setw(4)
				<< words[2]
				<< ':'
				<< std::setw(4)
				<< words[3]
				;
			return os;
		}
	
		std::ostream& operator<<(std::ostream& os, buffer_t const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			for (const char c : v)
			{
				if (c > 31 && c < 127)
				{
					os << c;
				}
				else if (c == '\\')
				{
					os << "\\\\";
				}
				else if (c == '"')
				{
					os << "\\\"";
				}
				else if (c == '\n')
				{
					os << "\\n";
				}
				else if (c == '\t')
				{
					os << "\\t";
				}
				else 
				{
					os
						<< "\\x"
						<< std::setw(2)
						<< +c
						;
				}
			}
			return os;
		}
	
	}
}



