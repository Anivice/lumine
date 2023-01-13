# add test, default file is test/*_test.cpp
function(_add_test TEST DESCRIPTION)
    set(TEST_NAME "UT_${TEST}")

    add_executable(${TEST_NAME} "tests/${TEST}_test.cpp")
    target_link_libraries(${TEST_NAME} PUBLIC ${PROJECT_NAME})

    foreach(LIB ${TEST_ESS_LIB})
        target_link_libraries(${TEST_NAME} PUBLIC ${LIB})
    endforeach()

    target_include_directories(${TEST_NAME} PUBLIC src/include)

    add_test(NAME ${TEST_NAME}
            COMMAND ${TEST_NAME}
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    message("Unit test `${TEST_NAME}`\t(${DESCRIPTION}) enabled")
endfunction()
