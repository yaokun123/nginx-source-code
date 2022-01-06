cmake_minimum_required(VERSION 2.8.4)
project(nginx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(nginx COMMAND make -C ${nginx_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})