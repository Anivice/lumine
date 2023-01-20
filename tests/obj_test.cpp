#include <lumine.h>
#include <debug.h>
#include <iostream>

int main(int argc, char ** argv)
{
    initialize_symbol_vector(argv[0]);

    define_entity("TestObject",
                  "  ^  \n"
                  " +|+ \n"
                  "-----\n");

    entity_display display;
    display.screen_display.clear();

    display.screen_display.v_memory[2][3] = 's';
    display.screen_display.v_memory[6][8] = 's';
    auto loc = display.find_empty_space_on_screen(10, 10);
    std::cout << loc.x << "x" << loc.y << std::endl;

    return 0;
}
