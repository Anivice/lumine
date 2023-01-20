#include <lumine.h>
#include <debug.h>
#include <iostream>
#include <ncurses.h>
#include <simple_err.h>

int main(int argc, char ** argv) {
    initialize_symbol_vector(argv[0]);

    entity_manager manager;

    /****************************
     * Add entity
     */

    manager.add_entity(
            "TestObject",

            "-----\n"
            " +|+ \n"
            "  V  \n",

            "0"
    );

    /****************************
     * Add entity when it already exists
     */

    try
    {
        manager.add_entity(
                "TestObject",

                "-----\n"
                " +|+ \n"
                "  V  \n",

                "0"
        );
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_ALREADY_EXISTS_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * replicate entity
    */

    manager.replicate_entity("TestObject", "1");

    /****************************
    * replicate entity when it already exists
    */

    try
    {
        manager.replicate_entity("TestObject", "1");
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_ALREADY_EXISTS_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * replicate entity when it doesn't exists
    */

    try
    {
        manager.replicate_entity("NotTestObject", "2");
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_NOT_FOUND_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * put entity when it doesn't exists #1
    */

    try
    {
        manager.put_entity("NotTestObject", location_t {.line = 1, .cols = 0}, "0");
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_NOT_FOUND_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * put entity when it doesn't exists #2
    */

    try
    {
        manager.put_entity("TestObject", location_t {.line = 1, .cols = 0}, "3");
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_NOT_FOUND_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * put entity
    */

    manager.put_entity("TestObject", location_t {.line = 1, .cols = 0}, "0");

    /****************************
    * erase entity when it doesn't exists #1
    */

    try
    {
        manager.erase_entity("NotTestObject", "0");
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_NOT_FOUND_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * erase entity when it doesn't exists #2
    */

    try
    {
        manager.erase_entity("TestObject", "3");
    }
    catch (simple_error_t & err)
    {
        if (err.get_err_code() != OBJ_IN_QUERY_NOT_FOUND_CDX)
        {
            return EXIT_FAILURE;
        }
    }

    /****************************
    * erase entity
    */

    manager.erase_entity("TestObject", "1");
    manager.erase_entity("TestObject", "0");

    return 0;
}
