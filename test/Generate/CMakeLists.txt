set(GENERATE_TEST_SRCS
    "generate_test.cpp"
)

add_executable(generate_test ${TEST_FIXTURE_SRC} ${GENERATE_TEST_SRCS})
target_link_libraries(generate_test gtest gtest_main)

add_test(
    NAME Generate
    COMMAND $<TARGET_FILE:generate_test>
)