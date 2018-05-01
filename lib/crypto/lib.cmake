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
	lib/crypto/src/x509.cpp
)
    
set (crypto_h
    src/main/include/cyng/crypto.h
    src/main/include/cyng/crypto/base64.h
    src/main/include/cyng/intrinsics/digest.h
    src/main/include/cyng/crypto/hash/md5.h
    src/main/include/cyng/crypto/hash/sha1.h
    src/main/include/cyng/crypto/hash/sha256.h
    src/main/include/cyng/crypto/hash/sha512.h
    src/main/include/cyng/crypto/scrambler.hpp
    src/main/include/cyng/crypto/rotating_counter.hpp
    src/main/include/cyng/crypto/aes_keys.h
    src/main/include/cyng/crypto/aes.h
    src/main/include/cyng/crypto/x509.h
)

set (crypto_open_ssl
	lib/crypto/src/openssl/applink.c
)

source_group("OpenSSL" FILES ${crypto_open_ssl})

# define the crypto lib
set (crypto_lib
  ${crypto_cpp}
  ${crypto_h}
  ${crypto_open_ssl}
)
