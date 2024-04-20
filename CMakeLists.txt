cmake_minimum_required(VERSION 3.28)
project(cpp_tree_sitter VERSION 0.2.4)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TREE_SITTER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tree-sitter-0.22.2)

add_library(tree_sitter STATIC ${TREE_SITTER_PATH}/lib/src/lib.c)
target_compile_options(tree_sitter PRIVATE -std=c17 -fno-exceptions)
target_include_directories(
  tree_sitter
  PUBLIC $<BUILD_INTERFACE:${TREE_SITTER_PATH}/lib/include>
         $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(tree_sitter PRIVATE ${TREE_SITTER_PATH}/lib/src)

set(cpp_TREE_SITTER_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cpp_tree_sitter STATIC
            ${cpp_TREE_SITTER_PATH}/src/cpp_tree_sitter/api.cc)
target_compile_options(cpp_tree_sitter PRIVATE -std=c++20 -fno-exceptions
                                               -fno-rtti)
target_include_directories(
  cpp_tree_sitter
  PUBLIC $<BUILD_INTERFACE:${cpp_TREE_SITTER_PATH}/src>
         $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(cpp_tree_sitter
                           PRIVATE ${TREE_SITTER_PATH}/lib/include)
target_link_libraries(cpp_tree_sitter PRIVATE tree_sitter)

install(
  TARGETS cpp_tree_sitter tree_sitter
  EXPORT cpp_tree_sitter-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  DIRECTORY ${cpp_TREE_SITTER_PATH}/src/cpp_tree_sitter
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "**/*.h")

install(
  DIRECTORY ${TREE_SITTER_PATH}/lib/include/tree_sitter
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "**/*.h")

install(
  EXPORT cpp_tree_sitter-targets
  FILE cpp_tree_sitter-config.cmake
  NAMESPACE cpp_tree_sitter::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_tree_sitter)
