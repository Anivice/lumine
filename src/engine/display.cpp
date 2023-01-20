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
    for (int line = 0; line < LINE_LEN; line++)
    {
        std::vector < std::string > null;
        std::vector < std::vector < std::string > > empty_line;
        for (int cols = 0; cols < COLS_LEN; cols++)
        {
            empty_line.emplace_back(null);
        }
        detailed_memory_view.emplace_back(empty_line);
    }

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

    for (int line = 0; line < LINE_LEN; line++)
    {
        for (int cols = 0; cols < COLS_LEN; cols++)
        {
            detailed_memory_view[line][cols].clear();
        }
    }
}

void screen::draw(const std::string & name, const std::string &raw_figure, location_t starting_location)
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
            if (figure[line][cols] != ' ')
            {
                detailed_memory_view
                    [starting_location.line + line]
                    [starting_location.cols + cols].emplace_back(name);

                v_memory
                    [starting_location.line + line]
                    [starting_location.cols + cols]
                        = figure[line][cols];
            }
        }
    }
}

std::list<std::vector<std::string> > screen::export_clash()
{
    std::list<std::vector<std::string> > ret;

    for (const auto & line : detailed_memory_view)
    {
        for (const auto & cols : line)
        {
            if (cols.size() > 1)
            {
                ret.emplace_back(cols);
            }
        }
    }

    ret.unique();
    return ret;
}
