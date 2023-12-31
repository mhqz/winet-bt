if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    # CMAKE_ANDROID_STANDALONE_TOOLCHAIN (and likely other cross compilation
    # android variables) was only introduced in v3.7
    cmake_minimum_required (VERSION 3.7)
else()
    cmake_minimum_required (VERSION 3.5)
endif()

################################################################################
# Configurable options
option(WITH_INJECTOR "Compile injector" OFF)

# NOTE: Need to disable ASan until Boost.Asio switches to Boost.Coroutine2
#       because trowing (and catching) exceptions inside a coroutine confuses
#       ASan. More info:
#       https://github.com/boostorg/coroutine/issues/30#issuecomment-325574878
option(WITH_ASAN "Build with Address sanitizer" OFF)


################################################################################
# Global build flags
if (WITH_ASAN)
    add_compile_options("-fsanitize=address" "-fno-omit-frame-pointer")
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options("-ggdb")
endif()
add_compile_options("-Wall")

add_compile_options("-O1")
add_definitions(-DBOOST_USE_WINDOWS_H -DWIN32_LEAN_AND_MEAN)
#add_compile_options("-Wa,-mbig-obj")

################################################################################
project(ouinet)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
include(OuinetDependencies)
#include(BuildVersion)


# This target needs to be linked by all code that includes ouinet headers.
add_library(ouinet_base INTERFACE)
add_library(ouinet::base ALIAS ouinet_base)
target_compile_options(ouinet_base
    INTERFACE -std=c++1z
)
target_include_directories(ouinet_base
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(ouinet_base
    INTERFACE
        Boost::asio
        Boost::boost
        Boost::date_time
        Boost::filesystem
        Boost::program_options
        Boost::regex
        Boost::system
        # generic_stream.h needs openssl headers, but not the library.
        #OpenSSL::headers
)
if (WITH_ASAN)
    # NOTE: https://stackoverflow.com/a/50163972
    target_link_libraries(ouinet_base
        INTERFACE -fsanitize=address
    )
endif()

###########################################################
option(ASIO_UTP_WITH_TESTS             "" OFF)
option(ASIO_UTP_WITH_EXAMPLES          "" OFF)
option(ASIO_UTP_DEBUG_LOGGING          "" OFF)
option(UTP_DEBUG_LOGGING               "" OFF)
add_subdirectory(./modules/asio-utp)
add_library(lib::asio_utp ALIAS asio_utp)

###########################################################
#set(CPPUPnP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/cpp-upnp)
#find_package(CPPUPnP REQUIRED)
###########################################################

################################################################################
file(GLOB client_sources
    "./src/client.cpp"
#    "./src/connect_to_host.cpp"
#    "./src/client_front_end.cpp"
#    "./src/endpoint.cpp"
##    "./src/cache_control.cpp"
#    "./src/request_routing.cpp"
#    "./src/ouiservice.cpp"
#    "./src/response_part.cpp"
#    "./src/bep5_swarms.cpp"
#    "./src/ssl/ca_certificate.cpp"
#    "./src/ssl/dummy_certificate.cpp"
#    "./src/ouiservice/lampshade.cpp"
#    "./src/ouiservice/pt-obfs2.cpp"
#    "./src/ouiservice/pt-obfs3.cpp"
#    "./src/ouiservice/pt-obfs4.cpp"
#    "./src/ouiservice/tcp.cpp"
#    "./src/ouiservice/utp.cpp"
#    "./src/ouiservice/tls.cpp"
#    "./src/ouiservice/bep5/client.cpp"
#    "./src/ouiservice/multi_utp_server.cpp"
#    "./src/ouiservice/connect_proxy.cpp"
#    "./src/ouiservice/pluggable-transports/*.cpp"
#    "./src/logger.cpp"
#    "./src/cache/*.cpp"
#    "./src/bittorrent/*.cpp"
#    "./src/util/atomic_dir.cpp"
#    "./src/util/atomic_file.cpp"
#    "./src/util/persistent_lru_cache.cpp"
#    "./src/util/crypto.cpp"
#    "./src/util/random.cpp"
#    "./src/util/file_io.cpp"
#    "./src/util/temp_dir.cpp"
#    "./src/util/temp_file.cpp"
#    "./src/util/hash.cpp"
#    "./src/util/reachability.cpp"
#    "./src/util.cpp"
#    "./src/util/handler_tracker.cpp"
#    "./src/doh.cpp"
#    "./src/http_util.cpp"
#    ${VERSION_CPP}
)

#if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    add_executable(client ${client_sources})
#else()
#    add_library(client SHARED ${client_sources})
#    add_library(ouinet::client ALIAS client)
#    find_library(android_log log)
#    target_link_libraries(client PUBLIC ${android_log})
#endif()
#
target_link_libraries(client
    PRIVATE
#        Boost::asio_ssl
#        Boost::iostreams
#        lib::asio_utp
#        cpp_upnp
#        lib::zdnsparser
#        lib::gcrypt
#        lib::json
#        lib::uri
#        OpenSSL::Crypto
##        ouinet::lampshade-asio
#        ouinet::ouiservice-i2p
##    PUBLIC
        ouinet::base
)

##################################################################################
## Tests
if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    add_subdirectory(test)
endif()
###################################################################################
