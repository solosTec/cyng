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
)
    
set (crypto_h
    src/main/include/cyng/crypto.h
    src/main/include/cyng/crypto/base64.h
    src/main/include/cyng/intrinsics/digest.h
    src/main/include/cyng/crypto/hash/md5.h
    src/main/include/cyng/crypto/hash/sha1.h
    src/main/include/cyng/crypto/hash/sha256.h
    src/main/include/cyng/crypto/hash/sha512.h
)

# define the crypto lib
set (crypto_lib
  ${crypto_cpp}
  ${crypto_h}
)