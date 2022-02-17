/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSICS_H
#define CYNG_OBJ_INTRINSICS_H

//
//	from STL
//
#include <filesystem>
#include <chrono>
#include <cstdint>
#include <cstddef>
#include <tuple>
#include <string>

//
//	from boost
//
#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/icmp.hpp>
#include <boost/system/error_code.hpp>

//
//	add new intrinsics here
//
#include <cyng/obj/intrinsics/aes_key.hpp>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/color.hpp>
#include <cyng/obj/intrinsics/digest.hpp>
#include <cyng/obj/intrinsics/edis.h>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/obj/intrinsics/mac.h>
#include <cyng/obj/intrinsics/null.h>
#include <cyng/obj/intrinsics/obis.h>
#include <cyng/obj/intrinsics/op.h>
#include <cyng/obj/intrinsics/pid.h>
#include <cyng/obj/intrinsics/severity.h>
#include <cyng/obj/intrinsics/version.h>
#include <cyng/obj/intrinsics/raw.h>

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/intrinsics/program.h>

#endif //	CYNG_OBJ_INTRINSICS_H
