#pragma once
#include "map.h"
#include <vector>
#include <algorithm>

namespace RLUtil
{
    inline void fill(Map& map, const Room& room)
    {
        for (int i = 0; i < room.size.x; i++)
            for (int ii = 0; ii < room.size.y; ii++)
                map.get_tile(room.pos + v2(i, ii)).character = '#';
    }

	inline int euclidean(const v2& start, const v2& end)
	{
		const v2 delta = end - start;
		return static_cast<int>(std::sqrt(delta.x * delta.x + delta.y * delta.y));
	}
	
	inline int euclidean(v2& pos)
	{
		return euclidean(v2(0, 0), pos);
	}

	inline int distance_RL(const v2& start, const v2& end)
	{
		const v2 delta = { std::abs(end.x - start.x), std::abs(end.y - start.y) };
		return std::max(delta.x, delta.y);
	};

	double vector_to_rad(v2 v)
	{
		return std::atanf((float)v.y / v.x) * (180.0/ 3.14);
	}

	int vector_to_deg(v2 v) {
		if (v.x == 0)
			return (v.y > 0) ? 90 : (v.y == 0) ? 0 : 270;
		else if (v.y == 0)
			return (v.x >= 0) ? 0 : 180;

		int ret = static_cast<int>(vector_to_rad(v));
		
		if (v.x < 0 && v.y < 0)
			ret = 180 + ret;
		else if (v.x < 0)
			ret = 180 + ret;
		else if (v.y < 0)
			ret = 270 + (90 + ret);
		return ret;
	}

	// https://github.com/marukrap/Woozoolike/blob/master/7DRL%202017/Utility.cpp#L46
	inline void draw_wall(Map& map, const v2 start, const v2 end)
	{
		const v2 delta(std::abs(end.x - start.x), -std::abs(end.y - start.y));
		const v2 dir(start.x < end.x ? 1 : -1, start.y < end.y ? 1 : -1);

		v2 p1 = start;

		int slope_error = delta.x + delta.y;

		map.get_tile(p1).character = 'G';
		while (p1 != end)
		{
			const int e2 = slope_error * 2;

			if (e2 >= delta.y)
			{
				slope_error += delta.y;
				p1.x += dir.x;
				map.get_tile(p1).character = 'G';
			}

			if (e2 <= delta.x)
			{
				slope_error += delta.x;
				p1.y += dir.y;
				map.get_tile(p1).character = 'G';
			}
			
		}
	};

	void voronoi(Map& map, std::vector<v2>& points)
	{
		std::vector<std::pair<v2, int>> regions;

		for (const auto& point : points)
			regions.emplace_back(point, 1);

		for (int y = 0; y < map.size.y; ++y)
		{
			for (int x = 0; x < map.size.x; ++x)
			{
				const v2 pos(x, y);
				int nearest = -1;
				int nearestDistance = std::numeric_limits<int>::max();

				for (int i = 0; i < points.size(); ++i)
				{
					const int distance = RLUtil::euclidean(points[i], pos);

					if (distance < nearestDistance)
					{
						nearest = i;
						nearestDistance = distance;
					}
				}

				regions[nearest].first += pos;
				regions[nearest].second += 1;
			}
		}

		for (size_t i = 0; i < points.size(); ++i)
		{
			std::cout << regions[i].first.x << "," << regions[i].first.y << " : " << regions[i].second << '\n';
			points[i] = regions[i].first / regions[i].second;
		}
		
		for (const v2& pos : points)
			map.get_tile(pos).character = 'b';

		// mst
		// std::vector<v2> connected;

		// connected.emplace_back(points.back());
		// points.pop_back();

		// while (!points.empty())
		// {
		// 	v2 bestFrom;
		// 	int bestToIndex = -1;
		// 	int bestDistance = std::numeric_limits<int>::max();

		// 	for (const auto& from : connected)
		// 	{
		// 		for (std::size_t i = 0; i < points.size(); ++i)
		// 		{
		// 			int distance = RLUtil::euclidean(points[i], from);

		// 			if (distance < bestDistance)
		// 			{
		// 				bestFrom = from;
		// 				bestToIndex = i;
		// 				bestDistance = distance;
		// 			}
		// 		}
		// 	}

		// 	v2 to = points[bestToIndex];

		// 	RLUtil::draw_wall(map, bestFrom, to);
			
		// 	connected.emplace_back(to);
		// 	points.erase(points.begin() + bestToIndex);
		// }
	}
};