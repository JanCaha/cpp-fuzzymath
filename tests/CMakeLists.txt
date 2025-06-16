include(CTest)
enable_testing(true)

find_package(GTest REQUIRED CONFIG)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/fuzzymath
    ${CMAKE_CURRENT_BINARY_DIR}
)

macro(AddTest testname)
message(STATUS "Adding test: ${testname}")

    add_executable(${testname} "${testname}.cpp")
    target_link_libraries(${testname}
        FuzzyMath_static
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
    )
    target_include_directories(${testname}
        PUBLIC ${CMAKE_SOURCE_DIR}/src/api
    )

    add_test(NAME ${testname} COMMAND ${testname})
endmacro(AddTest)

AddTest(test_interval)
AddTest(test_fuzzy_number)
AddTest(test_alpha_cut)
# AddTest(testalphacutoperators)