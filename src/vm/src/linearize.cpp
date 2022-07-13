/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/vm/linearize.hpp>
#include <cyng/io/serialize.h>

namespace cyng {

	cyng::buffer_t convert_binary(cyng::object const& obj) {

		boost::asio::streambuf	stream_buffer;
		std::iostream input(&stream_buffer);

		auto const size = io::serialize_binary(input, obj);
		cyng::buffer_t buffer;
		buffer.resize(size);
		input.read(buffer.data(), size);
		return buffer;
	}

}
