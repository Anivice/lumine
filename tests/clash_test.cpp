#include <lumine.h>
#include <debug.h>
#include <fstream>
#include <ncurses.h>

int main(int argc, char ** argv)
{
    initialize_symbol_vector(argv[0]);

    screen display;
    display.init();

    display.draw(
            "TestObject1",

            "-----\n"
            " +|+ \n"
            "  V  \n",

            location_t { .line = 0, .cols = 0 }
    );

    display.draw(
            "TestObject2",

            "-----\n"
            " +|+ \n"
            "  V  \n",

            location_t { .line = 0, .cols = 1 }
    );

    display.update_screen();

    std::fstream file("clash.txt", std::ios::out);
    for (const auto& list : display.export_clash())
    {
        file << "LIST:" << std::endl;

        for (const auto & name : list)
        {
            file << '\t' << name << std::endl;
        }
    }

    getch();
    return 0;
}
