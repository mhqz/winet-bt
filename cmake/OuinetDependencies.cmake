# This is used all over Ouinet's source.
include(${CMAKE_CURRENT_LIST_DIR}/dependencies/boost.cmake)

# For crypto, hash and random utilities.
include(${CMAKE_CURRENT_LIST_DIR}/dependencies/gcrypt.cmake)

# For URI parsing and encoding/decoding.
include(${CMAKE_CURRENT_LIST_DIR}/dependencies/uri.cmake)
