cmake_minimum_required( VERSION 3.5 )

project( tcp_server LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

add_test(NAME hasher COMMAND ./tests/hasher)
add_test(NAME session COMMAND ./tests/session)
add_test(NAME tcpserver COMMAND ./tests/tcpserver)

set_tests_properties(hasher PROPERTIES TIMEOUT 2)
set_tests_properties(session PROPERTIES TIMEOUT 2)
set_tests_properties(tcpserver PROPERTIES TIMEOUT 2)
