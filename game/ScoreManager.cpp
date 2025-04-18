
#include "ScoreManager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>

ScoreManager::ScoreManager() : score(0), scoreTexture(nullptr), font(nullptr) {
    textColor = {255, 255, 255, 255}; // Black color
}

ScoreManager::~ScoreManager() {
    clean();
}

bool ScoreManager::init(const std::string& fontPath, int fontSize) {
    if (TTF_Init() == -1) return false;

    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    return font != nullptr;
}


void ScoreManager::update(Uint32 startTime) {
    Uint32 currentTime = SDL_GetTicks();
    score = (currentTime - startTime) / 1000;
}

void ScoreManager::render(SDL_Renderer* renderer) {
    if (!font) return;

    std::stringstream ss;
    ss << "Score: " << score;
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, ss.str().c_str(), textColor);
    if (!textSurface) return;

    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    scoreRect = {10, 10, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
}

void ScoreManager::clean() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
}
