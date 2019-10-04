# 
#	reset 
#
set (crypto_lib)

set (crypto_cpp
	lib/crypto/src/md5.cpp
	lib/crypto/src/sha1.cpp
	lib/crypto/src/sha256.cpp
	lib/crypto/src/sha512.cpp
	lib/crypto/src/base64.cpp
	lib/crypto/src/aes.cpp
)
    
set (crypto_h
    src/main/include/cyng/crypto.h
    src/main/include/cyng/crypto/base64.h
    src/main/include/cyng/crypto/hash/md5.h
    src/main/include/cyng/crypto/hash/sha1.h
    src/main/include/cyng/crypto/hash/sha256.h
    src/main/include/cyng/crypto/hash/sha512.h
    src/main/include/cyng/crypto/scrambler.hpp
    src/main/include/cyng/crypto/rotating_counter.hpp
    src/main/include/cyng/crypto/aes.h
)

set (crypto_objects
    src/main/include/cyng/crypto/bio.h
	lib/crypto/src/bio.cpp
)

set (crypto_intrinsics
    src/main/include/cyng/intrinsics/digest.h
    src/main/include/cyng/crypto/aes_keys.h
)

set (crypto_util
    src/main/include/cyng/crypto/util/factory.h
    src/main/include/cyng/crypto/util/x509.h
    src/main/include/cyng/crypto/util/generate_key.h
    src/main/include/cyng/crypto/util/digest.h
    src/main/include/cyng/crypto/util/print.h
	lib/crypto/src/util/x509.cpp
	lib/crypto/src/util/generate_key.cpp
	lib/crypto/src/util/factory.cpp
	lib/crypto/src/util/digest.cpp
	lib/crypto/src/util/print.cpp
)

source_group("objects" FILES ${crypto_objects})
source_group("intrinsics" FILES ${crypto_intrinsics})
source_group("util" FILES ${crypto_util})


# define the crypto lib
set (crypto_lib
  ${crypto_cpp}
  ${crypto_h}
  ${crypto_objects}
  ${crypto_intrinsics}
  ${crypto_util}
)

if(WIN32)
	set (crypto_open_ssl
		lib/crypto/src/openssl/applink.c
	)

	source_group("OpenSSL" FILES ${crypto_open_ssl})
	list(APPEND crypto_lib ${crypto_open_ssl})
endif()
