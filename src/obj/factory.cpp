/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/factory.hpp>

namespace cyng {

	template <>
	object make_object(object&& obj)
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "make_object(object&& obj)" << std::endl;
#endif
		return std::move(obj);
	}

	template <>
	object make_object<object>(object const& obj)
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "make_object(object const& obj)" << std::endl;
#endif
		return obj.clone();
	}

	template <>
	object make_object<object>(object& obj)
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "make_object(object& obj)" << std::endl;
#endif
		return obj.clone();
	}

	/**
	 * produce the null object
	 */
	object make_object(void)
	{
#ifdef _DEBUG_OBJECT
		std::cerr << "make_object(void)" << std::endl;
#endif
		return make_object(null());
	}

	/**
	 * produce the EOD object
	 */
	object make_eod()
	{
		return make_object(eod());
	}

	namespace policy {

		boxing<char const*>::ptr_t boxing<char const*>::create(char const* p) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(value_t(p)), object::tracker_);
		}

		boxing<char const* const&>::ptr_t boxing<char const* const&>::create(char const* const& p) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(value_t(p)), object::tracker_);
		}

		boxing<buffer_t>::ptr_t boxing<buffer_t>::create(buffer_t val) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(std::move(val)), object::tracker_);
		}

		boxing<buffer_t>::ptr_t boxing<buffer_t&>::create(buffer_t& val) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(std::move(val)), object::tracker_);
		}

		boxing<buffer_t>::ptr_t boxing<buffer_t>::create(std::string const& val) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(value_t(std::begin(val), std::end(val))), object::tracker_);
		}

		boxing<buffer_t>::ptr_t boxing<buffer_t const&>::create(buffer_t const& val) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(val), object::tracker_);
		}

		boxing<buffer_t>::ptr_t boxing<buffer_t&&>::create(buffer_t&& val) {
			return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(std::move(val)), object::tracker_);
		}

		object boxing<object>::create(object const& obj) {
#ifdef _DEBUG_OBJECT
		std::cerr << "boxing<object>::create(object const& obj)" << std::endl;
#endif
			return obj.clone();
		}
		object boxing<object>::create(object&& obj) {
#ifdef _DEBUG_OBJECT
		std::cerr << "boxing<object>::create(object&& obj)" << std::endl;
#endif
			return std::move(obj);
		}
		object boxing<object&>::create(object& obj) {
#ifdef _DEBUG_OBJECT
			std::cerr << "boxing<object&>::create(object& obj)" << std::endl;
#endif
			return std::move(obj);
		}
		object boxing<object const&>::create(object const& obj) {
			return obj;
		}
	}

	vector_t make_vector(vector_t&& vec) {
#ifdef _DEBUG_OBJECT
		std::cerr << "make_vector(&&" << vec.size() << ")" << std::endl;
#endif
		return std::move(vec);
	}

	object address_factory(std::string str) {
		//	dotted address notation
		boost::system::error_code ec;
		return make_object(boost::asio::ip::make_address(str, ec));

	}

}

