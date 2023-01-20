#ifndef LUMINE_LUMINE_H
#define LUMINE_LUMINE_H

/** @file
 * This file defines interfaces for drawing objects on screen
 */

#include <vector>
#include <map>
#include <string>

#define COLS_LEN 110
#define LINE_LEN 28

/** structure containing location info */
struct location_t { int line; int cols; };

/** display content on screen according to the content in v_memory */
class screen
{
public:
    /** display contents on screen according to v_memory */
    void update_screen();

    /** clear screen */
    void clear();

    /** init ncurses and video memory */
    void init();

    /** cleanup recourses used by ncurses */
    ~screen();

    /** video memory */
    char v_memory [LINE_LEN] [COLS_LEN + 1] { };

    /** draw an entity on screen
     * @param raw_figure AxB figure map
     * @param starting_location starting location of the figure map */
    void draw(const std::string & raw_figure, location_t starting_location);
};

#endif //LUMINE_LUMINE_H
