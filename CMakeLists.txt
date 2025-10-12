cmake_minimum_required(VERSION 3.12)
project(externis)

option(EXTERNIS_BUILD_TEST "Build the compiler plugin test" ON)
option(EXTERNIS_INSTALL_TO_PLUGIN_DIR "Install Externis into the GCC plugin directory" ON)

if(NOT EXTERNIS_GCC_PLUGIN_DIR)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=plugin
        OUTPUT_VARIABLE EXTERNIS_GCC_PLUGIN_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if(NOT EXISTS ${EXTERNIS_GCC_PLUGIN_DIR}/include/gcc-plugin.h)
    message(FATAL_ERROR "gcc-plugin.h not found under ${EXTERNIS_GCC_PLUGIN_DIR}/include")
endif()

message("GCC plugin headers found in " ${EXTERNIS_GCC_PLUGIN_DIR})

add_library(externis SHARED externis.cc tracking.cc output.cc)
target_include_directories(externis PRIVATE ${EXTERNIS_GCC_PLUGIN_DIR}/include)

# Optional, useful for debugging.
find_package(fmt)
if (fmt_FOUND)
    target_link_libraries(externis PRIVATE fmt::fmt)
endif()

set_target_properties(externis PROPERTIES CXX_STANDARD 20)
set_target_properties(externis PROPERTIES COMPILE_FLAGS "-fno-rtti -g -Wall")
set_target_properties(externis PROPERTIES PREFIX "" OUTPUT_NAME "externis")

set(EXTERNIS_PLUGIN_PATH ${CMAKE_BINARY_DIR}/externis.so)

if (EXTERNIS_INSTALL_TO_PLUGIN_DIR)
    install(TARGETS externis DESTINATION ${EXTERNIS_GCC_PLUGIN_DIR})
else()
    install(TARGETS externis)
endif()

if(EXTERNIS_BUILD_TEST)
    add_subdirectory(test)

    # When building with Ninja, the dependency tree for add_dependencies(test externis) sets up a
    # rule the link step for test must run after the link step for externis; but that doesn't work
    # because the compilation steps for test require externis
    #
    # However, using the custom target that depends on externis.so fails with GNU Make, because
    # recursive GNU make doesn't have the knowledge to build files cross-directory; but using
    # add_dependencies orders things correctly with GNU Make
    if(CMAKE_GENERATOR STREQUAL "Ninja")
        add_custom_target(wait_for_externis DEPENDS $<TARGET_FILE:externis>)
        add_dependencies(test wait_for_externis)
    else()
        add_dependencies(test externis)
    endif()
endif()
