file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

tpqt4_setup_dbus_test_environment()

# Make check target. In case of check, output on failure and put it into a log
# This target has to stay here for catching all of the tests
add_custom_target(check ctest --output-on-failure -O test.log
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_dependencies(check check-local ${tpqt4_test_cases})
