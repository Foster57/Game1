
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();
    bool init(const std::string& fontPath, int fontSize);
    void update(Uint32 startTime);
    void render(SDL_Renderer* renderer);
    void clean();
    void saveHighScore(const std::string& file);
    void loadHighScore(const std::string& file);
    int getHighScore() const;
    TTF_Font* getFont() const { return font; }

private:
    int score;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    TTF_Font* font;
    SDL_Color textColor;
    int highScore;
};
extern ScoreManager scoreManager;


#endif
