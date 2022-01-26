#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <queue>

#include "directions.h"
#include "v2.h"
#include "map.h"
#include "astar.h"

struct Room
{
    v2 pos;
    v2 size;

    bool is_passable = true;

    void clamp_size(v2 map_size)
    {
        size.x = std::clamp(pos.x + size.x, 0, map_size.x-1);
        size.y = std::clamp(pos.y + size.y, 0, map_size.y-1);
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

    RLUtil::AStar astar(map);
    std::vector<v2> way = astar.draw_way(v2(1, 1), v2(78, 23));
    for (v2 pos : way)
    {
        map.get_tile(pos).character = '!';
    }
    
    
    map.print();

    return 0;
}
