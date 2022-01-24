#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <assert.h>

class Tile
{
public:
    Tile():Tile(0, 0) {}
    Tile(int in_x, int in_y) : x(in_x), y(in_y) {}
    char character = '.';
    bool is_border(int width, int height) 
    {
        if (x == 0 || x == (width-1) )
            return true;
        if (y == 0 || y == (height-1) )
            return true;

        return false;
    }

    int x = 0;
    int y = 0;
};

class Map
{
public:
    Map() = delete;
    Map(int size_x, int size_y) : width(size_x), height(size_y), tiles(size_x * size_y) 
    {
        size_t tile_num = tiles.size();
        for (size_t i = 0; i < tile_num; i++)
        {
            tiles[i].x = static_cast<int>(i%width);
            tiles[i].y = static_cast<int>(i/width);
        }
    }

    Tile& get_tile(const int x, const int y)
    { 
        assert(width >= x);
        assert(height >= y);

        return tiles[x + width*y];
    };

    void print()
    {
        size_t tile_num = tiles.size();
        for (size_t i = 0; i < tile_num; i++)
        {
            std::cout << tiles[i].character;
            if ((i+1) % width == 0)
                std::cout << '\n';
        }
    }

    void draw_border()
    {
        for (Tile& tile : tiles)
        {
            if (tile.is_border(width, height))
                tile.character = '#';
        }
    }

    std::vector<Tile> tiles;

    int width;
    int height;
};

struct Room
{
    int cp_x;
    int cp_y;
    int width;
    int height;

    void clamp_size(int map_width, int map_height)
    {
        width = std::clamp(cp_x + width, 0, map_width-1);
        height = std::clamp(cp_y + height, 0, map_height-1);
    }
};

class BSPDungeon
{
    BSPDungeon(Map& map);
    void Generate()
    {

    };
    std::vector<Room> rooms;
    std::vector<std::shared_ptr<Tile>> walkable_floors;
};

int main(int, char**) {

    Map map(80,25);

    map.draw_border();

    map.get_tile(79, 24).character = '$';
    map.print();

    return 0;
}
