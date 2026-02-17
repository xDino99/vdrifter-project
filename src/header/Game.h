#pragma once
#include <vector>
#include <string>
#include <random>
#include <atomic>
#include <signal.h>
#include "Terminal.h"
#include "Entity.h"

enum GameState { MENU, PLAY, SETTINGS, GAMEOVER, EXIT };
enum Difficulty { EASY, HARD, NIGHTMARE };
enum Language { LANG_EN, LANG_ES, LANG_PT };

class Game {
    Terminal term;
    GameState state;
    Difficulty difficulty;
    Language lang;
    char shipModel;

    Entity player;
    std::vector<Entity> bullets;
    std::vector<Entity> enemies;
    std::vector<Entity> particles;
    std::vector<Vec2> stars;

    long score;
    int health;
    int frames;

    std::mt19937 rng;

    static volatile sig_atomic_t resize_flag;

public:
    Game();
    void run();
    static void handle_resize(int sig);

private:
    void init();
    void processInput();
    void update();
    void render();
    void spawnBullet();
    void spawnEnemy();
    void createExplosion(float x, float y);
    std::string getStr(int index);
};