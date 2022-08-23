/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

/**	@file service.hpp
 *
 *	Windows NT service template. Here we are in windows land.
 *	This code is shurely not portable.
 */
#pragma once

#include <boost/asio.hpp>

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>


typedef std::function<void(DWORD)>	ctrl_handler_f;


/**
 *	The following template interface must be provided:
 *
 *	@li typedef service_impl_type and start_parameter_type
 *	@li Constructor with ( DWORD argc, char* argv[] )
 *	@li method: stop()
 *	@li method: run();
 *	@li method: reload();
 *	@li method: process_XML( start_parameter_type& );
 *	@li method: start_parameter_type::get_service_name()
 */
template < typename SRV_T >
class service
	: private boost::noncopyable
{

	/**
	 * Broker between windows API calls and implementation class.
	 */
	struct broker
	{
		static service*	this_service;

		static void WINAPI service_main(DWORD argc, char* argv[])
		{
			this_service->service_main(argc, argv);
			this_service->update_status();
		}

		static void WINAPI control_handler_(DWORD code)
		{
			this_service->control_handler(code);
		}
	};

public:
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
	service(SRV_T* srv, const std::string& service_name)
		: service_(srv)
		, handle_(NULL)
		, status_()
		, service_name_()
		, ctrl_handler_cb()
		, shutdown_(false)
	{
		BOOST_ASSERT_MSG(service_name.size() < sizeof(service_name_), "service name to long");
		BOOST_ASSERT_MSG(!service_name.empty(), "no service name");
		BOOST_ASSERT_MSG(service_name.at(0) != '"', "quoted service name");

		::memset(service_name_, 0, sizeof(service_name_));

		//	copy max. 256 characters
		::strcpy_s(service_name_, sizeof(service_name_), service_name.c_str());

		//status_.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
		status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		status_.dwCurrentState = SERVICE_STOPPED;
		status_.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		status_.dwWin32ExitCode = NO_ERROR;
		status_.dwServiceSpecificExitCode = 0;
		status_.dwCheckPoint = 0;

		//	The estimated time required for a pending start, stop, pause, or continue operation, in milliseconds.
		status_.dwWaitHint = 12 * 1000;	//	12 sec

		BOOST_ASSERT_MSG(broker::this_service == nullptr, "wrong service broker");
		broker::this_service = this;

		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] service base class successful constructed"
				<< std::endl
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}
	}

	/**
	 *	This parameters are NOT from the command line.
	 *	The property dialog of the service manager allows to specify 
	 *	additional "Start Parameters" that will passed here.
	 *
	 *	@param argc argument count
	 *	@param argv argument vector
	 */
	void service_main(DWORD argc, char* argv[])
	{
#ifdef _DEBUG
		//	now we have 30 sec. to startup
		//::DebugBreak();
#endif
		for (auto idx = 0; idx < argc; ++idx) {
			::OutputDebugString(argv[ idx ]);
		}

		//
		//	create service shell on stack
		//

		if (register_control_handler(std::bind(& SRV_T::control_handler, service_, std::placeholders::_1)))
		{
			{
				std::stringstream ss;
				ss
					<< "["
					<< service_name_
					<< "] update status: SERVICE-START-PENDING"
					<< std::endl
					;
				auto const msg = ss.str();
				::OutputDebugString(msg.c_str());
			}
			update_status(SERVICE_START_PENDING);


			//	startup server
			if (service_ != nullptr) {

				{
					std::stringstream ss;
					ss
						<< "["
						<< service_name_
						<< "] update status: SERVICE-RUNNING"
						;
					auto const msg = ss.str();
					::OutputDebugString(msg.c_str());
				}
				update_status(SERVICE_RUNNING);

				//
				//	start service
				//
				service_->run();

				while (!shutdown_) {
					//	Wait for all threads in the pool to exit.
					::Sleep(500);	//	1/2 second
				}
				service_ = nullptr;
			}
			else {
				::OutputDebugString("***error: no service\n");
			}

			{
				std::stringstream ss;
				ss
					<< "["
					<< service_name_
					<< "] update status: SERVICE-STOP-PENDING"
					;
				auto const msg = ss.str();
				::OutputDebugString(msg.c_str());
			}
			update_status(SERVICE_STOP_PENDING);
		}

		::Sleep(1500);	//	1+1/2 second more to calm down
		update_status(SERVICE_STOPPED);
		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] update status: SERVICE-STOPPED"
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}
	}

	/**
	*	run as service. Starts the dispatcher.
	*	This calls the service main() function and the
	*	service is running.
	*
	*	@return If the function succeeds, the return value is nonzero.
	*/
	DWORD run()
	{
		//	start dispatcher
		SERVICE_TABLE_ENTRY service_table[] =
		{
			{ service_name_, (LPSERVICE_MAIN_FUNCTION)&broker::service_main },
			{ NULL, NULL }
		};

		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] StartServiceCtrlDispatcher"
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}

		if (broker::this_service != this) {
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] ***Error: wrong service broker"
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}
		BOOST_ASSERT_MSG(broker::this_service == this, "wrong service broker");

		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] main function address = "
				<< (void*)&broker::service_main
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}

		if (0 == ::StartServiceCtrlDispatcher(service_table))
		{
			status_.dwWin32ExitCode = ::GetLastError();
		}
		return status_.dwWin32ExitCode;
	}


private:

	void control_handler(DWORD code)
	{
		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] received control code: "
				<< code
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}

		//	Just switch on the control code sent to 
		//	us and call the relavent virtual function
		switch (code)
		{
		case SERVICE_CONTROL_STOP:
			update_status(SERVICE_STOP_PENDING);
			try
			{
				shutdown_ = true;
				if (ctrl_handler_cb)
				{
					ctrl_handler_cb(SERVICE_CONTROL_STOP);
				}
			}
			catch (std::exception const& ex)
			{
				std::stringstream ss;
				ss
					<< "["
					<< service_name_
					<< "] received an exception: "
					<< ex.what()
					;
				auto const msg = ss.str();
				::OutputDebugString(msg.c_str());

			}
			catch (...)
			{
				std::stringstream ss;
				ss
					<< "["
					<< service_name_
					<< "] received an exception: "
					<< ::GetLastError()
					;
				auto const msg = ss.str();
				::OutputDebugString(msg.c_str());
			}

#ifdef _SRV_DEBUG
			::OutputDebugString("NT Service: on_stop(SERVICE_STOPPED)\n");
#endif
			update_status(SERVICE_STOPPED);
			break;
		case SERVICE_CONTROL_PAUSE:
			//on_pause();
			break;
		case SERVICE_CONTROL_CONTINUE:
			//on_continue();
			break;
		case SERVICE_CONTROL_INTERROGATE:
			//on_interrogate();
			break;
		case SERVICE_CONTROL_SHUTDOWN:
			ctrl_handler_cb(SERVICE_CONTROL_SHUTDOWN);
			break;
		default:
			ctrl_handler_cb(code);
			break;
		}
	}


	/**
	 *	Update status in SCM database.
	 */
	bool update_status()
	{
		if (handle_ != NULL)
		{
			return 0 == ::SetServiceStatus(handle_, &status_);
		}
		return false;
	}

	bool update_status(DWORD status)
	{
		status_.dwCurrentState = status;
		return update_status();
	}

	bool register_control_handler(ctrl_handler_f cb)
	{
		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] RegisterServiceCtrlHandler"
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}

		if (broker::this_service != this) {
			::OutputDebugString("***Error: wrong service broker");
		}
		BOOST_ASSERT_MSG(broker::this_service == this, "wrong service broker");

		{
			std::stringstream ss;
			ss
				<< "control handler address of "
				<< service_name_
				<< ": "
				<< (void*)&broker::control_handler_
				<< std::endl
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}

		handle_ = ::RegisterServiceCtrlHandler(service_name_, (LPHANDLER_FUNCTION)&broker::control_handler_);
		if (handle_ == 0)
		{
			status_.dwWin32ExitCode = ::GetLastError();

			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] RegisterServiceCtrlHandler failed: "
				<< status_.dwWin32ExitCode
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());

			return false;
		}

		ctrl_handler_cb.swap(cb);

		{
			std::stringstream ss;
			ss
				<< "["
				<< service_name_
				<< "] RegisterServiceCtrlHandler - successful registered"
				<< std::endl
				;
			auto const msg = ss.str();
			::OutputDebugString(msg.c_str());
		}
		return true;
	}



private:
	/**
	 * implementation class (controller_base)
	 */
	SRV_T*	service_;

	SERVICE_STATUS_HANDLE	handle_;
	SERVICE_STATUS			status_;
	char					service_name_[256];
	ctrl_handler_f			ctrl_handler_cb;
	bool					shutdown_;

	static	broker	broker_;
};

//
//	Initialize static members of this template
//
template < typename SRV_T >
service< SRV_T>*
service< SRV_T>::broker::this_service = nullptr;
