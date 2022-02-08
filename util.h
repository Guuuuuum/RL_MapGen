#pragma once

namespace RLUtil
{

inline int distance_RL(const v2& start, const v2& end)
{
    const v2 delta = { std::abs(end.x - start.x), std::abs(end.y - start.y) };
    return std::max(delta.x, delta.y);
};

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

};