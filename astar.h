#pragma once

#include <queue>
#include "map.h"
#include "directions.h"
#include "util.h"

class NaviOverlay
{
public:
    v2 pos_before;
    bool is_checked = false;
    bool is_passable = true;
    float cost = std::numeric_limits<float>::max();
};

class TileNode
{
public:
    TileNode(v2 in_pos) : pos(in_pos) {};
    TileNode(v2 in_pos, float cost) : pos(in_pos), cost_score(cost) {};
    bool operator<(const TileNode& rh) const { return cost_score > rh.cost_score;}
    TileNode* node_before;

    v2 pos;
    float cost_score = 0;
    // const float MOVE_COST = 10;
};

class AStar
{
public:

    AStar(Map& in_map) : map(in_map) //, map_tiles(in_map.size.x * in_map.size.y)
    {
    };

    Map& map;

    std::vector<v2> draw_way(v2 start, v2 end)
    {
        std::priority_queue<TileNode, std::vector<TileNode>> possible_queue;
        std::vector<NaviOverlay> navi_overlay(map.size.x * map.size.y);
        
        possible_queue.push( start );
        map.get_by_coord<NaviOverlay>(navi_overlay, start).cost = 0;

        while (!possible_queue.empty())
        {
            v2 search_pos = possible_queue.top().pos;
            possible_queue.pop();

            if (search_pos == end)
            {
                std::vector<v2> path;

                while (search_pos != start)
                {
                    path.emplace_back(search_pos);
                    
                    search_pos = map.get_by_coord<NaviOverlay>(navi_overlay, search_pos).pos_before;
                }
                
                path.emplace_back(start);
                return path;
            }

            NaviOverlay& nav_node = map.get_by_coord<NaviOverlay>(navi_overlay, search_pos);
            nav_node.is_checked = true;

            map.get_tile(search_pos.x, search_pos.y).character = '*';

            for (auto dir : Directions::OCT_DIRECTIONS)
            {
                v2 to_check = search_pos + dir.dir;

                if (!map.in_bounds(to_check))
                    continue;

                NaviOverlay& nav_next_node = map.get_by_coord<NaviOverlay>(navi_overlay, to_check);
                // todo else conditions
                if (map.get_tile(to_check.x, to_check.y).character == '#' ||// !nav_next_node.is_passable ||
                    nav_next_node.is_checked)
                {
                    continue;
                }
                
                float move_cost = nav_node.cost + dir.move_cost;
                if (move_cost < nav_next_node.cost)
                {
                    possible_queue.push( {to_check, move_cost+RLUtil::distance_RL(to_check, end) } );
                    nav_next_node.cost = move_cost;
                    nav_next_node.pos_before = search_pos;
                }
            };
        };

        return {};
    };
};
