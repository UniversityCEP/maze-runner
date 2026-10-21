#ifndef MAZE_HPP
#define MAZE_HPP
#include <SFML/Graphics.hpp>
#include<string>
using namespace sf;
using namespace std;

void runmaze(RenderWindow& window,int rows,int cols,float cellsize, bool timerenabled, bool enableMonsters, const string& BackgroundPath, int level);

#endif
