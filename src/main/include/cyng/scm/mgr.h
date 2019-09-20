/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#pragma once

#include <boost/asio.hpp>
#include <WinSvc.h>
#include <string>
#include <vector>

namespace scm
{
	struct service
	{
		std::string name_;
		std::string display_name;
		int pid;
	};

	using services_t = std::vector< service >;

	class mgr
	{
	public:
		mgr(bool admin);
		virtual ~mgr();

		bool start(std::string);
		bool stop(std::string);

		services_t get_active_services();
		services_t get_stopped_services();

	private:
		services_t get_services(int mask);

	private:
		SC_HANDLE hdl_;
	};
}