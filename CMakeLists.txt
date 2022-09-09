cmake_minimum_required (VERSION 3.10)
project(ufo)

add_compile_options(-g -ggdb -Wall -Wextra -pedantic)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE_FILES src/*/*.cpp)
include_directories(src)

add_library(UFO ${SOURCE_FILES})
add_executable(ufo src/main.cpp)
target_link_libraries(ufo UFO)

# testing
find_package(Catch2)
file(GLOB TEST_SOURCE_FILES test/*.cpp test/*/*.cpp)
add_executable(test-${PROJECT_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(test-${PROJECT_NAME} UFO Catch2WithMain)
