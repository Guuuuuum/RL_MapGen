#pragma once

#include <queue>
#include "map.h"

namespace RLUtil
{

struct TileNode
{

};

class AStar
{
public:
    // std::priority_queue<TileNode> tile_tracks;

    AStar(Map& in_map) : map(in_map) //, map_tiles(in_map.size.x * in_map.size.y)
    {
    };

    Map& map;

    void draw_way(v2 start, v2 end)
    {
        
    }
};

inline int tile_distance(const v2& start, const v2& end)
{
    const v2 delta = { std::abs(end.x - start.x), std::abs(end.y - start.y) };
    return std::max(delta.x, delta.y);
};

};
