#pragma once

#include <algorithm>

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

    const int max() const
    {
        if (x > y)
            return x;
        else
            return y;
    }

    const int min() const
    {
        if (x < y)
            return x;
        else
            return y;
    }

    const int abs_min() const
    {
        if (std::abs(x) < std::abs(y))
            return std::abs(x);
        else
            return std::abs(y);
    }

    const int abs_max() const
    {
        if (std::abs(x) < std::abs(y))
            return std::abs(y);
        else
            return std::abs(x);
    }
};
