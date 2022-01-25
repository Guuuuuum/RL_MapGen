#pragma once

#include <vector>
#include "v2.h"

class Tile
{
public:
    Tile():Tile(v2(0, 0)) {}
    Tile(v2 in_pos) : pos(in_pos) {}
    char character = '.';
    bool is_border(v2 size) 
    {
        if (pos.x == 0 || pos.x == (size.x-1) )
            return true;
        if (pos.y == 0 || pos.y == (size.y-1) )
            return true;

        return false;
    }

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

    Tile& get_tile(const int x, const int y)
    { 
        assert(size.x >= x);
        assert(size.x >= y);

        return tiles[x + size.x*y];
    };

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

    void draw_border()
    {
        for (Tile& tile : tiles)
        {
            if (tile.is_border(size))
                tile.character = '#';
        }
    }

    std::vector<Tile> tiles;

    v2 size;
};
