#pragma once

#include "level_generation.h"
#include "random.h"
#include "astar.h"

class BSPDungeon
{
public:
    BSPDungeon(Map& in_map) : map(in_map) {}
    std::vector<Room> rooms;
private:
    Map& map;

public:
    void generate(Room in_room, const int depth)
    {
        // rooms.reserve( static_cast<size_t>(std::powl(2, depth)) );

        std::vector<Room> bsp_spaces;
        divide_room(bsp_spaces, in_room, depth);

        // draw random size rooms inside bsp spaces 
        
        for (Room& space : bsp_spaces)
        {
            if (space.size.x <= Room::MIN_SIZE.x || space.size.y <= Room::MIN_SIZE.y)
            {
                rooms.emplace_back(space);
                continue;
            }

            Random rand;

            int x_rand_range = space.size.x/4 + rand.get_rand(space.size.x/2);
            int y_rand_range = space.size.y/4 + rand.get_rand(space.size.y/2);
            
            int x = std::clamp(x_rand_range - Room::MIN_SIZE.x, 0, space.size.x - Room::MIN_SIZE.x);
            int y = std::clamp(y_rand_range - Room::MIN_SIZE.y, 0, space.size.y - Room::MIN_SIZE.y);
            v2 rand_offset(x, y);

            Room rand_room(space.pos + rand_offset, space.size - rand_offset);
            rooms.emplace_back(rand_room);
        }

        for (Room& room : rooms)
        {
            for (int i = 0; i < room.size.x; i++)
            {
                map.get_tile(room.pos + v2(i, 0)).character = '#';
                map.get_tile(room.pos + v2(i, room.size.y-1 )).character = '#';
            }
            
            for (int i = 0; i < room.size.y; i++)
            {
                map.get_tile(room.pos + v2(0, i)).character = '#';
                map.get_tile(room.pos + v2(room.size.x-1, i)).character = '#';
            }
        }

        for (size_t i = 0; i < rooms.size(); i += 2)
        {
            Room& cur = rooms[i];
            Room& next = rooms[i+1];

            Random rand;
            const v2 dir = next.pos - cur.pos;
            const v2 dp_temp(
                std::clamp(dir.normalize().x * cur.size.x-1, 0, cur.size.x-1),
                std::clamp(dir.normalize().y * cur.size.y-1, 0, cur.size.y-1));

            v2 door_pos = dp_temp;
            if (door_pos.y > 0)
            {
                door_pos.x = rand.get_rand(1, cur.size.x-1);
            }
            else
            {
                door_pos.y = rand.get_rand(1, cur.size.y-1);
            }

            map.get_tile(cur.pos + door_pos).character = '+';
            
            const v2 door_dir = (next.pos - door_pos).normalize();
        }

        for (size_t i = 0; i < rooms.size(); i++)
            map.get_tile(rooms[i].pos).character = 'a' + i;
    };

private:
    void divide_room(std::vector<Room>& spaces, Room room, int depth)
    {
        if (room.size.x < Room::MIN_SIZE.x || room.size.y < Room::MIN_SIZE.y)
        {
            spaces.emplace_back(room);
            return;
        }

        if (depth == 0)
        {
            spaces.emplace_back(room);
            return;
        }

        Random rand;
        if (room.size.x > room.size.y)
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