
#include <cyng/asm/writer.h>

#include <cyng.h>

#include <cyng/io/serialize.h>
#include <cyng/io/serializer/binary.hpp>
#include <cyng/log/record.h>

#include <array>
#include <iostream>

namespace cyng {
    namespace assembler {
        writer::writer(logger _, std::filesystem::path out)
            : logger_(_)
            , osf_(out.string(), std::fstream::trunc | std::fstream::binary) {
            if (!osf_.is_open()) {
                CYNG_LOG_ERROR(logger_, "cannot open " << out);
            } else {
                //  write header - 12 bytes
                //  magic code
                for (char const c : CYNG_PROJECT_NAME) {
                    if (c != 0) {
                        osf_.put(c);
                    }
                }
                //  version
                //
                std::array<std::uint16_t, 4> const v{
                    CYNG_VERSION_MAJOR, CYNG_VERSION_MINOR, CYNG_VERSION_PATCH, CYNG_VERSION_TWEAK};
                io::write_binary(osf_, v);
            }
        }

        writer::~writer() { osf_.close(); }

        void writer::emit(object const &obj) {
            if (osf_.is_open()) {
                CYNG_LOG_TRACE(logger_, "(" << obj << ":" << obj.rtti().type_name() << ")");
                io::serialize_binary(osf_, obj);
            }
        }

    } // namespace assembler
} // namespace cyng
