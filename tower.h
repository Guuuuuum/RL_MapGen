#pragma once

#include "level_generation.h"

class Tower : LevelGeneration
{
private:
    std::vector<v2> walkable_floors;
    std::vector<Room> rooms;
    Map& map;
public:
    Tower(Map& in_map) : map(in_map) {}
    std::vector<v2> get_floors() override { return walkable_floors; }
    std::vector<Room> get_rooms() override { return rooms; }

    void generate(Room in_room)
    {
        
    }
};
