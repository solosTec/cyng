/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_WRITER_H
#define CYNG_ASM_WRITER_H

#include <cyng/log/logger.h>
#include <cyng/obj/object.h>

#include <filesystem>
#include <fstream>

namespace cyng {
    namespace assembler {
        class writer {

          public:
            writer(logger, std::filesystem::path);
            ~writer();

            void emit(object const &);

          private:
            logger logger_;
            std::ofstream osf_;
        };

    } // namespace assembler
} // namespace cyng

#endif
