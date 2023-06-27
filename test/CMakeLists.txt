cmake_minimum_required (VERSION 3.13)

link_libraries(
    Boost::iostreams
    Boost::system
    ouinet::base
)

file(GLOB bt_cpp_files
    "../src/bittorrent/*.cpp"
    "../src/util/crypto.cpp"
    "../src/util/random.cpp"
    "../src/util/hash.cpp"
    "../src/logger.cpp"
    "../src/util.cpp"
    "../src/util/file_io.cpp"
    "../src/util/temp_file.cpp"
    "../src/util/atomic_file.cpp"
    "../src/util/handler_tracker.cpp"
)

add_executable(test-bittorrent "test_bittorrent.cpp" ${bt_cpp_files})
target_link_libraries(test-bittorrent lib::asio_utp lib::gcrypt lib::uri)
