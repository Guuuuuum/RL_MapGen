#pragma once

#include "level_generation.h"
#include "directions.h"
#include <set>
#include <algorithm>

struct CA_params
{
public:
    CA_params(int in_gen, float percent, int in_wall_d, int in_tile_d, int in_tile_counter_d, int in_wall_counter_d) : 
        gen(in_gen),
        fill_percentage(percent),
        wall_d(in_wall_d),
        tile_d(in_tile_d),
        tile_counter_d(in_tile_counter_d),
        wall_counter_d(in_wall_counter_d) {};
    
    const int gen;
    const float fill_percentage;
    const int wall_d;
    const int tile_d;

    const int wall_counter_d;
    const int tile_counter_d;
};

class OpenWideFloor
{
private:
    Map& map;
public:
    OpenWideFloor(Map& in_map) : map(in_map) {}

    inline void draw_circle_wall(const Room& room, const v2& center, const int x, const int y)
    {
        v2 r1 (center.x + x, center.y + y);
        v2 r2 (center.x - x, center.y + y);
        v2 r3 (center.x + x, center.y - y);
        v2 r4 (center.x - x, center.y - y);
        v2 r5 (center.x + y, center.y + x);
        v2 r6 (center.x - y, center.y + x);
        v2 r7 (center.x + y, center.y - x);
        v2 r8 (center.x - y, center.y - x);

        std::vector<v2> corners = {r1, r2, r3, r4, r5, r6, r7, r8};
        for (v2 p : corners)
        {
            if (room.in_bounds(p))
                map.get_tile(p).character = '#';
        }
        
    }
      
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/?ref=lbp
    void bresenhams_circle(const Room& room, const v2& center, const int r)
    {
        int x = 0; 
        int y = r;
        int d = 3 - (2 * r);

        x = y - 3;

        draw_circle_wall(room, center, x, y);

        while (y >= x)
        {
            x++;

            if (d > 0)
            {
                y--;
                d += 4 * (x - y) + 10;
            }
            else
            {
                d += 4 * x + 6;
            }

            draw_circle_wall(room, center, x, y);
            draw_circle_wall(room, center, x-1, y); // plus or minus one idk which better
        }
    }

    void rounds_walking(const Room& room, const v2& pos, const int walk_energy = 1)
    {
             
    }

    using TileFlag = char;
    std::vector<TileFlag> fill_random(const Room& room, const float fill_percentage)
    {
        std::vector<TileFlag> layer(room.size.x * room.size.y);
        std::fill(layer.begin(), layer.end(), '.');

        Random rand;
        std::set<int> hands;

        const int cell_num = static_cast<int>(room.size.x * room.size.y * fill_percentage);
        for (int i = 0; i < cell_num; i++)
        {
            int index = rand.get_rand(room.size.x * room.size.y);
            while (hands.find(index) != hands.end())
                index = rand.get_rand(room.size.x * room.size.y);
            
            hands.emplace(index);
            map.get_by_coord(layer, room.size, v2(index%(room.size.x), index/room.size.x)) = '#';
        }

        return layer;
    }

    void cellular_automata(const Room room, const CA_params& params)
    {
        std::vector<TileFlag> room_seeds = fill_random(room, params.fill_percentage);
        
        for (int x = 0; x < room.size.x; x++)
            for (int y = 0; y < room.size.y; y++)
                map.get_tile(room.pos + v2(x, y)).character = map.get_by_coord(room_seeds, room.size, v2(x, y));
        
        const auto loop_overmap = [&](const auto methods)
        {
            for (int x = 0; x < room.size.x; x++)
            {
                for (int y = 0; y < room.size.y; y++)
                {
                    const v2 pos(x, y);
                    int wall_count = 0;
                    for (const Directions& dir : Directions::OCT_DIRECTIONS)
                    {
                        if (map.in_bounds(room.pos + pos + dir.dir))
                        {
                            if (map.get_tile(room.pos + pos + dir.dir).character == '#')
                                ++wall_count;
                        }
                        else
                        {
                            ++wall_count;
                        }
                    }

                    methods(pos, wall_count);
                }
            }
        };

        for (size_t i = 0; i < params.gen; i++)
        {
            std::vector<TileFlag> layer(room.size.x * room.size.y);
            std::fill(layer.begin(), layer.end(), '.');

            loop_overmap([&](const v2& pos, const int wall_count)
            {
                if (map.get_tile(room.pos + pos).character == '#')
                {
                    if (wall_count >= params.wall_d)
                        map.get_by_coord(layer, room.size, room.pos + pos) = '#';
                    else
                        map.get_by_coord(layer, room.size, room.pos + pos) = '.';

                    if (wall_count <= params.tile_counter_d)
                        map.get_by_coord(layer, room.size, room.pos + pos) = '.';
                }
                else
                {
                    if (wall_count >= params.tile_d)
                        map.get_by_coord(layer, room.size, room.pos + pos) = '#';
                    else
                        map.get_by_coord(layer, room.size, room.pos + pos) = '.';

                    if (8 - wall_count <= params.wall_counter_d)
                        map.get_by_coord(layer, room.size, room.pos + pos) = '#';
                }                
            });

            for (int x = 0; x < room.size.x; x++)
                for (int y = 0; y < room.size.y; y++)
                    map.get_tile(room.pos + v2(x, y)).character = map.get_by_coord(layer, room.size, room.pos + v2(x, y));
            
        }

        std::vector<std::vector<v2>> holes = determine_rooms(room);

        std::vector<std::vector<v2>> cave_paths;
        for (const std::vector<v2>& hole : holes)
        {
            std::vector<v2> path;

            for (size_t i = 6; i > 0; i--)
            {
                for (const v2& pos : hole)
                {
                    if (map.get_neibours_wall(pos).size() > i)
                    {
                        path.emplace_back(pos);
                        map.get_tile(pos).character = '+';
                    }
                }
                if (path.size() > 1)
                    break;
            }

            cave_paths.emplace_back(path);
        }

        for (size_t i = 0; i < cave_paths.size()-1; i++)
        {
            const std::vector<v2>& doors = cave_paths[i];
            const v2& door = doors[0];
            const v2& next = cave_paths[i+1][0];

            AStar pathfinding(map);
            std::vector<v2> paths = pathfinding.draw_way(door, next, '.', Directions::CROSS_DIRECTIONS);

            for (const v2& pos : paths)
            {
                map.get_tile(pos).character = '-';
            }
        }
    }

    std::vector<std::vector<v2>> determine_rooms(const Room& room)
    {
        std::vector<std::vector<v2>> holes;

        const auto contains_tile = [&](const v2& pos)
        {
            bool contains = false;

            for (const std::vector<v2>& tiles : holes)
            {
                if (std::find(tiles.begin(), tiles.end(), pos) != tiles.end())
                {
                    contains = true;
                    break;
                }
            }

            return contains;
        };

        for (int y = 0; y < room.size.y; y++)
        {
            for (int x = 0; x < room.size.x; x++)
            {
                const v2 pos = v2(x, y);

                if (map.get_tile(room.pos + pos).character != '#' && !contains_tile(room.pos + pos))
                {
                    std::vector<v2> tiles;

                    flood_fill(tiles, room.pos + pos);
                    holes.emplace_back(tiles);
                }
            }
        }

        const auto small_islands = std::remove_if(holes.begin(), holes.end(), [&](const std::vector<v2>& points)
        {
            if (points.size() <= 8)
            {
                for (const v2& pos : points)
                {
                    map.get_tile(pos).character = '#';
                }
                return true;
            }
            return false;
        });
        
        holes.erase(small_islands, holes.end());

        return holes;
    }

    void flood_fill(std::vector<v2>& tiles, const v2& pos)
    {
        for (const v2 dir : map.get_neibours(pos, true, true))
        {
            if (map.get_tile(pos + dir).character != '#')
            {
                if (std::find(tiles.begin(), tiles.end(), pos + dir) == tiles.end())
                {
                    tiles.emplace_back(pos + dir);
                    flood_fill(tiles, pos + dir);
                }
            }
        }
    }

    void mazelike_fill(const Room& room, const CA_params& params)
    {
        std::vector<TileFlag> room_seeds = fill_random(room, params.fill_percentage);
        
        for (int x = 0; x < room.size.x; x++)
            for (int y = 0; y < room.size.y; y++)
                map.get_tile(room.pos + v2(x, y)).character = map.get_by_coord(room_seeds, room.size, v2(x, y));

        for (int x = 0; x < room.size.x; x++)
        {
            for (int y = 0; y < room.size.y; y++)
            {
                const v2 pos(x, y);
                int wall_count = 0;
                for (const Directions& dir : Directions::OCT_DIRECTIONS)
                {
                    if (map.in_bounds(room.pos + pos + dir.dir))
                    {
                        if (map.get_tile(room.pos + pos + dir.dir).character == '#')
                            ++wall_count;
                    }
                    else
                    {
                        ++wall_count;
                    }
                }

                if (wall_count <= params.wall_d)
                    map.get_tile(room.pos + pos).character = '#';
                else
                    map.get_tile(room.pos + pos).character = '.';
            }
        }
    }

    using CarveOverlay = int;
    std::vector<CarveOverlay> random_walk_cave(const Room& room, const size_t energy)
    {
        RLUtil::fill(map, room);
        std::vector<CarveOverlay> overlay(room.get_extent());
        std::vector<v2> carve_shape = {v2(0,0), v2(1, 0), v2(0, 1), v2(1, 1)};
        std::function<bool(v2)> shape_in_bounds = [=](v2 pos)
        {
            bool col_test = false;
            for (v2 shape : carve_shape)
                col_test = col_test || room.in_bounds(shape + pos);
            
            return col_test;
        };

        v2 pivot = v2(room.size.x/2, room.size.y/2);

        for (v2& shape : carve_shape)
            map.try_call_in_bound(overlay, room, pivot + shape, [&](){ map.get_by_coord(overlay, room.size, pivot + shape) = -1; } );

        for (size_t i = 0; i < energy; i++)
        {
            Random rand;

            Directions dir = rand.pick_one(Directions::OCT_DIRECTIONS);            
            v2 exp = pivot + dir.dir;
            if (rand.get_rand(3) < 1)
            {
                
            }

            while (!shape_in_bounds(exp))
            {
                dir = rand.pick_one(Directions::OCT_DIRECTIONS);
                exp = pivot + dir.dir;
            }
            
            for (v2& shape : carve_shape)
                map.try_call_in_bound(overlay, room, exp + shape, [&](){ --map.get_by_coord(overlay, room.size, exp + shape); } );

            if (room.in_bounds(exp) && map.get_by_coord(overlay, room.size, exp) < 0)
                pivot = exp;
        }

        return overlay;
    };
    
    // 구형, 사각형의 얇은 벽들이 있는 맵
    // 3x3 +, ㅁ 형상의 기둥이 있는 맵
    // 랜덤 1x1 벽들
    // 공통적으로는 계단을 중심으로 정규분포로 벽이 퍼져있어야 함
public:
    void generate(const Room& in_room, const int energy)
    {
        std::vector<CarveOverlay> overlay = random_walk_cave(in_room, energy);

        for (int i = 0; i < in_room.size.x; i++)
            for (int ii = 0; ii < in_room.size.y; ii++)
                map.get_tile(in_room.pos + v2(i, ii)).character = map.get_by_coord(overlay, in_room.size, v2(i, ii)) < 0 ? '.' : '#';
        
        // circle(in_room, in_room.pos + v2(in_room.size.x/2, in_room.size.y/2), 10);
    }
};
