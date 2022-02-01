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

        std::vector<Room> bsp_spaces;
        divide_room(bsp_spaces, in_room, depth);

        // draw random size rooms inside bsp spaces 
        for (auto space : bsp_spaces)
        {
            for (int i = 0; i < space.size.x; i++)
            {
                map.get_tile(space.pos + v2(i, 0)).character = 'J';
            }
            for (int i = 1; i < space.size.y; i++)
            {
                map.get_tile(space.pos + v2(0, i)).character = 'J';
            }
        }
        
    };
private:
    void divide_room(std::vector<Room>& spaces, Room room, int depth)
    {
        if (depth == 0 || room.size < Room::MIN_SIZE)
        {
            spaces.emplace_back(room);
            return;
        }

        // cut by width/height ratio. it looks better
        if (room.size.x > static_cast<float>(room.size.y) * 2.0f )
        {
            int hf_width = room.size.x/2;
            
            divide_room(spaces, Room(room.pos, v2(hf_width, room.size.y)), depth - 1);

            if (room.pos.x % 2 != 0)
                divide_room(spaces, Room(room.pos + v2(hf_width, 0), v2(hf_width + 1, room.size.y)), depth - 1);
            else
                divide_room(spaces, Room(room.pos + v2(hf_width, 0), v2(hf_width, room.size.y)), depth - 1);
        }
        else
        {
            int hf_height = room.size.y/2;
            
            divide_room(spaces, Room(room.pos, v2(room.size.x, hf_height)), depth - 1);

            if (room.pos.x % 2 != 0)
                divide_room(spaces, Room(room.pos + v2(0, hf_height), v2(room.size.x, hf_height + 1)), depth - 1);
            else
                divide_room(spaces, Room(room.pos + v2(0, hf_height), v2(room.size.x, hf_height)), depth - 1);
        }
    };
};

int main(int, char**) 
{
    Map map(v2(80,25));
    map.draw_border();

    BSPDungeon bspmap(map);
    bspmap.generate({ {0, 0}, map.size}, 3);

    // RLUtil::AStar astar(map);
    // std::vector<v2> way = astar.draw_way(v2(1, 1), v2(78, 23));
    // for (v2 pos : way)
    // {
    //     map.get_tile(pos).character = '!';
    // }
    
    
    map.print();

    return 0;
}
