cmake_minimum_required(VERSION 3.5)
project(imageProcessor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

add_subdirectory(vendor)
add_subdirectory(src)