#include "duckdb_result.h"

#include <cyng/db/julian.h>
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/intrinsics/date.h>
#include <cyng/obj/tag.hpp>
#include <cyng/parse/buffer.h>
#include <cyng/parse/mac.h>
#include <cyng/parse/string.h>

#include <filesystem>
#include <iomanip>
#include <utility>

#include <boost/numeric/conversion/converter.hpp>
#include <boost/uuid/nil_generator.hpp>

#ifdef _DEBUG_DB
#include <iostream>
#endif

namespace cyng {
    namespace db {
        namespace duckdb {
            namespace detail {
                template <typename T> object get_value(sqlite3_stmt *, int index) { return make_object(); }

            } // namespace detail

            template <type_code C> object get_value_by_code(sqlite3_stmt *stmt, int index) {
                return detail::get_value<typename cyng::traits::reverse_type<C>::type>(stmt, index - 1);
            }

            duckdb_result::duckdb_result(statement::shared_type stmt)
                : statement_(stmt) {
                BOOST_ASSERT_MSG(statement_, "no valid statement");
                BOOST_ASSERT_MSG(statement_->state_ == statement::RUNNING, "RUNNING state expected");
            }

            result_ptr duckdb_result::factory(statement::shared_type stmt) {
                auto ptr = std::make_shared<sqlite_result>(stmt);
                return std::static_pointer_cast<result_interface>(ptr->shared_from_this());
            }

            object duckdb_result::get(int index, std::size_t code, std::size_t /*size*/) {
                BOOST_ASSERT_MSG(index != 0, "index out of range");
                try {

                    /**
                     * Column index in SQLite3 starts with 0
                     */
                    switch (code) {
                    case TC_BOOL: return get_value_by_code<TC_BOOL>(*statement_, index);
                    case TC_INT8: return get_value_by_code<TC_INT8>(*statement_, index);
                    case TC_UINT8: return get_value_by_code<TC_UINT8>(*statement_, index);
                    case TC_INT16: return get_value_by_code<TC_INT16>(*statement_, index);
                    case TC_UINT16: return get_value_by_code<TC_UINT16>(*statement_, index);
                    case TC_INT32: return get_value_by_code<TC_INT32>(*statement_, index);
                    case TC_UINT32: return get_value_by_code<TC_UINT32>(*statement_, index);
                    case TC_INT64: return get_value_by_code<TC_INT64>(*statement_, index);
                    case TC_UINT64: return get_value_by_code<TC_UINT64>(*statement_, index);

                    case TC_FLOAT: return get_value_by_code<TC_FLOAT>(*statement_, index);
                    case TC_DOUBLE: return get_value_by_code<TC_DOUBLE>(*statement_, index);
                    case TC_FLOAT80: return get_value_by_code<TC_FLOAT80>(*statement_, index);

                    //	standard library
                    case TC_STRING:
                        return get_value_by_code<TC_STRING>(*statement_, index);
                        //
                    case TC_TIME_POINT: return get_value_by_code<TC_TIME_POINT>(*statement_, index);
                    case TC_NANO_SECOND: return get_value_by_code<TC_NANO_SECOND>(*statement_, index);
                    case TC_MICRO_SECOND: return get_value_by_code<TC_MICRO_SECOND>(*statement_, index);
                    case TC_MILLI_SECOND: return get_value_by_code<TC_MILLI_SECOND>(*statement_, index);
                    case TC_SECOND: return get_value_by_code<TC_SECOND>(*statement_, index);
                    case TC_MINUTE: return get_value_by_code<TC_MINUTE>(*statement_, index);
                    case TC_HOUR:
                        return get_value_by_code<TC_HOUR>(*statement_, index);
                        // case cyng::types::CYNG_DAYS:		return get_value_by_code<cyng::types::CYNG_DAYS>(*statement_,
                        // index);

                        // 			//	data types defined in cyng library
                    case TC_VERSION: return get_value_by_code<TC_VERSION>(*statement_, index);
                    case TC_REVISION:
                        return get_value_by_code<TC_REVISION>(*statement_, index);
                        // 			CYNG_OP,			//!<	VM operation
                    case TC_BUFFER: return get_value_by_code<TC_BUFFER>(*statement_, index);
                    case TC_MAC48: return get_value_by_code<TC_MAC48>(*statement_, index);
                    case TC_MAC64: return get_value_by_code<TC_MAC64>(*statement_, index);

                    case TC_OBIS:
                        return get_value_by_code<TC_OBIS>(*statement_, index);
                        // case TC_EDIS:
                        // 			TC_COLOR_8,		//!<	color with 8 bits per channel
                        // 			TC_COLOR_16,	//!<	color with 16 bits per channel

                    case TC_DIGEST_MD5: return get_value_by_code<TC_DIGEST_MD5>(*statement_, index);
                    case TC_DIGEST_SHA1: return get_value_by_code<TC_DIGEST_SHA1>(*statement_, index);
                    case TC_DIGEST_SHA256: return get_value_by_code<TC_DIGEST_SHA256>(*statement_, index);
                    case TC_DIGEST_SHA512: return get_value_by_code<TC_DIGEST_SHA512>(*statement_, index);

                    case TC_AES128: return get_value_by_code<TC_AES128>(*statement_, index);
                    case TC_AES192: return get_value_by_code<TC_AES192>(*statement_, index);
                    case TC_AES256: return get_value_by_code<TC_AES256>(*statement_, index);

                    case TC_DATE:
                        return get_value_by_code<TC_DATE>(*statement_, index);
                        //
                        //	std::map< std::size_t, object >
                    case TC_ATTR_MAP: return get_value_by_code<TC_ATTR_MAP>(*statement_, index);
                    //	std::map< std::string, object >
                    case TC_PARAM_MAP:
                        return get_value_by_code<TC_PARAM_MAP>(*statement_, index);
                        // 			CYNG_ATTRIBUTE,	//!<	std::pair<std::size_t, object>
                        // 			CYNG_PARAMETER,	//!<	std::pair<std::string, object>
                        // 			CYNG_TUPLE,		//!<	std::list<object>
                        // 			CYNG_VECTOR,		//!<	std::vector<object>
                        // 			CYNG_SET,		//!<	std::set<object>
                        // 			CYNG_LOCKABLE,		//!<	std::pair<T,mutex>
                        // 			CYNG_TABLE,		//!<	defined in CYNG store library
                        //
                        // 			//	datatypes from boost library
                    case TC_UUID: return get_value_by_code<TC_UUID>(*statement_, index);
                    case TC_FS_PATH:
                        return get_value_by_code<TC_FS_PATH>(*statement_, index);
                        // 			CYNG_BOOST_ERROR,	//!<	boost::system::error_code
                    case TC_IP_ADDRESS: return get_value_by_code<TC_IP_ADDRESS>(*statement_, index);
                    case TC_IP_TCP_ENDPOINT:
                        return get_value_by_code<TC_IP_TCP_ENDPOINT>(*statement_, index);
                        // 			CYNG_BOOST_TCP_SOCKET,	//!<	boost::asio::ip::tcp::socket
                        // 			CYNG_BOOST_TRIBOOL,		//!<	boost::logic::tribool

                        break;

                    default: break;
                    }
                } catch (std::exception const &ex) {
#ifdef _DEBUG_DB
                    std::cerr << __FILE__ << ':' << __LINE__ << ' ' << ex.what() << std::endl;
#endif
                }
                return make_object();
            }

            std::size_t duckdb_result::column_count() {
                return 0;
                // return duckdb_column_count(result);
                // return ::sqlite3_column_count(*statement_);
            }

        } // namespace duckdb
    }     // namespace db
} // namespace cyng
