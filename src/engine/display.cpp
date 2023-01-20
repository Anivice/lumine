#include <lumine.h>
#include <ncurses.h>
#include <simple_err.h>
#include <csignal>

static void sig_handler(int)
{
}

void screen::init()
{
    if (!initscr())
    {
        throw simple_error_t(NCURSES_INIT_FAILED_CDX);
    }
    cbreak();
    noecho();
    curs_set(0);

    // override SIGWINCH
    if (signal(SIGWINCH, sig_handler) == SIG_ERR) {
        throw simple_error_t(SIGNAL_OVERRIDE_FAILED_CDX);
    }

    // init memory space
    clear();

    // clear screen
    update_screen();
}

screen::~screen()
{
    endwin();
}

void screen::update_screen()
{
    // resize term if resizing detected
    if (is_term_resized(LINE_LEN, COLS_LEN))
    {
        resize_term(LINE_LEN, COLS_LEN);
    }

    // display video memory
    for (int i = 0; i < LINE_LEN; i++)
    {
        move(i, 0);
        printw("%s\n", v_memory[i]);
    }

    refresh();
}

void screen::clear()
{
    for (auto & line : v_memory)
    {
        for (char & sig : line)
        {
            sig = ' ';
        }
    }
}

void screen::draw(const std::string &raw_figure, location_t starting_location)
{
    std::vector < std::string > figure;
    std::string cache;

    // convert raw_figure to AxB figure map
    for (auto i : raw_figure)
    {
        // not eol, add to cache
        if (i != '\n') {
            cache += i;
        }
        else // is eol, save to _figure
        {
            figure.emplace_back(cache);
            cache.clear();
        }
    }

    // sanity check
    if (
            !figure.empty() &&

            ((starting_location.line + figure.begin()->size() > LINE_LEN)
            || starting_location.cols + figure.size() > COLS_LEN)
        )
    {
        throw simple_error_t(INVALID_LOCATION_CDX);
    }

    // update video memory
    for (int line = 0; line < figure.size(); line++)
    {
        for (int cols = 0; cols < figure[line].size(); cols++)
        {
            v_memory
                [starting_location.line + line]
                [starting_location.cols + cols]
                    = figure[line][cols];
        }
    }
}
