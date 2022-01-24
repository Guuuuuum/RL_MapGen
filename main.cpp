#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <queue>

struct v2
{
    v2() : v2(0, 0){};
    v2(int in_x, int in_y) : x(in_x), y(in_y){};
    int x;
    int y;

	v2 operator-() const
    {
        return v2(-x, -y);
    };
	v2 operator+(const v2& rh) const
    {
        return v2(x + rh.x, y + rh.y);
    };
	v2 operator-(const v2& rh) const
    {
        return v2(x - rh.x, y - rh.y);
    };
};

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

struct Room
{
    v2 pos;
    v2 size;

    void clamp_size(v2 map_size)
    {
        size.x = std::clamp(pos.x + size.x, 0, map_size.x-1);
        size.y = std::clamp(pos.y + size.y, 0, map_size.y-1);
    }
};

class AStar
{
public:
    AStar(Map& in_map) : map(in_map) {}
    Map& map;

    std::priority_queue<Room, std::vector<Room>> set;
    
    void draw_way(v2 start, v2 end)
    {

    }
};

class BSPDungeon
{
    BSPDungeon(Map& in_map) : map(in_map) {}
    Map& map;
    std::vector<Room> rooms;
    std::vector<std::shared_ptr<Tile>> walkable_floors;

    void Generate()
    {

    };
};

int main(int, char**) 
{

    Map map(v2(80,25));
    map.draw_border();

    AStar astar(map);
    astar.draw_way(v2(0, 0), v2(79, 24));
    
    map.print();

    return 0;
}
