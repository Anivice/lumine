#include <lumine.h>
#include <simple_err.h>

void entity_manager::add_entity(const std::string &name, const std::string &raw_figure,
                                const std::string & id)
{
    std::map < std::string, location_t > figure_starting_location;
    figure_starting_location.emplace(id, location_t {.line = 0, .cols = 0});
    auto it = entity_map.find(name);

    if (it == entity_map.end())
    {
        // if not found
        entity_map.emplace(name, entity_list_t{
                .figure = raw_figure,
                .figure_starting_locations = figure_starting_location
        });
    }
    else
    {
        // found
        throw simple_error_t(OBJ_IN_QUERY_ALREADY_EXISTS_CDX);
    }
}

void entity_manager::replicate_entity(const std::string &name, const std::string &id)
{
    std::map < std::string, location_t > figure_starting_location;
    figure_starting_location.emplace(id, location_t {.line = 0, .cols = 0});
    auto it = entity_map.find(name);
    if (it == entity_map.end())
    {
        throw simple_error_t(OBJ_IN_QUERY_NOT_FOUND_CDX);
    }

    auto it_figure = it->second.figure_starting_locations.find(id);
    if (it_figure != it->second.figure_starting_locations.end())
    {
        throw simple_error_t(OBJ_IN_QUERY_ALREADY_EXISTS_CDX);
    }

    it->second.figure_starting_locations.emplace(id, location_t {.line = 0, .cols = 0});
}

void entity_manager::put_entity(const std::string &name, location_t location,
                                const std::string & id)
{
    auto it = entity_map.find(name);
    if (it == entity_map.end())
    {
        throw simple_error_t(OBJ_IN_QUERY_NOT_FOUND_CDX);
    }

    auto it_figure = it->second.figure_starting_locations.find(id);
    if (it_figure == it->second.figure_starting_locations.end())
    {
        throw simple_error_t(OBJ_IN_QUERY_NOT_FOUND_CDX);
    }

    it->second.figure_starting_locations.at(id) = location;
}

void entity_manager::erase_entity(const std::string &name, const std::string & id)
{
    auto it = entity_map.find(name);
    if (it == entity_map.end())
    {
        throw simple_error_t(OBJ_IN_QUERY_NOT_FOUND_CDX);
    }

    auto it_figure = it->second.figure_starting_locations.find(id);
    if (it_figure == it->second.figure_starting_locations.end())
    {
        throw simple_error_t(OBJ_IN_QUERY_NOT_FOUND_CDX);
    }

    it->second.figure_starting_locations.erase(id);
    if (it->second.figure_starting_locations.empty())
    {
        entity_map.erase(it);
    }
}

void entity_manager::update_screen()
{
    display.clear();

    for (auto & entity_list : entity_map)
    {
        for (const auto& loc : entity_list.second.figure_starting_locations)
        {
            display.draw(entity_list.second.figure, loc.second);
        }
    }

    display.update_screen();
}

