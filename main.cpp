#include <iostream>
#include <cstdlib>

#include "directions.h"
#include "v2.h"
#include "map.h"
#include "astar.h"
#include "bsp_dungeon.h"
#include "tower.h"
#include "open_wide_floor.h"
#include "util.h"

#include "triangulation.hpp"

// 시도한 조합
// BSP
// BSP + Random walk
// BSP + Celluar automata
// BSP + Celluar automata ( no bounds )

int main(int, char**) 
{
    Map map(v2(50,50));

    OpenWideFloor owf(map);
    std::vector<char> layer = owf.fill_random(map.as_room(), 0.01f);

    std::vector<v2> points;
    for (int y = 0; y < map.size.y; y++)
    {
        for (int x = 0; x < map.size.x; x++)
        {
            char c = map.get_by_coord(layer, map.size, v2(x, y));
            // map.get_tile(x, y).character = c;

            if (c == '#')
                points.emplace_back(x, y);
        }
    }

    // std::vector<v2> points;
    // points.emplace_back(v2(25, 10));
    // points.emplace_back(v2(10, 35));
    // points.emplace_back(v2(40, 25));
    // points.emplace_back(v2(40, 45));

    RLUtil::voronoi(map, points);

    for (const v2& pos : points)
    {
        map.get_tile(pos).character = '#';
    }

    delaunator::Delaunator d = delaunator::Delaunator::create(points);

    for (size_t i = 0; i < d.triangles.size(); i+=3)
    {
        v2 pos_0;
        v2 pos_1;
        v2 pos_2;
        pos_0.x = d.coords[2 * d.triangles[i]];
        pos_0.y = d.coords[2 * d.triangles[i] + 1];
        pos_1.x = d.coords[2 * d.triangles[i + 1]];
        pos_1.y = d.coords[2 * d.triangles[i + 1] + 1];
        pos_2.x = d.coords[2 * d.triangles[i + 2]];
        pos_2.y = d.coords[2 * d.triangles[i + 2] + 1];

        RLUtil::draw_wall(map, pos_0, pos_1);
        RLUtil::draw_wall(map, pos_1, pos_2);
        RLUtil::draw_wall(map, pos_0, pos_2);
    }

    // RLUtil::voronoi(map, points);

    // v2 pos;
    // for (size_t i = 0; i < points.size(); i++)
    // {
    //     pos += points[i];
    // }
    // pos /= points.size();
    
    // map.get_tile(pos).character = 'a';

    // owf.rounds_walking(map.as_room(), v2(25, 35));
    // owf.cellular_automata(map.as_room(), {2, 0.45f, 3, 4, 3, 3} );

    // BSPDungeon bspmap(map);
    // bspmap.generate({ {0, 0}, map.size}, 4);    

    // for (Room& room : bspmap.rooms)
    // {
    //     OpenWideFloor owf(map);
    //     // owf.cellular_automata(room, 3, 4, 5);
    //     owf.generate(room, 100);
    // }
    
    // OpenWideFloor owf(map);
    // owf.circle_vector(map.as_room(), v2(map.size.x/2, map.size.y/2), 11);d

    map.print();

    return 0;
}
