#pragma once

#include <array>
#include "v2.h"

class Directions
{
public:
    Directions() : Directions( {0, 0}, 10.0f ) {};
    Directions(v2 in_dir, float cost) : dir(in_dir), move_cost(cost) {};
    v2 dir;
    float move_cost;

    static const Directions nw;
    static const Directions n;
    static const Directions ne;
    static const Directions w;
    static const Directions e;
    static const Directions sw;
    static const Directions s;
    static const Directions se;

    static const std::array<Directions, 8> OCT_DIRECTIONS;
};


inline const float nw_cost = 15.0f;
inline const float n_cost = 10.0f;
inline const float ne_cost = 15.0f;
inline const float w_cost = 10.0f;
inline const float e_cost = 10.0f;
inline const float sw_cost = 15.0f;
inline const float s_cost = 10.0f;
inline const float se_cost = 15.0f;
inline const Directions Directions::nw =    {{-1 ,-1}, 15.0f};
inline const Directions Directions::n =     {{0  ,-1}, 10.0f};
inline const Directions Directions::ne =    {{1  ,-1}, 15.0f};
inline const Directions Directions::w =     {{1  ,0}, 10.0f};
inline const Directions Directions::e =     {{-1 ,0}, 10.0f};
inline const Directions Directions::sw =    {{-1 ,1}, 15.0f};
inline const Directions Directions::s =     {{0  ,1}, 10.0f};
inline const Directions Directions::se =    {{1  ,1}, 15.0f};

inline const std::array<Directions, 8> Directions::OCT_DIRECTIONS = { nw, n, ne, w, e, sw, s, se };