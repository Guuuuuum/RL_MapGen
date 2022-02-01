#include <iostream>
#include <cstdlib>

#include "directions.h"
#include "v2.h"
#include "map.h"
#include "astar.h"
#include "generation.h"

int main(int, char**) 
{
    Map map(v2(80,25));
    // map.draw_border();

    BSPDungeon bspmap(map);
    bspmap.generate({ {0, 0}, map.size}, 4);

    // RLUtil::AStar astar(map);
    // std::vector<v2> way = astar.draw_way(v2(1, 1), v2(78, 23));
    // for (v2 pos : way)
    // {
    //     map.get_tile(pos).character = '!';
    // }
    
    
    map.print();

    return 0;
}
