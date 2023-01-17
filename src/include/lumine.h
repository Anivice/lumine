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
struct location_t { int x; int y; };

/** structure for a figure */
struct figure_t
{
    /* figure must be a AxA type string vector, empty spaces should be filled with ' ' */
    std::vector < std::string > figure;
    std::vector < location_t > occupied_space;
};
/** map for a list of all figures that will be used for drawing process */
extern std::map < std::string /* name */,
        figure_t /* figure */
> entities;

/** define an entity
 * @param name      entity name
 * @param figure    entity figure */
void define_entity(const std::string & name, const std::string & figure);

/** display content on screen according to the content in v_memory */
class screen
{
public:
    /** display contents on screen according to v_memory */
    void display();

    /** clear screen */
    void clear();

    /** init ncurses and video memory */
    void init();

    /** cleanup recourses used by ncurses */
    ~screen();

    /** video memory */
    char v_memory [LINE_LEN] [COLS_LEN + 1] { };
};

/** display entities on screen */
struct entity_display
{
public:
    /** screen */
    screen screen_display;

    /** entity list, containing starting locations for entities */
    std::map < std::string /* figure */,
            std::vector < location_t > /* offset */ > entity;

    /** find a starting point of an empty spot with the size of line * cols
     * @param line  length of line
     * @param cols  length of colum
     * @return starting point of the empty spot */
    location_t find_empty_space_on_screen(int line, int cols);

    void update();

    int draw(const std::string & figure);
    void move(const std::string & figure, location_t offset);
    void erase(const std::string & figure, int);
};

#endif //LUMINE_LUMINE_H
