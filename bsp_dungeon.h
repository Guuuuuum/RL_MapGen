#pragma once

#include "level_generation.h"
#include "random.h"

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
                map.get_tile(room.pos + v2(i, 0)).character = '#';
                map.get_tile(room.pos + v2(i, room.size.y-1 )).character = '#';
            }
            
            for (int i = 0; i < room.size.y; i++)
            {
                map.get_tile(room.pos + v2(0, i)).character = '#';
                map.get_tile(room.pos + v2(room.size.x-1, i)).character = '#';
            }

        }  

        // temporal room test
        for (size_t i = 0; i < rooms.size(); i++)
            map.get_tile(rooms[i].pos).character = 'a' + i;

        // connect dungeons
        // for (size_t i = 0; i < rooms.size() - 1; i++)
        // {
        //     const Room cur_room = rooms[i];
        //     const Room dest_room = rooms[i+1];

        //     v2 d = dest_room.pos - (cur_room.pos + cur_room.size);
        //     // d.x = std::abs(d.x);
        //     // d.y = std::abs(d.y);
        //     Random rand;

        //     // exclude the edge of square to minus 1,2
        //     v2 door_pos = d.x > d.y ? 
        //         v2(cur_room.size.x-1, rand.get_rand(1, cur_room.size.y-1) ) : 
        //         v2(rand.get_rand(1, cur_room.size.x-1), cur_room.size.y-1);

        //     if (d.x == d.y)
        //     {
        //         door_pos = rand.get_rand(2) < 1 ? 
        //             v2(cur_room.size.x-1, rand.get_rand(1, cur_room.size.y-1)) : 
        //             v2(rand.get_rand(1, cur_room.size.x-1), cur_room.size.y-1);
        //     }
            
        //     int hall_length_1 = std::abs((dest_room.pos - (cur_room.pos + door_pos)).max());
        //     d = dest_room.pos - cur_room.pos + door_pos;
        //     for (size_t ii = 0; ii < hall_length_1+1; ii++)
        //     {
        //         map.get_tile(cur_room.pos + door_pos).character = 'p';

        //         if (ii < std::abs(hall_length_1))
        //             door_pos += d.normalize();
        //     }

        //     // while (!dest_room.in_bounds_world(cur_room.pos + door_pos))
        //     if (dest_room.in_bounds_world(cur_room.pos + door_pos))
        //     {
        //         map.get_tile(cur_room.pos + door_pos).character = 'p';
        //         continue;
        //     }
        // }
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