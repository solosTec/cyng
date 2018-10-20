/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/async/signal_handler.h>
#include <cyng/compatibility/async.h>
#include <future>
#include <csignal>
#if BOOST_OS_WINDOWS
#include <Windows.h>
#endif
namespace cyng
{

	/**
	 * global method to catch process signals
	 */
#if BOOST_OS_WINDOWS
	BOOL WINAPI handle_signal(DWORD ctrl_type);
#else
	void handle_signal(int sig);
#endif

	struct signal_handler
	{
		async::promise< int > result_;
		signal_handler()
			: result_()
		{   //  CTRL-C
#if BOOST_OS_WINDOWS
			::SetConsoleCtrlHandler(&handle_signal, TRUE);
#else
			std::signal(SIGINT, &handle_signal);
			std::signal(SIGHUP, &handle_signal);
#endif
		}
		~signal_handler()
		{   //  remove handler
#if BOOST_OS_WINDOWS
			::SetConsoleCtrlHandler(&handle_signal, FALSE);
#else
			std::signal(SIGINT, SIG_DFL);
			std::signal(SIGHUP, SIG_DFL);
#endif
		}

		int wait()
		{
			auto f = result_.get_future();
			return f.get();
		}

		void send_signal(int sig)
		{
			result_.set_value(sig);
		}
	};

#if BOOST_OS_WINDOWS
	BOOL WINAPI handle_signal(DWORD ctrl_type)
	{
		SIGABRT;
		BOOST_ASSERT_MSG(signal_mgr::impl_, "signal handler not initialized");
		switch (ctrl_type)
		{
		case CTRL_C_EVENT:	//	 CTRL+C 
		case CTRL_BREAK_EVENT:	//	CTRL+BREAK
		case CTRL_CLOSE_EVENT:	//	SIGINT (interrupt)
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			forward_signal(ctrl_type);
			return TRUE;
		default:
			return FALSE;
		}
	}
#else
	void handle_signal(int sig)
	{
		BOOST_ASSERT_MSG(signal_mgr::impl_, "signal handler not initialized");
		forward_signal(sig);
	}
#endif

	signal_mgr::signal_mgr()
	{
		BOOST_ASSERT_MSG(!impl_, "signal handler already installed");
		impl_.reset(new signal_handler());
	}

	signal_mgr::~signal_mgr()
	{
		//	explicit destruction
		delete impl_.release();
	}

	int signal_mgr::wait()
	{
		return impl_->wait();
	}

	//  static initialization
	std::unique_ptr< signal_handler > signal_mgr::impl_;


#if BOOST_OS_WINDOWS
	void forward_signal(DWORD sig)
	{
		BOOST_ASSERT_MSG(signal_mgr::impl_, "no signal handler installed");
		if (signal_mgr::impl_)
		{
			signal_mgr::impl_->send_signal(sig);
		}
	}
#else
	void forward_signal(int sig)
	{
		BOOST_ASSERT_MSG(signal_mgr::impl_, "no signal handler installed");
		if (signal_mgr::impl_)
		{
			signal_mgr::impl_->send_signal(sig);
		}
	}
#endif

}
