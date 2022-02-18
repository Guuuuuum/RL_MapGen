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

// 시도한 조합
// BSP
// BSP + Random walk
// BSP + Celluar automata
// BSP + Celluar automata ( no bounds )

int main(int, char**) 
{
    Map map(v2(50,50));

    // OpenWideFloor owf(map);
    // owf.rounds_walking(map.as_room(), v2(25, 35));
    // owf.cellular_automata(map.as_room(), 2, 4, 5);

    BSPDungeon bspmap(map);
    bspmap.generate({ {0, 0}, map.size}, 4);    

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
