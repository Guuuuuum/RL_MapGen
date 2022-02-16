#pragma once

#include "level_generation.h"
#include "directions.h"

class OpenWideFloor
{
private:
    Map& map;
public:
    OpenWideFloor(Map& in_map) : map(in_map) {}

    inline void draw_circle_wall(const Room room, const v2 center, int x, int y)
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
    
    v2 bresenham_circle_dir(v2 vector, int& d)
    {
        v2 dir;
        int x = std::abs(vector.x);
        int y = RLUtil::euclidean(vector);

        int cake_index = RLUtil::vector_to_deg(vector) / 45;

        switch (cake_index)
        {
        case 0: 
            dir = v2(+x, +y);
            break;
        case 1:
            dir = v2(-x, +y);
            break;
        case 2:
            dir = v2(+x, -y);
            break; 
        case 3:
            dir = v2(-x, -y);
            break; 
        case 4:
            dir = v2(+y, +x);
            break; 
        case 5:
            dir = v2(-y, +x);
            break; 
        case 6:
            dir = v2(+y, -x);
            break; 
        case 7:
            dir = v2(-y, -x);
            break; 
        default:
            dir = v2(0, 0);
            break;
        }

        if (d > 0)
            d += 4 * (x - y) + 10;
        else
            d += 4 * x + 6;

        v2 asdf = dir.normalize();
        return dir.normalize();
    }
    
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/?ref=lbp
    void circle(const Room room, const v2 center, const int r)
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

    void fill(const Room room)
    {
        for (int i = 0; i < room.size.x; i++)
            for (int ii = 0; ii < room.size.y; ii++)
                map.get_tile(room.pos + v2(i, ii)).character = '#';
    }

    void circle_dir(const v2 center, v2 pos)
    {
        for (size_t i = 0; i < 10; i++)
        {
            int d;
            pos += bresenham_circle_dir(pos, d);
            map.get_tile(pos).character = '#';
        }
    }

    using CarveOverlay = int;
    std::vector<CarveOverlay> random_walk_cave(const Room room, const size_t energy)
    {
        fill(room);
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
    void generate(Room in_room, const int energy)
    {
        std::vector<CarveOverlay> overlay = random_walk_cave(in_room, energy);

        for (int i = 0; i < in_room.size.x; i++)
            for (int ii = 0; ii < in_room.size.y; ii++)
                map.get_tile(in_room.pos + v2(i, ii)).character = map.get_by_coord(overlay, in_room.size, v2(i, ii)) < 0 ? '.' : '#';
        
        // circle(in_room, in_room.pos + v2(in_room.size.x/2, in_room.size.y/2), 10);
    }
};
