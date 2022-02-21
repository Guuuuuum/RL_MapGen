#pragma once

#include <algorithm>
#include <assert.h>

class v2
{
public:
    v2() : v2(0, 0){};
    v2(int in_x, int in_y) : x(in_x), y(in_y){};
    int x;
    int y;

	v2 operator-() const
    {
        return v2(-x, -y);
    };
    
	v2 operator+(const v2& rh) const
    {
        return v2(x + rh.x, y + rh.y);
    };

	v2 operator-(const v2& rh) const
    {
        return v2(x - rh.x, y - rh.y);
    };

	v2 operator*(const v2& rh) const
    {
        return v2(x * rh.x, y * rh.y);
    };

	v2 operator/(const v2& rh) const
    {
        return v2(x / rh.x, y / rh.y);
    };

	v2 operator/(const int d) const
    {
        return v2(x / d, y / d);
    };

	bool operator==(const v2& rh) const
    {
        return (rh.x == x && rh.y == y);
    };

	bool operator!=(const v2& rh) const
    {
        return !(rh.x == x && rh.y == y);
    };

    bool operator<(const v2& rh) const
    {
        return std::sqrt(x*x + y*y) < std::sqrt(rh.x*rh.x + rh.y*rh.y);
    };

    int operator[](const int index) const
    {
        assert(index <= 1 && index >= 0);

        if (index == 0)
            return x;
        else
            return y;
    };

    v2& operator+=(const v2& rh)
    {
        x += rh.x;
        y += rh.y;
        return *this;
    }

    const v2 flatten_max() const
    {
        if (x == y)
        {
            if (x == 0)
                return v2(0, 0);
            else
                return v2(std::clamp(x, -1, 1), 0);
                // return v2(std::clamp(x, -1, 1), std::clamp(y, -1, 1));
        }

        if (std::abs(x) > std::abs(y))
            return v2(std::clamp(x, -1, 1), 0);
        else
            return v2(0, std::clamp(y, -1, 1));
    }

    const v2 flatten_min() const
    {
        if (x == y)
        {
            if (x == 0)
                return v2(0, 0);
            else
                return v2(0, std::clamp(y, -1, 1));
                // return v2(std::clamp(x, -1, 1), std::clamp(y, -1, 1));
        }

        if (std::abs(x) < std::abs(y))
            return v2(std::clamp(x, -1, 1), 0);
        else
            return v2(0, std::clamp(y, -1, 1));
    }

    const v2 flatten_index(int index) const
    {
        if (index == 0)
            return v2(std::clamp(x, -1, 1), 0);
        else
            return v2(0, std::clamp(y, -1, 1));
    }

    const int max_index() const
    {
        if (x > y)
            return 0;
        else
            return 1;
    }

    const int min_index() const
    {
        if (x < y)
            return 0;
        else
            return 1;
    }
};
