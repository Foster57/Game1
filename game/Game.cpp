#include "Game.h"
#include "Texture.h"
#include "ScoreManager.h"
#include "Mixer.h"
#include <string>

Game::Game()
    : window(nullptr), renderer(nullptr), background(nullptr), playerTexture(nullptr),
      obstacleTexture(nullptr), gameOverTexture(nullptr), menuBackground(nullptr),
      startButtonTexture(nullptr), frame(0), lastSpawnTime(0), obstacleBaseSpeed(0),
      startTime(0), moveLeft(false), moveRight(false), isRunning(false),
      isGameOver(false), isMenu(true), musicVolume(64) {
}

Game::~Game() {}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Khởi tạo font
    if (!scoreManager.init("pixel.ttf", 24)) {
        return false;
    }
    scoreManager.loadHighScore("../Pic and mixer/highscore.txt");


    menuBackground = TextureManager::loadTexture("../Pic and mixer/menu_background.png", renderer);
    startButtonTexture = TextureManager::loadTexture("../Pic and mixer/start_button.png", renderer);
    volumeUpTexture = TextureManager::loadTexture("../Pic and mixer/volume_up.png", renderer);
    volumeDownTexture = TextureManager::loadTexture("../Pic and mixer/volume_down.png", renderer);

    if (!volumeUpTexture || !volumeDownTexture) {
        return false;
    }

volumeUpRect = { 650, 500, 64, 64 };      // Tùy chỉnh vị trí
volumeDownRect = { 550, 500, 64, 64 };


    if (!menuBackground || !startButtonTexture) {
        return false;
    }
    if (!SoundManager::init()) {
        std::cout << "Không thể khởi tạo âm thanh!" << std::endl;
        return false;
    }
    SoundManager::playBackgroundMusic("../Pic and mixer/musicbackground.mp3");
    SoundManager::setMusicVolume(musicVolume);


    background = TextureManager::loadTexture("../Pic and mixer/background.png", renderer);
    playerTexture = TextureManager::loadTexture("../Pic and mixer/knight1.png", renderer);
    obstacleTexture = TextureManager::loadTexture("../Pic and mixer/dan1.png", renderer);
    gameOverTexture = TextureManager::loadTexture("../Pic and mixer/gameover.png", renderer);
    if (!gameOverTexture) {
        return false;
    }

    bg1 = {0, 0, width, height};
    bg2 = {width, 0, width, height};

    character = {{100, GROUND_Y - FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT}, 0, false};

    obstacle = {{rand() % (SCREEN_WIDTH - 25), -50, 75, 75}, 5}; // spawn trên đầu, rơi xuống

    isRunning = true;

    return true;
}

// Xử lí các nút
void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        if (isMenu) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        // Bấm start
        if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
            y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
            isMenu = false;
            startTime = SDL_GetTicks();
        }

        // Bấm tăng âm lượng
        if (x >= volumeUpRect.x && x <= volumeUpRect.x + volumeUpRect.w &&
            y >= volumeUpRect.y && y <= volumeUpRect.y + volumeUpRect.h) {
            musicVolume = std::min(musicVolume + 8, MIX_MAX_VOLUME);
            SoundManager::setMusicVolume(musicVolume);
        }

        // Bấm giảm âm lượng
        if (x >= volumeDownRect.x && x <= volumeDownRect.x + volumeDownRect.w &&
            y >= volumeDownRect.y && y <= volumeDownRect.y + volumeDownRect.h) {
            musicVolume = std::max(musicVolume - 8, 0);
            SoundManager::setMusicVolume(musicVolume);
        }
    }

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_PLUS:
            case SDLK_EQUALS:
                musicVolume = std::min(musicVolume + 8, MIX_MAX_VOLUME);
                SoundManager::setMusicVolume(musicVolume);
                break;
            case SDLK_MINUS:
                musicVolume = std::max(musicVolume - 8, 0);
                SoundManager::setMusicVolume(musicVolume);
                break;
        }
    }

    return;  // ✅ giữ lại, nhưng đã xử lý volume trước đó rồi
}


        if (isGameOver) return;

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    moveLeft = true;
                    characterDirection = LEFT;
                    break;
                case SDLK_RIGHT:
                    moveRight = true;
                    characterDirection = RIGHT;
                    break;
                case SDLK_UP:
                    if (!character.isJumping) {
                        character.velocityY = JUMP_FORCE;
                        character.isJumping = true;
                    }
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    moveLeft = false;
                    break;
                case SDLK_RIGHT:
                    moveRight = false;
                    break;
            }
        }
    }
}



void Game::update() {
    if (isGameOver) return;
    bg1.x -= 2;
    bg2.x -= 2;

    if (bg1.x + bg1.w <= 0) bg1.x = bg2.x + bg2.w;
    if (bg2.x + bg2.w <= 0) bg2.x = bg1.x + bg1.w;

    character.velocityY += GRAVITY;
    character.rect.y += (int)character.velocityY;

    if (character.rect.y + character.rect.h >= GROUND_Y) {
        character.rect.y = GROUND_Y - character.rect.h;
        character.velocityY = 0; // Reset lại vận tốc rơi
        character.isJumping = false;
    }
    // Tạo hitbox nhỏ hơn nhân vật
    SDL_Rect characterHitbox = {
        character.rect.x + 5,
        character.rect.y + 5,
        character.rect.w - 20,
        character.rect.h - 20
    };

    playerSrc = {
        frame * FRAME_WIDTH,
        0, // vì chỉ có 1 hàng
        FRAME_WIDTH,
        FRAME_HEIGHT
    };
    Uint32 currentTime = SDL_GetTicks();
    // Tăng tốc độ dần dần theo thời gian (ví dụ mỗi 10 giây)
    scoreManager.update(startTime);

    float timeInSeconds = SDL_GetTicks() / 1000.0f;
    obstacleBaseSpeed = 2.0f + timeInSeconds / 10.0f;

    if (currentTime - lastSpawnTime > 5000) { // spawn mỗi 5s
        if (currentTime - lastSpawnTime > 5000) { // mỗi 5s
    int count = 1 + rand() % 5; // số lượng obstacle sinh ra

    for (int i = 0; i < count; ++i) {
        bool valid = false;
        SDL_Rect newRect;
        int attempts = 0;

        while (!valid && attempts < 20) {
            newRect = { rand() % (SCREEN_WIDTH - 75), -50, 75, 75 };
            valid = true;

            for (const auto& existing : obstacles) {
                if (SDL_HasIntersection(&newRect, &existing.rect)) {
                    valid = false;
                    break;
                }
            }

            attempts++;
        }

        if (valid) {
            Obstacle obs;
            obs.rect = newRect;
            obs.speed = obstacleBaseSpeed + rand() % 3;
            obstacles.push_back(obs);
        }
    }

    lastSpawnTime = currentTime;
}

    }

    // Xóa obstacle nếu ra khỏi màn hình
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& o) {
        return o.rect.y > SCREEN_HEIGHT;
    }), obstacles.end());
    if (moveLeft) {
        character.rect.x = std::max(0, character.rect.x - 10);
        frame = (frame + 1) % 4;
    }
    if (moveRight) {
        character.rect.x = std::min(SCREEN_WIDTH - FRAME_WIDTH, character.rect.x + 10);
        frame = (frame + 1) % 4;
    }
    if (!isGameOver) {
        for (auto& obs : obstacles) {
            obs.rect.y += obs.speed;

            // Va chạm
            if (SDL_HasIntersection(&characterHitbox, &obs.rect)) {
                isGameOver = true;
                SoundManager::stopMusic();
                break;
            }
        }

        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& o) {
            return o.rect.y > SCREEN_HEIGHT;
        }), obstacles.end());
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, nullptr, &bg1);
    SDL_RenderCopy(renderer, background, nullptr, &bg2);
    SDL_RendererFlip flip = characterDirection == LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, playerTexture, &playerSrc, &character.rect, 0, nullptr, flip);

    for (const auto& obs : obstacles) {
        SDL_RenderCopy(renderer, obstacleTexture, nullptr, &obs.rect);
    }
    if (isGameOver && gameOverTexture) {
        SDL_Rect gameOverRect = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, 400, 400 };
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    }
    if (isMenu) {
        SDL_RenderCopy(renderer, menuBackground, nullptr, nullptr);
        SDL_RenderCopy(renderer, startButtonTexture, nullptr, &startButtonRect);
        SDL_RenderCopy(renderer, volumeUpTexture, nullptr, &volumeUpRect);
        SDL_RenderCopy(renderer, volumeDownTexture, nullptr, &volumeDownRect);
        SDL_RenderPresent(renderer);
        return;
    }
    std::string hsText = "High Score: " + std::to_string(scoreManager.getHighScore());
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(scoreManager.getFont(), hsText.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect hsRect = { 10, 50, surface->w, surface->h };  // Vị trí tùy chỉnh
    SDL_RenderCopy(renderer, texture, nullptr, &hsRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    scoreManager.render(renderer);
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(startButtonTexture);
    scoreManager.saveHighScore("../Pic and mixer/highscore.txt");
    SoundManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
