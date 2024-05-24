cmake_minimum_required(VERSION 3.27)

project(FSBuddyMiniFilter)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(
    CMAKE_CXX_CLANG_TIDY 
    "clang-tidy;-p=./build;-checks=-*,modernize-*,readibility-*,clang-analyzer-*,cppcoreguidelines-*,bugprone-*,hicpp-*,performance-*,-bugprone-suspicious-include,-clang-diagnostic-error;--extra-arg=-Iinclude"
)

include_directories("C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\km")

add_executable(
    FSBuddyMiniFilter
    main.cpp
    minifilter.hpp
    minifilter.rc
    minifilter.cpp
)

set_target_properties(
    FSBuddyMiniFilter 
    PROPERTIES 
    COMPILE_FLAGS
    "-Wall"
    OUTPUT_NAME
    "minifilter"
)
