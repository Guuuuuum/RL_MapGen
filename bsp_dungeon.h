#pragma once

#include "level_generation.h"
#include "random.h"
#include "astar.h"

#include <algorithm>

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
        RLUtil::fill(map, in_room);

        std::vector<Room> bsp_spaces;
        divide_room(bsp_spaces, in_room, depth);

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
                for (int ii = 0; ii < room.size.y; ii++)
                {
                    if (room.on_bounds(v2(i, ii)))
                        map.get_tile(room.pos + v2(i, ii)).character = '#';
                    else
                        map.get_tile(room.pos + v2(i, ii)).character = '.';
                }
            }
        }

        std::vector<std::vector<v2>> doorway_pos;
        for (size_t i = 0; i < rooms.size()-1; i++)
        {
            Room& cur = rooms[i];
            Room& next = rooms[i+1];

            std::vector<v2> doorways;

            Random rand;
            const v2 dir = next.pos - cur.pos;
            const v2 dp_temp(
                std::clamp(dir.flatten_max().x * cur.size.x-1, 0, cur.size.x-1),
                std::clamp(dir.flatten_max().y * cur.size.y-1, 0, cur.size.y-1));

            v2 door_pos = dp_temp;
            if (door_pos.y > 0)
                door_pos.x = rand.get_rand(1, cur.size.x-1);
            else
                door_pos.y = rand.get_rand(1, cur.size.y-1);

            v2 next_doorpos = next.pos;
            const v2 delta_nextroom = next.pos - (cur.pos + door_pos);
            if (delta_nextroom.y > delta_nextroom.x)
                next_doorpos.x += rand.get_rand(1, next.size.x-1);
            else
                next_doorpos.y += rand.get_rand(1, next.size.y-1);

            // making pathway as xyx or yxy order
            v2 door_dir = next_doorpos - (cur.pos + door_pos);
            int xyx_yxy = door_dir.max_index();

            doorways.emplace_back(cur.pos + door_pos);

            const auto doorway = [&]()
            {
                door_pos += door_dir.flatten_index(xyx_yxy);
                map.get_tile(cur.pos + door_pos).character = '.';
                doorways.emplace_back(cur.pos + door_pos);
            };

            int curveway_num = (std::abs(door_dir[xyx_yxy])/4) + rand.get_rand(std::abs(door_dir[xyx_yxy])/2);
            for (size_t ii = 0; ii < std::abs(door_dir[xyx_yxy]) - curveway_num; ii++)
            {
                doorway();
            }

            xyx_yxy = !xyx_yxy;
            for (size_t ii = 0; ii < std::abs(door_dir[xyx_yxy]); ii++)
            {
                doorway();
            }

            xyx_yxy = !xyx_yxy;
            door_dir = next_doorpos - (cur.pos + door_pos);
            for (size_t ii = 0; ii < std::abs(door_dir[xyx_yxy]); ii++)
            {
                doorway();
            }

            doorways.emplace_back(next_doorpos);
            doorway_pos.emplace_back(doorways);
        }

        for (const std::vector<v2>& ways : doorway_pos)
        {
            bool door_placed_start = false;
            bool door_placed_end = false;

            for (const v2& way : ways)            
            {                
                for (const Directions& dir : Directions::OCT_DIRECTIONS)
                {
                    if (std::find(ways.begin(), ways.end(), way + dir.dir) == ways.end())
                    {
                        bool outdoor = true;
                        for (const Room& room : rooms)
                        {
                            if (room.in_bounds_world(way + dir.dir))
                            {
                                outdoor = false;
                                break;
                            }
                        }

                        if (outdoor)
                        {
                            map.try_call_in_bound(way + dir.dir, [&]()
                            {
                                map.get_tile(way + dir.dir).character = '#';
                            });
                        }
                    }
                }

                bool vertical_door = map.is_not_passable(way + v2::up) && map.is_not_passable(way + v2::down);
                bool horizonal_door = map.is_not_passable(way + v2::right) && map.is_not_passable(way + v2::left);

                if (!door_placed_start && (horizonal_door || vertical_door))
                {
                    door_placed_start = true;
                    map.get_tile(way).character = '+';
                }
                else
                {
                    map.get_tile(way).character = '.';
                }
            }
            

            for (int i = ways.size() - 1; i >= 0; i--)
            {
                const v2& way = ways[i];
                bool vertical_door = map.is_not_passable(way + v2::up) && map.is_not_passable(way + v2::down);
                bool horizonal_door = map.is_not_passable(way + v2::right) && map.is_not_passable(way + v2::left);

                if (!door_placed_end && (horizonal_door || vertical_door))
                {
                    door_placed_end = true;
                    map.get_tile(way).character = '+';
                    break;
                }
                else
                {
                    map.get_tile(way).character = '.';
                } 
            }           
        }

        // for (size_t i = 0; i < rooms.size(); i++)
        //     map.get_tile(rooms[i].pos).character = 'a' + i;
    };

private:
    void divide_room(std::vector<Room>& spaces, Room room, int depth)
    {
        if (room.size.x <= Room::MIN_SIZE.x || room.size.y <= Room::MIN_SIZE.y)
        {
            // spaces.emplace_back(room);
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

            if (room.size.x/2 <= Room::MIN_SIZE.x)
                rand_width = Room::MIN_SIZE.x + rand.get_rand(room.size.x - Room::MIN_SIZE.x);

            divide_room(spaces, Room(room.pos, v2(rand_width, room.size.y)), depth - 1);
            divide_room(spaces, Room(room.pos + v2(rand_width-1, 0), v2(room.size.x - rand_width+1, room.size.y)), depth - 1);
        }
        else
        {
            const int min_cutlength = room.size.y/4;
            int rand_height = min_cutlength + rand.get_rand(min_cutlength) * 2;

            if (room.size.y/2 <= Room::MIN_SIZE.y)
                rand_height = Room::MIN_SIZE.y + rand.get_rand(room.size.y - Room::MIN_SIZE.y);

            divide_room(spaces, Room(room.pos, v2(room.size.x, rand_height)), depth - 1);
            divide_room(spaces, Room(room.pos + v2(0, rand_height-1), v2(room.size.x, room.size.y - rand_height+1)), depth - 1);
        }
    };
};