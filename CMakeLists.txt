cmake_minimum_required(VERSION 3.20...3.27)

project(scapix-core CXX)

add_library(scapix_core INTERFACE)
add_library(scapix::core ALIAS scapix_core)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "source/*")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})
target_sources(scapix_core PRIVATE ${sources})
target_include_directories(scapix_core INTERFACE "source")
