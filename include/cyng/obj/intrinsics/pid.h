/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_INTRINSCIS_PID_H
#define DOCC_OBJ_INTRINSCIS_PID_H	

#include <cstdint>
#include <cstddef>
#include <boost/process/child.hpp>

namespace docscript {

	/**
	 * process ID
	 * Defines a system independed data type to serialize and transmit 
	 * a process ID.
	 */
	class pid {

	public:
		using value_type = std::uint64_t;

	public:
		constexpr pid(boost::process::pid_t pid) 
			: pid_(pid)
		{}
		pid(pid const&) = default;
		pid() = delete;

		pid& operator=(pid const&) = default;

		/**
		 * Always convert the PID to std::uint64_t
		 */
		constexpr operator value_type() const noexcept {
			return static_cast<value_type>(pid_);
		}

		constexpr boost::process::pid_t get_internal_value() const noexcept {
			return pid_;
		}
		
	private:
		//	internal representation
		boost::process::pid_t pid_;
	};

	//	comparison
	bool operator==(pid const&, pid const&) noexcept;
	bool operator<(pid const&, pid const&) noexcept;
	bool operator!=(pid const&, pid const&) noexcept;
	bool operator>(pid const&, pid const&) noexcept;
	bool operator<=(pid const&, pid const&) noexcept;
	bool operator>=(pid const&, pid const&) noexcept;

}

#include <functional>

namespace std {

	template <>
	class hash<docscript::pid> {
	public:
		size_t operator()(docscript::pid const&) const noexcept;
	};
}

#endif 
