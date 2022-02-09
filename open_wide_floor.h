#pragma once

#include "level_generation.h"
#include "directions.h"

class OpenWideFloor : LevelGeneration
{
private:
    std::vector<v2> walkable_floors;
    std::vector<Room> rooms;
    Map& map;
public:
    OpenWideFloor(Map& in_map) : map(in_map) {}
    std::vector<v2> get_floors() override { return walkable_floors; }
    std::vector<Room> get_rooms() override { return rooms; }

private:
    inline void draw_circle_wall(const Room room, const v2 center, int x, int y)
    {
        const v2 clamp_size = map.size;

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
    void circle(const Room room, const v2 center, const int r)
    {
        int x = 0; 
        int y = r;
        int d = 3 - 2 * r;

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

    using CarveOverlay = int;
    void random_walk_cave(const Room room, const size_t energy)
    {
        fill(room);
        std::vector<CarveOverlay> overlay(room.size.x * room.size.y);

        v2 walking_carve = v2(room.size.x/2, room.size.y/2);
        map.get_by_coord(overlay, walking_carve) = -1;

        for (size_t i = 0; i < energy; i++)
        {
            Random rand;
            Directions dir = rand.pick_one(Directions::OCT_DIRECTIONS);
            
            v2 exp = walking_carve + dir.dir;
            while (!room.in_bounds(exp))
            {
                dir = rand.pick_one(Directions::OCT_DIRECTIONS);
                exp = walking_carve + dir.dir;
            }
            
            --map.get_by_coord(overlay, exp);

            if (map.get_by_coord(overlay, exp) < 0)
                walking_carve = exp;
        }

        for (int i = 0; i < room.size.x; i++)
            for (int ii = 0; ii < room.size.y; ii++)
                map.get_tile(room.pos + v2(i, ii)).character = map.get_by_coord(overlay, v2(i, ii)) < 0 ? '.' : '#';
    };
    
    // 구형, 사각형의 얇은 벽들이 있는 맵
    // 3x3 +, ㅁ 형상의 기둥이 있는 맵
    // 랜덤 1x1 벽들
    // 공통적으로는 계단을 중심으로 정규분포로 벽이 퍼져있어야 함
public:
    void generate(Room in_room)
    {
        random_walk_cave(in_room, 1000);

        // circle(in_room, in_room.pos + v2(in_room.size.x/2, in_room.size.y/2), 10);
    }
};
