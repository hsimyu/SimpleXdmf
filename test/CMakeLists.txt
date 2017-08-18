file(GLOB TEST_FIXTURE_SRC test_fixture.cpp)
message(STATUS "TEST_FIXTURE = ${TEST_FIXTURE_SRC}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TEST_SRC
    "Generate"
)

subdirs(${TEST_SRC})