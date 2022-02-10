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

    BSPDungeon bspmap(map);
    bspmap.generate({ {0, 0}, map.size}, 4);    

    for (Room& room : bspmap.rooms)
    {
        // OpenWideFloor owf(map);
        // owf.generate(room, 100);
    }
    
    map.print();

    return 0;
}
