#include "Mixer.h"
#include <algorithm>
Mix_Music* SoundManager::bgMusic = nullptr;
Mix_Chunk* SoundManager::footstepSound = nullptr;



bool SoundManager::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer lỗi: " << Mix_GetError() << std::endl;
        return false;
    }

    footstepSound = Mix_LoadWAV("../Pic and mixer/footstep.wav");
    if (!footstepSound) {
        std::cout << "Can't load footstepSound: " << Mix_GetError() << std::endl;
        return false;
    }

    // Tăng âm lượng bước chân lên tối đa (128)
    Mix_VolumeChunk(footstepSound, 128);

    return true;
}

// Music background
void SoundManager::playBackgroundMusic(const std::string& path) {
    bgMusic = Mix_LoadMUS(path.c_str());
    if (!bgMusic) {
        std::cout << "Không load được nhạc nền: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayMusic(bgMusic, -1); // lặp vô hạn
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}
// Khoi chay foot step
void SoundManager::playFootstep() {
    if (footstepSound) {
        Mix_PlayChannel(-1, footstepSound, 0);
    }
}


void SoundManager::setMusicVolume(int volume) {
    volume = std::clamp(volume, 0, MIX_MAX_VOLUME); //
    Mix_VolumeMusic(volume);
}


void SoundManager::clean() {
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr;
    }
    if (footstepSound){
        Mix_FreeChunk(footstepSound);
        footstepSound = nullptr;
    }
    Mix_CloseAudio();
}
