cmake_minimum_required(VERSION 2.8)
project(SimpleXdmf)

# options
option(TEST_SIMPLE_XDMF "Test SimpleXdmf with GoogleTest." OFF)
option(BUILD_SIMPLE_XDMF_EXAMPLES "Build example uses of SimpleXdmf." OFF)

# add submodules
include_directories(${CMAKE_SOURCE_DIR}/include)

if(BUILD_SIMPLE_XDMF_EXAMPLES OR TEST_SIMPLE_XDMF)
    # -- check C++11 --
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

    if(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
    endif()

    # compiler info
    message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
    if(${DEBUG})
        message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
    else()
        message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
    endif()

endif()

if(BUILD_SIMPLE_XDMF_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TEST_SIMPLE_XDMF)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)
    include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)
    add_subdirectory(test)
endif()