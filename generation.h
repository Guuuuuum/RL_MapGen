#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>

#include "map.h"
#include "random.h"

class Room
{
public:
    Room(v2 in_pos, v2 in_size) : pos(in_pos), size(in_size) {};
    v2 pos; //pivot is left-up.
    v2 size;
    std::vector<v2> doors;

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
            Random rand;
            int x_rand_range = space.size.x/4 + rand.get_rand(space.size.x/2);
            int y_rand_range = space.size.y/4 + rand.get_rand(space.size.y/2);
            int x = std::clamp(x_rand_range - Room::MIN_SIZE.x, 0, space.size.x - Room::MIN_SIZE.x);
            int y = std::clamp(y_rand_range - Room::MIN_SIZE.y, 0, space.size.y - Room::MIN_SIZE.y);
            v2 rand_offset(x, y);

            Room rand_room(space.pos + rand_offset, space.size - rand_offset);
            rooms.push_back(rand_room);
        }
        
        for (auto room : rooms)
        {
            for (int i = 0; i < room.size.x; i++)
            {
                map.get_tile(room.pos + v2(i, 0)).character = 'J';
                map.get_tile(room.pos + v2(i, room.size.y-1 )).character = 'X';
            }
            for (int i = 0; i < room.size.y; i++)
            {
                map.get_tile(room.pos + v2(0, i)).character = 'J';
                map.get_tile(room.pos + v2(room.size.x-1, i)).character = 'X';
            }

            Random door_rand;
            v2 door_pos = door_rand.get_room_outline_rand(room.size);
            map.get_tile(room.pos + door_pos).character = 'D';
        }
        
    };

private:
    void divide_room(std::vector<Room>& spaces, Room room, int depth)
    {
        if (room.size.x < Room::MIN_SIZE.x || room.size.y < Room::MIN_SIZE.y)
            return;

        if (depth == 0)
        {
            spaces.emplace_back(room);
            return;
        }

        Random rand;
        // cut by width/height ratio. it looks better
        if (room.size.x > static_cast<float>(room.size.y) * 2.0f )
        {
            const int min_cutlength = room.size.x/4;
            int rand_width = min_cutlength + rand.get_rand(min_cutlength) * 2;

            divide_room(spaces, Room(room.pos, v2(rand_width, room.size.y)), depth - 1);
            divide_room(spaces, Room(room.pos + v2(rand_width-1, 0), v2(room.size.x - rand_width+1, room.size.y)), depth - 1);
        }
        else
        {
            const int min_cutlength = room.size.y/4;
            int rand_height = min_cutlength + rand.get_rand(min_cutlength) * 2;

            divide_room(spaces, Room(room.pos, v2(room.size.x, rand_height)), depth - 1);
            divide_room(spaces, Room(room.pos + v2(0, rand_height-1), v2(room.size.x, room.size.y - rand_height+1)), depth - 1);
        }
    };
};