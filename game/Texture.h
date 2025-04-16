// Texture.h
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class TextureManager {
public:
    static SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* renderer);
};

#endif

