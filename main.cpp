#include <iostream>
#include <tuple>
#include <vector>

bool IsBound(int w, int h, int x, int y)
{
    if (x == 0 || x == w - 1)
        return true;

    if (y == 0 || y == h - 1)
        return true;

    return false;
}

void DrawBounds(std::vector<char>& map, int w, int h)
{
    size_t map_size = w*h;
    for (size_t i = 0; i < map_size; i++)
    {
        if (IsBound(w, h, i%w, i/w))
            map[i]='#';
    }
}

// http://roguebasin.com/index.php/Articles#Programming_languages
// symmetry Box Rooms
void DrawBoxRooms()
{

}

// templated rooms with herringbone patterns
void DrawTemplatedFloor()
{

}

// Cellular automata rooms
void Cave()
{

}

std::vector<char> Generate(int w, int h)
{
    std::vector<char> map(w*h);

    size_t map_size = w*h;
    
    for (size_t i = 0; i < map_size; i++)
        map[i]='.';

    DrawBounds(map, w, h);
    return map;
}

void PrintMap(std::vector<char>& map, int w)
{
    size_t map_size = map.size();
    for (size_t i = 0; i < map_size; i++)
    {
        std::cout << map[i];

        if ((i+1) % w == 0)
            std::cout << '\n';
    }
}

int main(int, char**) {

    PrintMap(Generate(80, 25), 80);

    return 0;
}
