cmake_minimum_required (VERSION 3.12 FATAL_ERROR)

find_package(Catch2 REQUIRED)

enable_testing()

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_compile_options(-Wextra)
endif()

# for convenience, make a library that depends on everything so each
# separate test can be as small as possible
add_library(test_common STATIC
            catch2_main.cpp
    )
set_target_properties(test_common PROPERTIES CXX_STANDARD 20)
set_target_properties(test_common PROPERTIES CXX_STANDARD_REQUIRED ON)
target_link_libraries(test_common PUBLIC Catch2::Catch2)

macro(create_test testname)
    add_executable(test_${testname} test_${testname}.cpp)
    set_target_properties(test_${testname} PROPERTIES CXX_STANDARD 20)
    set_target_properties(test_${testname} PROPERTIES CXX_STANDARD_REQUIRED ON)
    add_test(NAME test_${testname} COMMAND test_${testname})
    target_link_libraries(test_${testname} PRIVATE
        test_common
    )
endmacro()

create_test(simple)
