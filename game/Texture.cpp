
// Texture.cpp
#include "Texture.h"

SDL_Texture* TextureManager::loadTexture(const char* fileName, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    if (!tempSurface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    // Xóa nền trắng của nhân vật (nếu có)
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 255, 255));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return texture;
}
