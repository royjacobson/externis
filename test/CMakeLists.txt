add_executable(test test.cc)
set_target_properties(
    test
    PROPERTIES
        COMPILE_FLAGS
        "-fplugin=${EXTERNIS_PLUGIN_PATH} -fplugin-arg-externis-trace=${CMAKE_CURRENT_BINARY_DIR}/trace.json"
)

# It's a bit weird, but it's convenient. This deletes the
add_custom_target(
    remove_test ALL
    COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/test
    COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/test.dir/test.cc.o
    COMMAND echo "Removed!"
    DEPENDS test
)
