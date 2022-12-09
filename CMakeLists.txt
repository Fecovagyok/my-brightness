cmake_minimum_required(VERSION 3.21)
project(myFirst C)

set(CMAKE_C_STANDARD 99)

add_executable(bright-daemon daemon.c)
add_executable(bright-up clients/bright-up.c clients/client_common.c)
add_executable(bright-down clients/bright-down.c clients/client_common.c)
# add_executable(myFirst myFirst.c)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(bright-daemon PUBLIC DEBUG=1)
    target_compile_definitions(bright-up PUBLIC DEBUG=1)
    target_compile_definitions(bright-down PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(bright-daemon PUBLIC DEBUG=0)
    target_compile_definitions(bright-up PUBLIC DEBUG=0)
    target_compile_definitions(bright-down PUBLIC DEBUG=0)
endif()