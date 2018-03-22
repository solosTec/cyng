//	Copyright (C) 2007-2012, solosTec gmbh
//	Copyright (C) 2012-2014, GWF AG
//	All rights reserved.
//---------------------------------------------------------------------
//	This software is protected by copyrights. It is not allowed
//	to modify or to redistribute without the express permission
//	of the owner.
//---------------------------------------------------------------------
//	developed by Sylko Olzscher
//---------------------------------------------------------------------
//	$Id: $
//---------------------------------------------------------------------
#include <cyy/scm/service.hpp>
#include <string.h>

//	initialize static member
//service_base*	service_broker::this_service = NULL;

/**
*	Constructor
*
*	The following events could be accepted
*	@li SERVICE_ACCEPT_STOP                    0x00000001
*	@li SERVICE_ACCEPT_PAUSE_CONTINUE          0x00000002
*	@li SERVICE_ACCEPT_SHUTDOWN                0x00000004
*	@li SERVICE_ACCEPT_PARAMCHANGE             0x00000008
*	@li SERVICE_ACCEPT_NETBINDCHANGE           0x00000010
*	@li SERVICE_ACCEPT_HARDWAREPROFILECHANGE   0x00000020
*	@li SERVICE_ACCEPT_POWEREVENT              0x00000040
*	@li SERVICE_ACCEPT_SESSIONCHANGE           0x00000080
*
*/
//service_base::service_base(const std::string& service_name)
//: handle_(NULL)
//, status_()
//, service_name_()
//, ctrl_handler_cb()
//, shutdown_(false)
//{
//	BOOST_ASSERT_MSG(service_name.size() < sizeof(service_name_), "service name to long");
//	BOOST_ASSERT_MSG(!service_name.empty(), "no service name");
//	BOOST_ASSERT_MSG(service_name.at(0) != '"', "quoted service name");
//
//	::memset(service_name_, 0, sizeof(service_name_));
//
//	//	copy max. 256 characters
//	::strcpy_s(service_name_, sizeof(service_name_), service_name.c_str());
//
//	//status_.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
//	status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
//	status_.dwCurrentState = SERVICE_STOPPED;
//	status_.dwControlsAccepted = SERVICE_ACCEPT_STOP;
//	status_.dwWin32ExitCode = NO_ERROR;
//	status_.dwServiceSpecificExitCode = 0;
//	status_.dwCheckPoint = 0;
//
//	//	The estimated time required for a pending start, stop, pause, or continue operation, in milliseconds.
//	status_.dwWaitHint = 12 * 1000;	//	12 sec
//
//	BOOST_ASSERT_MSG(service_broker::this_service == NULL, "wrong service broker");
//	service_broker::this_service = this;
//
//	std::stringstream ss;
//	ss
//		<< "service base class("
//		<< service_name_
//		<< ") successful constructed"
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//
//}

/**
*	Update status in SCM database.
*/
//bool service_base::update_status()
//{
//	if (handle_ != NULL)
//	{
//		return 0 == ::SetServiceStatus(handle_, &status_);
//	}
//	return false;
//}

//bool service_base::update_status(DWORD status)
//{
//	status_.dwCurrentState = status;
//	return update_status();
//}

//bool service_base::register_control_handler(ctrl_handler_f cb)
//{
//
//	//::DebugBreak();
//	std::stringstream ss;
//	ss
//		<< "RegisterServiceCtrlHandler("
//		<< service_name_
//		<< ")"
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//
//	if (service_broker::this_service != this)
//	{
//		::OutputDebugString("***Error: wrong service broker");
//	}
//	BOOST_ASSERT_MSG(service_broker::this_service == this, "wrong service broker");
//
//	ss.str("");
//	ss
//		<< "control handler address of "
//		<< service_name_
//		<< ": "
//		<< (void*)&service_broker::control_handler_
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//	
//	handle_ = ::RegisterServiceCtrlHandler(service_name_, (LPHANDLER_FUNCTION) &service_broker::control_handler_);
//	if (handle_ == 0)
//	{
//		status_.dwWin32ExitCode = ::GetLastError();
//
//		ss.str("");
//		ss
//			<< "RegisterServiceCtrlHandler("
//			<< status_.dwWin32ExitCode
//			<< ") failed"
//			<< std::endl
//			;
//		::OutputDebugString(ss.str().c_str());
//
//		return false;
//	}
//
//	ctrl_handler_cb.swap(cb);
//
//	ss.str("");
//	ss
//		<< "RegisterServiceCtrlHandler("
//		<< service_name_
//		<< ") successful registered"
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//	return true;
//}

//void service_base::control_handler(DWORD code)
//{
//	std::stringstream ss;
//	ss
//		<< "service "
//		<< service_name_
//		<< " received control code: "
//		<< code
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//
//	//	Just switch on the control code sent to 
//	//	us and call the relavent virtual function
//	switch (code)
//	{
//	case SERVICE_CONTROL_STOP:
//		update_status(SERVICE_STOP_PENDING);
//		try
//		{
//			shutdown_ = true;
//			if (ctrl_handler_cb)
//			{
//				ctrl_handler_cb(SERVICE_CONTROL_STOP);
//			}
//		}
//		catch (std::exception const& ex)
//		{
//			ss.str("");
//			ss
//				<< "service "
//				<< service_name_
//				<< " got an exception: "
//				<< ex.what()
//				<< std::endl
//				;
//
//			::OutputDebugString(ss.str().c_str());
//
//		}
//		catch (...)
//		{
//			ss.str("");
//			ss
//				<< "service "
//				<< service_name_
//				<< " got an exception "
//				<< std::endl
//				;
//
//			::OutputDebugString(ss.str().c_str());
//		}
//
//#ifdef _SRV_DEBUG
//		::OutputDebugString("NT Service: on_stop(SERVICE_STOPPED)\n");
//#endif
//		update_status(SERVICE_STOPPED);
//		break;
//	case SERVICE_CONTROL_PAUSE:
//		//on_pause();
//		break;
//	case SERVICE_CONTROL_CONTINUE:
//		//on_continue();
//		break;
//	case SERVICE_CONTROL_INTERROGATE:
//		//on_interrogate();
//		break;
//	case SERVICE_CONTROL_SHUTDOWN:
//		ctrl_handler_cb(SERVICE_CONTROL_SHUTDOWN);
//		break;
//	default:
//		ctrl_handler_cb(code);
//		break;
//	}
//
//	//	Any request from the SCM will be acked by this service
//	//update_status();
//}

/**
*	run as service. Starts the dispatcher.
*	This calls the service main() function and the
*	service is running.
*
*	@return If the function succeeds, the return value is nonzero.
*/
//DWORD service_base::run()
//{
//	//	start dispatcher
//	SERVICE_TABLE_ENTRY service_table[] =
//	{
//		{ service_name_, (LPSERVICE_MAIN_FUNCTION)(LPSERVICE_MAIN_FUNCTION) &service_broker::service_main },
//		{ NULL, NULL }
//	};
//
//	std::stringstream ss;
//	ss
//		<< "StartServiceCtrlDispatcher("
//		<< service_name_
//		<< ")"
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//
//	if (service_broker::this_service != this)
//	{
//		::OutputDebugString("***Error: wrong service broker");
//	}
//	BOOST_ASSERT_MSG(service_broker::this_service == this, "wrong service broker");
//
//	ss.str("");
//	ss
//		<< "main function address of "
//		<< service_name_
//		<< ": "
//		<< (void*)&service_broker::service_main
//		<< std::endl
//		;
//	::OutputDebugString(ss.str().c_str());
//
//	if (0 == ::StartServiceCtrlDispatcher(service_table))
//	{
//		status_.dwWin32ExitCode = ::GetLastError();
//	}
//	return status_.dwWin32ExitCode;
//}

//	+-----------------------------------------------------------------+
//	| service [declaration]
//	+-----------------------------------------------------------------+

//	+-----------------------------------------------------------------+
//	| service_broker [declaration]
//	+-----------------------------------------------------------------+

//void service_broker::service_main(DWORD argc, char* argv[])
//{
//	this_service->service_main(argc, argv);
//	this_service->update_status();
//}

//void service_broker::control_handler_(DWORD code)
//{
//	this_service->control_handler(code);
//}
