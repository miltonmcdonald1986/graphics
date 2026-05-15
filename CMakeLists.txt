cmake_minimum_required(VERSION 3.25)
project(graphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(graphics_ENABLE_TESTING "Enable testing" ${PROJECT_IS_TOP_LEVEL})
if(graphics_ENABLE_TESTING)
  enable_testing()
endif()

add_subdirectory(graphics)
add_subdirectory(demos)