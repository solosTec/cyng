# 
#	reset 
#
set (mail_lib)

set (mail_cpp
 	lib/io/mail/src/base64.cpp  
 	lib/io/mail/src/bit7.cpp  
 	lib/io/mail/src/codec.cpp   
 	lib/io/mail/src/imap.cpp       
 	lib/io/mail/src/message.cpp  
 	lib/io/mail/src/pop3.cpp     
 	lib/io/mail/src/quoted_printable.cpp
	lib/io/mail/src/binary.cpp  
	lib/io/mail/src/bit8.cpp  
	lib/io/mail/src/dialog.cpp  
	lib/io/mail/src/mailboxes.cpp  
	lib/io/mail/src/mime.cpp     
	lib/io/mail/src/q_codec.cpp  
	lib/io/mail/src/smtp.cpp
)
    
set (mail_h
    src/main/include/cyng/io/mail/base64.hpp  
    src/main/include/cyng/io/mail/bit7.hpp  
    src/main/include/cyng/io/mail/codec.hpp   
    src/main/include/cyng/io/mail/imap.hpp       
    src/main/include/cyng/io/mail/pop3.hpp     
    src/main/include/cyng/io/mail/quoted_printable.hpp
	src/main/include/cyng/io/mail/binary.hpp  
	src/main/include/cyng/io/mail/bit8.hpp  
	src/main/include/cyng/io/mail/dialog.hpp  
	src/main/include/cyng/io/mail/mime.hpp     
	src/main/include/cyng/io/mail/q_codec.hpp  
	src/main/include/cyng/io/mail/smtp.hpp
	src/main/include/cyng/io/mail/mailboxes.hpp  
    src/main/include/cyng/io/mail/message.hpp  
)

# define the io lib
set (mail_lib
  ${mail_cpp}
  ${mail_h}
)
