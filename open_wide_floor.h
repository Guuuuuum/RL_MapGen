#pragma once

#include "level_generation.h"

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

    // 구형, 사각형의 얇은 벽들이 있는 맵
    // 3x3 +, ㅁ 형상의 기둥이 있는 맵
    // 랜덤 1x1 벽들
    // 공통적으로는 계단을 중심으로 정규분포로 벽이 퍼져있어야 함
    void generate(Room in_room)
    {
        
    }
};
