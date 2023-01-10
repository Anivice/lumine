# default marco definition. set marco to default value is not override manually
function(default_definition DEFINITION DEFAULT_VAL)
    # if definition not predefined
    if ("${${DEFINITION}}" STREQUAL "")
        add_compile_definitions("${DEFINITION}=${DEFAULT_VAL}")
    endif()
endfunction()
