# Arbitrary - We probably do not need any fancy functionality
cmake_minimum_required(VERSION 3.1)
project(windows_system_error)

set(CMAKE_CXX_STANDARD 17)

# Build library
add_library(windows_system_error
        src/win_error.cpp
        src/win_errc.cpp
        src/win_error_category.cpp
        )
target_include_directories(windows_system_error
        PUBLIC
            include
        # PRIVATE
            src/private
        )

# Build tests
add_subdirectory(dependencies/Catch2)
add_executable(test_win_error
        test/testmain.cpp
        test/test_win_error.cpp
        )
target_link_libraries(test_win_error Catch2 windows_system_error)