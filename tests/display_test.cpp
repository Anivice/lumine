#include <lumine.h>
#include <debug.h>
#include <iostream>
#include <ncurses.h>

int main(int argc, char ** argv)
{
    initialize_symbol_vector(argv[0]);

    screen display;
    display.init();

    display.draw(
            "-----\n"
            " +|+ \n"
            "  V  \n",

            location_t { .line = 0, .cols = 0 }
            );

    display.update_screen();
    getch();
    return 0;
}
