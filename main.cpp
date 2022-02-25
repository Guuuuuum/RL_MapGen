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

    OpenWideFloor owf(map);
    std::vector<char> layer = owf.fill_random(map.as_room(), 0.01f);

    std::vector<v2> points;
    for (int y = 0; y < map.size.y; y++)
    {
        for (int x = 0; x < map.size.x; x++)
        {
            char c = map.get_by_coord(layer, map.size, v2(x, y));
            map.get_tile(x, y).character = c;

            if (c == '#')
                points.emplace_back(x, y);
        }
    }
    
	// std::vector<std::pair<v2, int>> regions;

	// for (const auto& point : points)
	// 	regions.emplace_back(point, 1);

	// for (int y = 0; y < map.size.y; ++y)
	// 	for (int x = 0; x < map.size.x; ++x)
	// 	{
	// 		v2 pos(x, y);
	// 		int nearest = -1;
	// 		int nearestDistance = INT_MAX;

	// 		for (size_t i = 0; i < points.size(); ++i)
	// 		{
	// 			int distance = RLUtil::euclidean(points[i], pos);

	// 			if (distance < nearestDistance)
	// 			{
	// 				nearest = i;
	// 				nearestDistance = distance;
	// 			}
	// 		}

	// 		regions[nearest].first += pos;
	// 		regions[nearest].second += 1;
	// 	}

	// for (size_t i = 0; i < points.size(); ++i)
	// 	points[i] = regions[i].first / regions[i].second;
    
    // for (const v2& pos : points)
    //     map.get_tile(pos).character = 'b';

	std::vector<v2> connected;

	connected.emplace_back(points.back());
	points.pop_back();

	while (!points.empty())
	{
		v2 bestFrom;
		int bestToIndex = -1;
		int bestDistance = INT_MAX;

		for (const auto& from : connected)
		{
			for (std::size_t i = 0; i < points.size(); ++i)
			{
				int distance = RLUtil::euclidean(points[i], from);

				if (distance < bestDistance)
				{
					bestFrom = from;
					bestToIndex = i;
					bestDistance = distance;
				}
			}
		}

		v2 to = points[bestToIndex];

        RLUtil::draw_wall(map, bestFrom, to);
        
		connected.emplace_back(to);
		points.erase(points.begin() + bestToIndex);
	}

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
