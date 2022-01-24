#include <iostream>
#include <cstdlib>
#include <algorithm>
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
void DrawBoxRooms(std::vector<char>& map, int w, int h)
{
    // random boxes
    const int map_cent_x = (w-1)/2;
    const int map_cent_y = (h-1)/2;
    const int box_width = 5;
    const int box_height = 5;

    // const int pos_x = std::rand() % (w-1);
    const int pos_x = map_cent_x;
    const int pos_y = map_cent_y;

    const int box_x_min = std::clamp(pos_x - (int)std::floor(box_width/2.0), 0, w);
    const int box_x_max = std::clamp(pos_x + (int)std::round(box_width/2.0), 0, w);
    const int box_y_min = std::clamp(pos_y - (int)std::floor(box_height/2.0), 0, h);
    const int box_y_max = std::clamp(pos_y + (int)std::round(box_height/2.0), 0, h);

    const int actual_size_x = box_x_max - box_x_min;
    const int actual_size_y = box_y_max - box_y_min;

    for (size_t i = 0; i < actual_size_x * actual_size_y; i++)
    {
        map[ box_x_min+(i%actual_size_x) + (box_y_min+(i/actual_size_x))*w ] = '#';
    }
}

// templated rooms with herringbone patterns
void DrawTemplatedFloor()
{

}

// Cellular automata rooms
void Cave()
{

}

std::vector<char> Generate(const int w, const int h)
{
    std::vector<char> map(w*h);

    size_t map_size = w*h;
    
    for (size_t i = 0; i < map_size; i++)
        map[i]='.';

    DrawBoxRooms(map, w, h);
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
