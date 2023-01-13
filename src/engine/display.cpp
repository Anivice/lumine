#include <lumine.h>
#include <ncurses.h>
#include <simple_err.h>
#include <unistd.h>
#include <thread>

void screen::display_thread()
{
    while (!_exit)
    {
        for (int i = 0; i < LINE_LEN; i++) {
            move(i, 0);
            printw("%s\n", v_memory[i]);
        }

        refresh();

        usleep(1000/30);
    }

    _is_exited = true;
}

screen::screen()
{
    if (!initscr())
    {
        throw simple_error_t(NCURSES_INIT_FAILED_CDX);
    }
    cbreak();
    noecho();
    curs_set(0);

    // init memory space
    for (auto & line : v_memory)
    {
        for (char & sig : line)
        {
            sig = ' ';
        }
    }

    detach();
}

screen::~screen()
{
    _exit = true;
    while (!_is_exited);
    endwin();
}

void screen::detach()
{
    std::thread display(&screen::display_thread, this);
    display.detach();
}
