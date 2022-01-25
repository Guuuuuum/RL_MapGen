#pragma once

#include <array>
#include "v2.h"

class Directions
{
public:
    Directions() : Directions( {0, 0} ) {};
    Directions(v2 in_dir) : dir(in_dir){};
    v2 dir;

    static const Directions none;
    static const Directions nw;
    static const Directions n;
    static const Directions ne;
    static const Directions w;
    static const Directions e;
    static const Directions sw;
    static const Directions s;
    static const Directions se;

    static const std::array<Directions, 8> dir_all;
};

inline const Directions Directions::none =  {{0  ,0}};
inline const Directions Directions::nw =    {{-1 ,-1}};
inline const Directions Directions::n =     {{0  ,-1}};
inline const Directions Directions::ne =    {{1  ,-1}};
inline const Directions Directions::w =     {{1  ,0}};
inline const Directions Directions::e =     {{-1 ,0}};
inline const Directions Directions::sw =    {{-1 ,1}};
inline const Directions Directions::s =     {{0  ,1}};
inline const Directions Directions::se =    {{1  ,1}};

inline const std::array<Directions, 8> Directions::dir_all = { nw, n, ne, w, e, sw, s, se };