#pragma once

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

    v2 straight_dir() const
    {
        if (x == y)
            if (x == 0 && y == 0)
                return v2(0, 0);
            else
                return v2(1, 1);

        if (x > y)
            return v2(1, 0);
        else
            return v2(0, 1);
    }

    int max() const
    {
        if (x > y)
            return x;
        else
            return y;
    }
};
