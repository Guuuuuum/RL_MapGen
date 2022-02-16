#pragma once

#include <vector>
#include <assert.h>
#include <functional>

#include "level_generation.h"

class Tile
{
public:
    Tile():Tile(v2(0, 0)) {}
    Tile(v2 in_pos) : pos(in_pos) {}
    char character = '.';

    v2 pos;
};

class Map
{
private:
    std::vector<Tile> tiles;
public:
    const v2 size;
    std::vector<Tile>& get_tiles() {return tiles;}
    Map() = delete;
    Map(v2 in_size) : size(in_size), tiles(in_size.x * in_size.y) 
    {
        size_t tile_num = tiles.size();
        for (size_t i = 0; i < tile_num; i++)
        {
            tiles[i].pos.x = static_cast<int>(i%size.x);
            tiles[i].pos.y = static_cast<int>(i/size.x);
        }
    }

    Room as_room()
    {
        return Room(v2(0,0), size);
    }

    Tile& get_tile(const v2 pos)
    {
        return get_tile(pos.x, pos.y);
    };
    
    Tile& get_tile(const int x, const int y)
    { 
        assert(size.x >= x || x > 0);
        assert(size.y >= y || y > 0);

        return tiles[x + size.x*y];
    };

    template <typename T>
    T& get_by_coord(std::vector<T>& map, const v2 room_size, const v2 pos)
    {
        assert(room_size.x >= pos.x || pos.x > 0);
        assert(room_size.y >= pos.y || pos.y > 0);

        return map[pos.x + room_size.x*pos.y];
    }

    template <typename T>
    bool try_call_in_bound(std::vector<T>& map, const Room room, const v2 pos, std::function<void()> func)
    {
        if (room.in_bounds(pos))
        {
            func();
            return true;
        }

        return false;
    }

    void print()
    {
        size_t tile_num = tiles.size();
        for (size_t i = 0; i < tile_num; i++)
        {
            std::cout << tiles[i].character;
            if ((i+1) % size.x == 0)
                std::cout << '\n';
        }
    }

    bool is_border(v2 pos) 
    {
        if (pos.x == 0 || pos.x == (size.x-1) )
            return true;
        if (pos.y == 0 || pos.y == (size.y-1) )
            return true;

        return false;
    }

    bool in_bounds(const v2 pos) const
    {
        if (pos.x < 0 || pos.x >= size.x )
            return false;
        if (pos.y < 0 || pos.y >= size.y )
            return false;

        return true;
    }

    std::vector<v2> get_border()
    {
        std::vector<v2> border;
        border.reserve(( (size.x*2) + (size.y*2) - 4));

        const int x_max = size.x - 1;
        const int y_max = size.y - 1;

        for (int i = 0; i < size.x; i++)
        {
            border.emplace_back(i, 0);
            border.emplace_back(i, y_max);
        }
            
        for (int i = 1; i < size.y - 1; i++)
        {
            border.emplace_back(0, i);
            border.emplace_back(x_max, i);
        }

        return border;
    }
};