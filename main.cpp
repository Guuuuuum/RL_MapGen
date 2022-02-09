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

int main(int, char**) 
{
    Map map(v2(50,50));
    
    // map.draw_border();

    // BSPDungeon bspmap(map);
    // bspmap.generate({ {0, 0}, map.size}, 4);

    // Tower tower(map);
    // tower.generate({ {0, 0}, map.size});

    OpenWideFloor owf(map);
    owf.generate({ {0, 0}, map.size});

    v2 end(0, 0);
    v2 start(10, 35);
    // draw_wall(map, start, end );

    // RLUtil::AStar astar(map);
    // std::vector<v2> way = astar.draw_way(v2(1, 1), v2(78, 23));
    // for (v2 pos : way)
    // {
    //     map.get_tile(pos).character = '!';
    // }
    
    
    map.print();

    return 0;
}
