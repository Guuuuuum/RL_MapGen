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

class Room
{
public:
    Room(v2 in_pos, v2 in_size) : pos(in_pos), size(in_size) {};
    v2 pos; //pivot is left-up.
    v2 size;

    static const v2 MIN_SIZE;
    void clamp_size(v2 map_size)
    {
        size.x = std::clamp(pos.x + size.x, 0, map_size.x-1);
        size.y = std::clamp(pos.y + size.y, 0, map_size.y-1);
    }
};

inline const v2 Room::MIN_SIZE = v2(4, 4);

class BSPDungeon
{
public:
    BSPDungeon(Map& in_map) : map(in_map) {}
    Map& map;
private:
    std::vector<Room> rooms;
    std::vector<std::shared_ptr<Tile>> walkable_floors;

public:
    void generate(Room in_room, const int depth)
    {
        rooms.reserve( static_cast<size_t>(std::powl(2, depth)) );
        std::vector<Room> div_rooms = devide_room(in_room);
        rooms.insert(rooms.end(), div_rooms.begin(), div_rooms.end() );
    };
private:
    std::vector<Room> devide_room(Room room)
    {
        std::vector<Room> div_rooms;

        if (room.size < Room::MIN_SIZE)
        {
            div_rooms.push_back(room);
            return div_rooms;
        }

        return div_rooms;
    };
};

int main(int, char**) 
{
    Map map(v2(80,25));
    map.draw_border();

    BSPDungeon bspmap(map);
    bspmap.generate({ {0, 0}, map.size}, 3);

    RLUtil::AStar astar(map);
    std::vector<v2> way = astar.draw_way(v2(1, 1), v2(78, 23));
    for (v2 pos : way)
    {
        map.get_tile(pos).character = '!';
    }
    
    
    map.print();

    return 0;
}
