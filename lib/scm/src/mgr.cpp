/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */
#include <cyng/scm/mgr.h>

namespace scm
{
	mgr::mgr(bool admin)
		: hdl_(::OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, admin ? SC_MANAGER_ALL_ACCESS : SC_MANAGER_ENUMERATE_SERVICE))
		
	{}

	mgr::~mgr()
	{
		::CloseServiceHandle(hdl_);
	}

	bool mgr::start(std::string name)
	{
		SC_HANDLE service = OpenService(hdl_, name.c_str(), SERVICE_START);
		if (!service)	return false;
		bool ret = ::StartService(service, 0, NULL);
		::CloseServiceHandle(service);
		return ret;
	}

	bool mgr::stop(std::string name)
	{
		SC_HANDLE service = ::OpenService(hdl_, name.c_str(), SERVICE_STOP);
		if (!service)	return false;
		SERVICE_STATUS status;
		bool ret = ::ControlService(service, SERVICE_CONTROL_STOP, &status);
		::CloseServiceHandle(service);
		return ret;
	}

	services_t mgr::get_active_services()
	{
		return get_services(SERVICE_ACTIVE);
	}

	services_t mgr::get_stopped_services()
	{
		return get_services(SERVICE_INACTIVE);
	}

	services_t mgr::get_services(int mask)
	{
		services_t result;
		int const maxnum = 64000 / sizeof(ENUM_SERVICE_STATUS_PROCESS);
		ENUM_SERVICE_STATUS_PROCESS* pData = new ENUM_SERVICE_STATUS_PROCESS[maxnum];
		int const size = maxnum * sizeof(ENUM_SERVICE_STATUS_PROCESS); //the maximum
		DWORD needed = 0;
		DWORD num = 0;
		DWORD pos = 0;
		if (!EnumServicesStatusEx(hdl_
			, SC_ENUM_PROCESS_INFO
			, SERVICE_DRIVER | SERVICE_WIN32
			, mask
			, (BYTE*)pData
			, size
			, &needed
			, &num
			, &pos
			, NULL))
		{
			delete[] pData;
			return result;
		}

		for (int i = 0; i < num; ++i)
		{
			service srv;
			srv.name_ = pData[i].lpServiceName;
			srv.display_name = pData[i].lpDisplayName;
			srv.pid = pData[i].ServiceStatusProcess.dwProcessId;
			result.push_back(srv);
		}

		delete[] pData;
		return result;

	}

}