cmake_minimum_required(VERSION 3.28...4.5)

project(scapix-core VERSION 2.6.0 LANGUAGES CXX)

add_library(scapix_core INTERFACE)
add_library(scapix::core ALIAS scapix_core)
set_target_properties(scapix_core PROPERTIES EXPORT_NAME core)
target_compile_features(scapix_core INTERFACE cxx_std_20)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "source/*")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})
target_sources(scapix_core PRIVATE ${sources} INTERFACE FILE_SET HEADERS BASE_DIRS source FILES ${sources})

if(PROJECT_IS_TOP_LEVEL)
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(ScapixCoreConfigVersion.cmake COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)
  install(TARGETS scapix_core EXPORT ScapixCore FILE_SET HEADERS)
  install(EXPORT ScapixCore FILE ScapixCoreConfig.cmake NAMESPACE scapix:: DESTINATION share/cmake/ScapixCore)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ScapixCoreConfigVersion.cmake" DESTINATION share/cmake/ScapixCore)
endif()
