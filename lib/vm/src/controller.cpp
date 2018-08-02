/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/controller.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>
#include <cyng/io/serializer.h>

#include <memory>
#include <boost/assert.hpp>
#ifdef _DEBUG
#include <cyng/io/serializer.h>
#endif
#include <system_error>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{

	controller::controller(io_service_t& ios
		, boost::uuids::uuid tag
		, std::ostream& out
		, std::ostream& err)
	: dispatcher_(ios)
		, vm_(tag, out, err)
		, halt_(false)
	{
		vm_.lib_.insert("vm.halt", 0, [&](context& ctx) {

			if (halt_.exchange(true))
			{
				//
				//	already halted
				//
				err << vm_.tag() << " already halted" << std::endl;
			}
		});
	}
	
	bool controller::is_halted() const
	{
		return halt_.load();
	}

	controller const& controller::access(std::function<void(vm&)> cb) const
	{
		if (!halt_ ||
#if (BOOST_ASIO_VERSION < 101200)
			!dispatcher_.get_io_service().stopped()
#else
			!dispatcher_.get_io_context().stopped()
#endif
			) {

			dispatcher_.post([this, cb]() {

				cb(this->vm_);

			});
		}
		return *this;
	}

	controller const& controller::async_run(vector_t&& prg) const
	{
		if (!halt_ || 
#if (BOOST_ASIO_VERSION < 101200)
			!dispatcher_.get_io_service().stopped()
#else
			!dispatcher_.get_io_context().stopped()
#endif
		) 	{

			//
			//	grab the content of the code vector 
			//
			parameter param(std::move(prg));
			dispatcher_.post([this, param]() {

				this->vm_.run(std::move(param.prg_));

			});
		}
		return *this;
	}

	std::size_t controller::hash() const noexcept
	{
		static boost::hash<boost::uuids::uuid> uuid_hasher;
		return uuid_hasher(tag());
	}

	boost::uuids::uuid controller::tag() const noexcept
	{
		return vm_.tag();
	}

	bool controller::same_thread() const
	{
		return dispatcher_.running_in_this_thread();
	}

	controller const& controller::register_function(std::string name
		, std::size_t arity
		, vm_call proc)
	{
		dispatcher_.post([this, name, arity, proc]() {

			this->vm_.lib_.insert(name, arity, proc);

		});

		return *this;
	}

	controller::parameter::parameter(vector_t&& prg)
		: prg_(std::move(prg))
	{}

	controller::parameter::parameter(parameter const& other)
		: prg_(std::move(other.prg_))
	{}

	controller::parameter::parameter(parameter&& other)
		: prg_(std::move(other.prg_))
	{}

	std::size_t op_counter(vector_t const& prg, code op)
	{
		return std::count_if(prg.begin(), prg.end(), [&](object const& obj) {
			return ((obj.get_class().tag() == TC_CODE) && (value_cast(obj, code::NOOP) == op));
		});
	}

}






