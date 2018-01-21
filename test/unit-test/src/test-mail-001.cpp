
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-mail-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/log.h>
// #include <cyng/async/scheduler.h>
#include <cyng/io/mail/message.hpp>
#include <cyng/io/mail/smtp.hpp>


using mailio::message;
using mailio::mail_address;
using mailio::smtps;
using mailio::smtp_error;
using mailio::dialog_error;
using std::cout;
using std::endl;
namespace cyng 
{

	bool test_mail_001()
	{
		
		//
		//	Gmail
		//
// 		try
// 		{
// 			// create mail message
// 			message msg;
// 			msg.sender(mail_address("Commander Pirx", "CmdPirx@gmail.com"));// set the correct sender name and address
// 			msg.add_recipient(mail_address("Sylko Olzscher", "sylko.olzscher@gmail.com"));// set the correct recipent name and address
// 			msg.subject("smtps simple message");
// 			msg.content("Hello, World!");
// 
// 			// connect to server
// 			cout << "connect to smtp.gmail.com" << endl;
// 			smtps conn("smtp.gmail.com", 587);	// TLS
// 			// modify username/password to use real credentials
// 			conn.authenticate("CmdPirx@gmail.com", "seven9", smtps::auth_method_t::START_TLS);
// 			conn.submit(msg);
// 		}
// 		catch (smtp_error& exc)
// 		{
// 			cout << "smtp_error: " <<  exc.what() << endl;
// 		}
// 		catch (dialog_error& exc)
// 		{
// 			cout << "dialog_error: " << exc.what() << endl;
// 		}
		
		//
		//	Hetzner - solosTec.de
		//
		try
		{
			// create mail message
			message msg;
			msg.sender(mail_address("Sylko Olzscher", "sylko.olzscher@solostec.de"));// set the correct sender name and address
			msg.add_recipient(mail_address("Sylko Olzscher", "sylko.olzscher@gmail.com"));// set the correct recipent name and address
// 			msg.add_recipient(mail_address("Ayke Mauve", "malve0712@gmail.com"));// set the correct recipent name and address
			msg.subject("e-mail with an attitude");
			std::ifstream ifs1("test/favicon.png");
			msg.attach(ifs1, "favicon.png", message::media_type_t::IMAGE, "png");
			msg.content("Have a nice day from the office room");

			// connect to server
			cout << "connect to mail.solostec.de" << endl;
// 			smtps conn("smtp.gmail.com", 465);	// SSL
			smtps conn("mail.solostec.de", 25);	// TLS
			// modify username/password to use real credentials
			conn.authenticate("sylko.olzscher@solostec.de", "seven9", smtps::auth_method_t::START_TLS);
			conn.submit(msg);
		}
		catch (smtp_error& exc)
		{
			cout << "smtp_error: " <<  exc.what() << endl;
		}
		catch (dialog_error& exc)
		{
			cout << "dialog_error: " << exc.what() << endl;
		}
		
		return true;
	}
	
}
