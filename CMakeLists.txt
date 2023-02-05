cmake_minimum_required( VERSION 3.5 )

project( tcp_server LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

add_test(NAME test COMMAND ./tests/tests)

