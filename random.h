#pragma once

#include <iostream>
#include <random>

class Random
{
public:
    Random(uint32_t in_seed = std::random_device()()) : seed(in_seed), rand_method(in_seed)
    {
    };

    int get_rand(int max)
    {
        return std::uniform_int_distribution<>(0, max - 1)(rand_method);
    };
    
private:
	unsigned int seed;
	std::mt19937 rand_method;
};
