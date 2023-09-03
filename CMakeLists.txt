cmake_minimum_required(VERSION 3.5)

project(Snake-Console LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Snake-Console main.cpp)

include(GNUInstallDirs)

install(TARGETS Snake-Console
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
