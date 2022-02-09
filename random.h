#pragma once

#include <iostream>
#include <random>
#include "v2.h"

class Random
{
public:
    Random(uint32_t in_seed = std::random_device()()) : seed(in_seed), rand_method(in_seed)
    {
    };

    int32_t get_rand(int32_t max)
    {
        return std::uniform_int_distribution<>(0, max - 1)(rand_method);
    };
    
    v2 get_room_outline_rand(v2& size)
    {
        std::vector<v2> outline(size.x*2 + size.y*2 -4);

        for (int i = 0; i < size.x; i++)
        {
            outline.emplace_back(i, 0);
            outline.emplace_back(i, std::clamp(size.y-1, 0, size.y));
        }

        for (int i = 1; i < size.y-1; i++)
        {
            outline.emplace_back(0, i);
            outline.emplace_back(std::clamp(size.x-1, 0, size.x), i);
        }
        
        assert(size.x*2 + size.y*2 -4 != outline.size());
        
        return outline[ get_rand(static_cast<int32_t>(outline.size())) ];
    };

    template <typename T>
    std::vector<T> pick_num(std::vector<T> v, const int num)
    {
        const size_t size = v.size();
        assert(num < size);

        std::shuffle(v.begin(), v.end(), rand_method);

        return {v.begin(), v.begin() + num};
    };
    
    template <typename T>
    T pick_one(std::vector<T> v)
    {
        return v[get_rand(static_cast<int32_t>(v.size()))];
    };

private:
	const uint32_t seed;
	std::mt19937 rand_method;
};
