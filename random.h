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
    
    int32_t get_rand(int32_t min, int32_t max)
    {
        return min + std::uniform_int_distribution<>(0, max - min - 1)(rand_method);
    }

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
