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

/** entity list */
struct entity_list_t
{
    std::string figure;
    std::map < std::string /* id */, location_t > figure_starting_locations;
};

/** display content on screen according to the content in v_memory */
class screen
{
public:
    /** video memory */
    char v_memory [LINE_LEN] [COLS_LEN + 1] { };

    /** display contents on screen according to v_memory */
    void update_screen();

    /** clear screen (fill v_memory with ' ', do NOT flash cache) */
    void clear();

    /** init ncurses and video memory */
    void init();

    /** cleanup recourses used by ncurses */
    ~screen();

    /** draw an entity on screen
     * @param raw_figure AxB figure map
     * @param starting_location starting location of the figure map */
    void draw(const std::string & raw_figure, location_t starting_location);
};

/** entity manager */
class entity_manager
{
private:
    /** entity map */
    std::map < std::string /* name */, entity_list_t > entity_map;

    /** screen display */
    screen display;

public:

    /** init screen */
    void init() { display.init(); }

    /** add entity
     * @param name entity name
     * @param raw_figure raw figure
     * @param id new entity ID */
    void add_entity(const std::string & name, const std::string & raw_figure, const std::string & id);

    /** replicate entity
     * @param name entity name
     * @param id new entity ID */
    void replicate_entity(const std::string & name, const std::string & id);

    /** move entity location
     * @param name entity name
     * @param raw_figure raw figure
     * @param id entity ID */
    void put_entity(const std::string & name, location_t location, const std::string & id);

    /** erase entity
     * @param name entity name
     * @param id entity ID */
    void erase_entity(const std::string & name, const std::string & id);

    /** update screen */
    void update_screen();
};

#endif //LUMINE_LUMINE_H
