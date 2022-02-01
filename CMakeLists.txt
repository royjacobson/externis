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
install(TARGETS externis DESTINATION ${GCC_PLUGIN_INCLUDE})


add_executable(test test.cc)
set_target_properties(test PROPERTIES COMPILE_FLAGS "-fplugin=${CMAKE_BINARY_DIR}/externis.so -fplugin-arg-externis-trace=${CMAKE_SOURCE_DIR}/trace.json")
add_dependencies(test externis)

add_custom_target(
    remove_test ALL
    COMMAND rm ${CMAKE_BINARY_DIR}/test 
    COMMAND rm CMakeFiles/test.dir/test.cc.o
    COMMAND echo "Removed!"
    DEPENDS test
)
