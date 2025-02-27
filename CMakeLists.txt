cmake_minimum_required(VERSION 3.12)
project(externis)

if(NOT GCC_PLUGIN_INCLUDE)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=plugin
        OUTPUT_VARIABLE GCC_PLUGIN_INCLUDE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if(NOT EXISTS ${GCC_PLUGIN_INCLUDE}/include/gcc-plugin.h)
    message(FATAL_ERROR "gcc-plugin.h not found under ${GCC_PLUGIN_INCLUDE}/include")
endif()

message("GCC plugin headers found in " ${GCC_PLUGIN_INCLUDE})

add_library(externis SHARED externis.cc tracking.cc output.cc)
target_include_directories(externis PRIVATE ${GCC_PLUGIN_INCLUDE}/include)

# Optional, useful for debugging.
find_package(fmt)
if (fmt_FOUND)
    target_link_libraries(externis PRIVATE fmt::fmt)
endif()

set_target_properties(externis PROPERTIES CXX_STANDARD 20)
set_target_properties(externis PROPERTIES COMPILE_FLAGS "-fno-rtti -g -Wall")
set_target_properties(externis PROPERTIES PREFIX "" OUTPUT_NAME "externis")

set(EXTERNIS_PLUGIN_PATH ${CMAKE_BINARY_DIR}/externis.so)
install(TARGETS externis DESTINATION ${GCC_PLUGIN_INCLUDE})

add_subdirectory(test)

add_custom_target(wait_for_externis DEPENDS ${EXTERNIS_PLUGIN_PATH})
add_dependencies(test wait_for_externis)
