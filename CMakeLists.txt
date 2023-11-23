cmake_minimum_required(VERSION 3.24.2)

set(CMAKE_C_COMPILER "/usr/local/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++")

project(ASCLANG)
file(GLOB SOURCES "src/*.hpp" "src/*.cpp")

# Guess cmake paths for Clang and LLVM
get_filename_component(CLANG_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
set(CLANG_BASE ${CLANG_BIN_DIR}/..)
message(STATUS "CLANG_BASE:" ${CLANG_BASE})
message(STATUS "CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})

set(CLANG_CMAKE_DIR ${CLANG_BASE}/lib/cmake/clang)
set(LLVM_CMAKE_DIR ${CLANG_BASE}lib/cmake/llvm)
include("${CLANG_CMAKE_DIR}/ClangConfig.cmake")
include("${LLVM_CMAKE_DIR}/LLVMConfig.cmake")

include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})
message(STATUS "CLANG INCLUDE PATH:" ${CLANG_INCLUDE_DIRS})
message(STATUS "LLVM INCLUDE PATH:" ${LLVM_INCLUDE_DIRS})

set(LocalLibClang_DEFINITIONS -fPIC -fvisibility-inlines-hidden
  -Wall -Wno-unused-parameter -Wwrite-strings
  -Wcast-qual -Wmissing-field-initializers
  -pedantic -Wno-long-long -Wcovered-switch-default
  -Wnon-virtual-dtor -Wdelete-non-virtual-dtor
  -Werror=date-time -fno-exceptions -fno-rtti
  -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)
add_definitions(${LocalLibClang_DEFINITIONS})



add_executable(ASCLANG ${SOURCES})
set_property(TARGET ASCLANG PROPERTY CXX_STANDARD 17)
target_include_directories(ASCLANG PUBLIC "/usr/local/include")
target_link_directories(ASCLANG PUBLIC "/usr/local/lib")
target_link_libraries(ASCLANG PUBLIC clangAST clangBasic clangFrontend clangIndex clangRewrite clangTooling clangAST
  clangDriver
  clangRewriteFrontend
  clangSerialization
  clangStaticAnalyzerFrontend
  clangToolingSyntax)
