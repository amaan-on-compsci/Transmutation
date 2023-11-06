cmake_minimum_required(VERSION 3.5)
project(imageRenderer)

#wxWidgets flags
set(wxBUILD_SHARED OFF)
add_subdirectory(wxWidgets)

add_library(vendor INTERFACE)

 target_link_libraries(vendor 
    INTERFACE 
        wx::net 
        wx::core
        wx::base
    )