cmake_minimum_required(VERSION 3.12)
project(externis)

exec_program(gcc ARGS "-print-file-name=plugin" OUTPUT_VARIABLE GCC_PLUGIN_INCLUDE)

message("GCC plugin headers found in " ${GCC_PLUGIN_INCLUDE})

find_package(fmt)

add_library(externis SHARED externis.cc tracking.cc output.cc)
target_include_directories(externis PRIVATE ${GCC_PLUGIN_INCLUDE}/include)
target_link_libraries(externis PRIVATE fmt::fmt)
set_target_properties(externis PROPERTIES CXX_STANDARD 20)
set_target_properties(externis PROPERTIES COMPILE_FLAGS "-fno-rtti -g -Wall")
set_target_properties(externis PROPERTIES PREFIX "" OUTPUT_NAME "externis")
set(EXTERNIS_PLUGIN_PATH ${CMAKE_BINARY_DIR}/externis.so)
install(TARGETS externis DESTINATION ${GCC_PLUGIN_INCLUDE})

add_subdirectory(test)
