#pragma once

#include <vector>
#include "v2.h"

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
public:
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

    Tile& get_tile(const v2 pos)
    {
        return get_tile(pos.x, pos.y);
    };
    Tile& get_tile(const int x, const int y)
    { 
        assert(size.x >= x || x < 0);
        assert(size.y >= y || y < 0);

        return tiles[x + size.x*y];
    };

    template <typename T>
    T& get_by_coord(std::vector<T>& map, v2 coord)
    {
        assert(size.x >= coord.x || coord.x < 0);
        assert(size.y >= coord.y || coord.y < 0);

        return map[coord.x + size.x*coord.y];
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

    bool in_bounds(v2 pos) 
    {
        if (pos.x < 0 || pos.x >= size.x )
            return false;
        if (pos.y < 0 || pos.y >= size.y )
            return false;

        return true;
    }

    void draw_border()
    {
        for (Tile& tile : tiles)
        {
            if (is_border(tile.pos))
                tile.character = '#';
        }
    }

    std::vector<Tile> tiles;

    v2 size;
};