#include <lumine.h>
#include <debug.h>
#include <iostream>
#include <ncurses.h>
#include <simple_err.h>

int main(int argc, char ** argv)
{
    initialize_symbol_vector(argv[0]);

    entity_manager manager;
    manager.init();

    ///////////////////////////////////////////////////

    manager.add_entity(
            "TestObject",

            "-----\n"
            " +|+ \n"
            "  V  \n",

            "0"
    );
    manager.update_screen();
    getch();

    ///////////////////////////////////////////////////

    manager.replicate_entity("TestObject", "1");
    manager.put_entity("TestObject", location_t {.line = 10, .cols = 0}, "0");

    manager.update_screen();
    getch();

    ///////////////////////////////////////////////////

    manager.erase_entity("TestObject", "1");

    manager.update_screen();
    getch();

    manager.erase_entity("TestObject", "0");

    manager.update_screen();
    getch();

    return 0;
}
