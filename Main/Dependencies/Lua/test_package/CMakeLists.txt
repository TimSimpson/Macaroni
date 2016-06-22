project(MyHello)
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(hello-lua hello-lua.cpp)
target_link_libraries(hello-lua ${CONAN_LIBS})
