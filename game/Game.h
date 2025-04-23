// Game.h
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include <vector>
//#include <cstdlib>
//#inlcude <ctime>
#include "ScoreManager.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GROUND_Y = 600;
const int FRAME_WIDTH = 68;
const int FRAME_HEIGHT = 100;
const int GRAVITY = 6;
const int JUMP_FORCE = -50;



class Game {
public:
    Game();
    ~Game();
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() { return isRunning; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* menuBackground = nullptr;
    SDL_Texture* startButtonTexture = nullptr;
    SDL_Rect startButtonRect = { SCREEN_WIDTH / 2 -135, SCREEN_HEIGHT / 2 - 50, 300, 200 };
    SDL_Texture* background;
    SDL_Texture* playerTexture;
    SDL_Texture* obstacleTexture;
    SDL_Texture* gameOverTexture;
    SDL_Rect bg1, bg2;
    SDL_Rect playerSrc, playerDst;
    SDL_Texture* volumeUpTexture;
    SDL_Texture* volumeDownTexture;
    SDL_Rect volumeUpRect;
    SDL_Rect volumeDownRect;

    int frame;
    float obstacleBaseSpeed = 2.0f; // tốc độ cơ bản
    struct Character {
        SDL_Rect rect;
        float velocityY;
        bool isJumping;
    } character;
    struct Obstacle {
        SDL_Rect rect;
        int speed;
    } obstacle;

    enum Direction { RIGHT, LEFT };
    Direction characterDirection = RIGHT;
    std::vector<Obstacle> obstacles;
    bool isRunning;
    Uint32 lastSpawnTime = 0;
    Uint32 startTime = 0;
    bool moveLeft = false;
    bool moveRight= false;
    bool isGameOver = false;
    bool isMenu = true;
    int musicVolume;

};


#endif

