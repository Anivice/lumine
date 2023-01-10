# add test, default file is test/*_test.cpp
function(add_utils UTILS)
    add_executable(${UTILS} "utils/${UTILS}.cpp")
    target_link_libraries(${UTILS} PUBLIC ${PROJECT_NAME})
    target_include_directories(${UTILS} PUBLIC src/include)

    message("Added utility `${UTILS}`")
endfunction()
