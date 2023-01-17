#include <lumine.h>
#include <simple_err.h>

std::map < std::string /* name */,
        figure_t /* figure */
> entities;

void define_entity(const std::string & name, const std::string & figure)
{
    figure_t _figure;
    std::string cache;

    for (auto i : figure)
    {
        // not eol, add to cache
        if (i != '\n') {
            cache += i;
        }
        else // is eol, save to _figure
        {
            _figure.figure.emplace_back(cache);
            cache.clear();
        }
    }

    // calculate occupied space
    auto line_len = _figure.figure.begin()->length();
    auto cols_len = _figure.figure.size();

    for (int cols_i = 0; cols_i < cols_len; cols_i++)
    {
        for (int line_i = 0; line_i < line_len; line_i++)
        {
            // current location is (line_i <x>, cols_i <y>)
            if (_figure.figure[cols_i][line_i] != ' ')
            {
                _figure.occupied_space.emplace_back(location_t{.x = line_i, .y = cols_i});
            }
        }
    }

    entities.emplace(name, _figure);
}

int entity_display::draw(const std::string &figure)
{
    auto it = entities.find(figure);
    auto point = entity.find(it->first);
    if (point == entity.end())
    {
        std::vector < location_t > data;
        data.emplace_back(find_empty_space_on_screen(
                (int)it->second.figure.begin()->size(),
                (int)it->second.figure.size()
                    )
                );
        entity.emplace(it->first, data);
        return 0;
    }
    else
    {
        point->second.emplace_back(find_empty_space_on_screen(
                (int)it->second.figure.begin()->size(),
                (int)it->second.figure.size()
                    )
                );
        return (int)point->second.size();
    }
}

void entity_display::move(const std::string &figure, location_t offset)
{

}

location_t entity_display::find_empty_space_on_screen(int line, int cols)
{
    auto is_spot_empty = [&](int org_line, int org_cols,
                             int tag_line_size, int tag_cols_size)->bool
    {
        for (int line_off = 0; line_off < tag_line_size; line_off++)
        {
            for (int cols_off = 0; cols_off < tag_cols_size; cols_off++)
            {
                if (screen_display.v_memory[org_line + line_off][org_cols + cols_off] == ' ')
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }

        return true;
    };

    for (int line_off = 0; line_off < LINE_LEN - line; line_off++)
    {
        for (int cols_off = 0; cols_off < COLS_LEN - cols; cols_off++)
        {
            if (is_spot_empty(line_off, cols_off, line, cols))
            {
                return location_t {.x = line_off, .y = cols_off};
            }
        }
    }

    throw simple_error_t(NO_SPACE_LEFT_ON_SCRREN_CDX);
}

void entity_display::update()
{

}

void entity_display::erase(const std::string &figure, int)
{

}
