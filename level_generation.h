#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>

class Room
{
public:
    Room(v2 in_pos, v2 in_size) : pos(in_pos), size(in_size) {};
    const v2 pos; //pivot is left-up.
    const v2 size;

    static const v2 MIN_SIZE;

    int get_extent() const
    {
        return size.x * size.y;
    }

    bool in_bounds(const v2 target) const
    {
        if (target.x < 0 || target.x >= size.x )
            return false;
        if (target.y < 0 || target.y >= size.y )
            return false;

        return true;
    }

    bool in_bounds_world(const v2 target) const
    {
        if (target.x < pos.x || target.x >= (pos.x + size.x) )
            return false;
        if (target.y < pos.y || target.y >= (pos.y + size.y) )
            return false;

        return true;
    }

    bool on_bounds(const v2 target) const
    {
        if (target.x == 0 || target.x == size.x-1 )
            return true;
        if (target.y == 0 || target.y == size.y-1 )
            return true;

        return false;
    }

    bool on_bounds_world(const v2 target) const
    {
        if (target.x == pos.x || target.x == (pos.x + size.x-1) )
            return true;
        if (target.y == pos.y || target.y == (pos.y + size.y-1) )
            return true;

        return false;
    }
};

inline const v2 Room::MIN_SIZE = v2(4, 4);