#pragma once

#include "level_generation.h"

class Tower : LevelGeneration
{
private:
    std::vector<v2> walkable_floors;
    std::vector<Room> rooms;
public:
    std::vector<v2> get_floors() const { return walkable_floors; }
    std::vector<Room> get_rooms() const { return rooms; }

    void generate(Room in_room)
    {
    }
};
