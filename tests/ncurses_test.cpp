#include <lumine.h>
#include <debug.h>
#include <ncurses.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    initialize_symbol_vector(argv[0]);

    screen screen;
    screen.init();

    // clear screen
    screen.update_screen();

    screen.v_memory[1][0] = ' ';
    screen.v_memory[1][1] = 'H';
    screen.v_memory[1][2] = 'e';
    screen.v_memory[1][3] = 'l';
    screen.v_memory[1][4] = 'l';
    screen.v_memory[1][5] = 'o';

    screen.update_screen();

    getch();

    screen.v_memory[1][0] = ' ';
    screen.v_memory[1][1] = 'W';
    screen.v_memory[1][2] = 'o';
    screen.v_memory[1][3] = 'r';
    screen.v_memory[1][4] = 'l';
    screen.v_memory[1][5] = 'd';

    screen.update_screen();

    getch();

    return 0;
}
