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
#include <cyng/vm/manip.h>

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
		, children_()
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

		vm_.lib_.insert("vm.remove", 1, [&](context& ctx) {

			//
			//	remove specified VM from child list
			//
			vector_t frame = ctx.get_frame();
			auto const tag = value_cast(frame.at(0), boost::uuids::nil_uuid());
			auto const b = vm_.remove(tag);
			auto pos = children_.find(tag);
			if (pos != children_.end()) {

				//
				//	remove embedded VM
				//
				auto const r = children_.erase(pos);
			}

		});

	}
	
	bool controller::is_halted() const
	{
		return halt_.load();
	}

	controller const& controller::access(std::function<void(vm&)> cb) const
	{
		if (halt_)	return *this;
		if (
#if (BOOST_ASIO_VERSION < 101200)
			!dispatcher_.get_io_service().stopped()
#elif (BOOST_ASIO_VERSION < 101202)
			!dispatcher_.get_io_context().stopped()
#else
			!dispatcher_.context().stopped()
#endif
			) {

			dispatcher_.post([this, cb]() {

				if (!halt_)	cb(this->vm_);

			});
		}
		return *this;
	}

	controller const& controller::async_run(vector_t&& prg) const
	{
		if (halt_)	return *this;
		if (
#if (BOOST_ASIO_VERSION < 101200)
			!dispatcher_.get_io_service().stopped()
#elif (BOOST_ASIO_VERSION < 101202)
			!dispatcher_.get_io_context().stopped()
#else
			!dispatcher_.context().stopped()
#endif
		) 	{

			//
			//	grab the content of the code vector 
			//
			parameter param(std::move(prg));
			dispatcher_.post([this, param]() {

				if (!halt_)	this->vm_.run(std::move(param.prg_));

			});
		}
		return *this;
	}

	controller const& controller::async_run(std::initializer_list<vector_t> prgs) const
	{
		//
		//	collect all 
		//
		vector_t vec;
		for (auto const& prg : prgs)	{
			std::move(prg.begin(), prg.end(), std::back_inserter(vec));
		}
		return async_run(std::move(vec));
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
		if (halt_)	return *this;
		dispatcher_.post([this, name, arity, proc]() {

			this->vm_.lib_.insert(name, arity, proc);

		});

		return *this;
	}

	void controller::halt()
	{
		if (halt_)	return;

		access([this](cyng::vm& vm) {
			this->vm_.lib_.try_debug_log(vm_, "HALT");
			this->vm_.run(cyng::vector_t{ cyng::make_object(cyng::code::HALT) });
		});
		
	}

	controller& controller::emplace(boost::uuids::uuid tag, std::ostream& out, std::ostream& err)
	{
		auto* res = this;
		
		auto r = children_.emplace(std::piecewise_construct
			, std::forward_as_tuple(tag)
			, std::forward_as_tuple(dispatcher_.context(), tag, out, err));

		if (r.second) {

			//
			//	thread safe update of VM internal child list
			//
			access([&](vm& v) {

				auto pos = children_.find(tag);
				if (pos != children_.end()) {
					v.emplace(tag, pos->second.vm_);
				}
			});
			return r.first->second;
		}
		return *res;
	}

	void controller::forward(boost::uuids::uuid tag, vector_t const& vec)
	{
		vector_t prg;

		prg
			<< vec	//	copy
			<< tag
			<< code::FORWARD
			;


		async_run(std::move(prg));
	}

	controller::parameter::parameter()
		: prg_()
	{}

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






