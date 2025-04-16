
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();
    bool init(const std::string& fontPath, int fontSize);
    void update(Uint32 startTime);
    void render(SDL_Renderer* renderer);
    void clean();

private:
    int score;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    TTF_Font* font;
    SDL_Color textColor;
};
extern ScoreManager scoreManager;


#endif
