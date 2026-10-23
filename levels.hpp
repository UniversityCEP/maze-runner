#ifndef LEVELS_HPP
#define LEVELS_HPP
#include <SFML/Graphics.hpp>
using namespace sf;
void LevelWindow(RenderWindow& window);
struct LevelNode{
    int levelNumber;
    bool unlocked;
    bool completed;
    LevelNode* next;
};
void intializeLevels();
bool isLevelUnlocked(int levelNumber);
bool isLevelCompleted(int levelNumber);
void unlockNextLevel(int completeLevel);

#endif
