#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>

#include "map.h"
#include "random.h"
#include "astar.h"

class Room
{
public:
    Room(v2 in_pos, v2 in_size) : pos(in_pos), size(in_size) {};
    v2 pos; //pivot is left-up.
    v2 size;

    static const v2 MIN_SIZE;
    void clamp_size(v2 map_size)
    {
        size.x = std::clamp(pos.x + size.x, 0, map_size.x-1);
        size.y = std::clamp(pos.y + size.y, 0, map_size.y-1);
    }

    int get_extent() const
    {
        return size.x * size.y;
    }

    bool in_bounds(const v2 pos) const
    {
        if (pos.x < 0 || pos.x >= size.x )
            return false;
        if (pos.y < 0 || pos.y >= size.y )
            return false;

        return true;
    }
};

inline const v2 Room::MIN_SIZE = v2(4, 4);

class LevelGeneration
{
public:
    virtual std::vector<Room> get_rooms() = 0;
    virtual std::vector<v2> get_floors() = 0;
};
