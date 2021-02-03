#
#	detect address model
#	and generate magic numbers
#
#	ToDo: use cmake_host_system_information(RESULT <variable> QUERY <key> ...)
#
if(CMAKE_SIZEOF_VOID_P EQUAL 8)

	message(STATUS "** Address Model      : 64 bit")
	set(${PROJECT_NAME}_ADDRESS_MODEL 64)
	string(RANDOM LENGTH 16 ALPHABET "1234567890ABCDEF" __RND_VALUE_NULL)
	string(RANDOM LENGTH 16 ALPHABET "1234567890ABCDEF" __RND_VALUE_EOD)

elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)

	message(STATUS "** Address Model      : 32 bit")
	set(${PROJECT_NAME}_ADDRESS_MODEL 32)
	string(RANDOM LENGTH 8 ALPHABET "1234567890ABCDEF" __RND_VALUE_NULL)
	string(RANDOM LENGTH 8 ALPHABET "1234567890ABCDEF" __RND_VALUE_EOD)

else()

	message(WARNING "** Address Model      : not supported")
	set(${PROJECT_NAME}_ADDRESS_MODEL 16)
	string(RANDOM LENGTH 4 ALPHABET "1234567890ABCDEF" __RND_VALUE_NULL)
	string(RANDOM LENGTH 4 ALPHABET "1234567890ABCDEF" __RND_VALUE_EOD)

endif()

#
#	query host system data
#
cmake_host_system_information(RESULT _HOST_FQDN QUERY FQDN)
cmake_host_system_information(RESULT _HOST_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
cmake_host_system_information(RESULT _HOST_NUMBER_OF_PHYSICAL_CORES QUERY NUMBER_OF_PHYSICAL_CORES)
cmake_host_system_information(RESULT _TOTAL_VIRTUAL_MEMORY QUERY TOTAL_VIRTUAL_MEMORY)
cmake_host_system_information(RESULT _TOTAL_PHYSICAL_MEMORY QUERY TOTAL_PHYSICAL_MEMORY)
cmake_host_system_information(RESULT _PROCESSOR_DESCRIPTION QUERY PROCESSOR_DESCRIPTION)

#
#	Generate magic numbers
#
set (${PROJECT_NAME}_NULL_HASH "0x${__RND_VALUE_NULL}")
set (${PROJECT_NAME}_EOD_HASH "0x${__RND_VALUE_EOD}")
