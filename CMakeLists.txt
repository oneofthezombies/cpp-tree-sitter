cmake_minimum_required(VERSION 3.28)
project(cpp_tree_sitter VERSION 0.2.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(tree_sitter_path ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tree-sitter-0.22.2)

add_library(tree_sitter STATIC ${tree_sitter_path}/lib/src/lib.c)
target_compile_options(tree_sitter PRIVATE -std=c17 -fno-exceptions)
target_include_directories(
  tree_sitter SYSTEM
  INTERFACE $<BUILD_INTERFACE:${tree_sitter_path}/lib/include>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(tree_sitter PRIVATE ${tree_sitter_path}/lib/src)

set(cpp_tree_sitter_path ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cpp_tree_sitter STATIC
            ${cpp_tree_sitter_path}/src/cpp_tree_sitter/api.cc)
target_compile_options(cpp_tree_sitter PRIVATE -std=c++20 -fno-exceptions
                                               -fno-rtti)
target_include_directories(
  cpp_tree_sitter SYSTEM
  INTERFACE $<BUILD_INTERFACE:${cpp_tree_sitter_path}/src>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(cpp_tree_sitter
                           PRIVATE ${tree_sitter_path}/lib/include)
target_link_libraries(cpp_tree_sitter PRIVATE tree_sitter)

install(
  TARGETS cpp_tree_sitter tree_sitter
  EXPORT cpp_tree_sitter-targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

install(
  DIRECTORY ${cpp_tree_sitter_path}/src/cpp_tree_sitter
  DESTINATION include
  FILES_MATCHING
  PATTERN "**/*.h")

install(
  DIRECTORY ${tree_sitter_path}/lib/include/tree_sitter
  DESTINATION include
  FILES_MATCHING
  PATTERN "**/*.h")

install(
  EXPORT cpp_tree_sitter-targets
  FILE cpp_tree_sitter-config.cmake
  NAMESPACE cpp_tree_sitter::
  DESTINATION lib/cmake/cpp_tree_sitter)
