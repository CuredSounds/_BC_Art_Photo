cmake_minimum_required(VERSION 3.16)
project(BC_Art_Photo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(photo_core
    src/photo_core.cpp
    src/halide_bridge.cpp
)

target_include_directories(photo_core PUBLIC src)

add_executable(photo_core_test tests/photo_core_test.cpp)
target_link_libraries(photo_core_test PRIVATE photo_core)

enable_testing()
add_test(NAME photo_core_test COMMAND photo_core_test)
