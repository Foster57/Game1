#include "Mixer.h"

Mix_Music* SoundManager::bgMusic = nullptr;
Mix_Chunk* SoundManager::effect = nullptr;

bool SoundManager::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer lỗi: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void SoundManager::playBackgroundMusic(const std::string& path) {
    bgMusic = Mix_LoadMUS(path.c_str());
    if (!bgMusic) {
        std::cout << "Không load được nhạc nền: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayMusic(bgMusic, -1); // lặp vô hạn
}

void SoundManager::playSoundEffect(const std::string& path) {
    effect = Mix_LoadWAV(path.c_str());
    if (!effect) {
        std::cout << "Không load được hiệu ứng âm thanh: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayChannel(-1, effect, 0);
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}

void SoundManager::setMusicVolume(int volume) {
    // Đảm bảo âm lượng nằm trong khoảng hợp lệ (0 đến MIX_MAX_VOLUME)
    if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
    Mix_VolumeMusic(volume);
}

void SoundManager::clean() {
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr;
    }
    if (effect) {
        Mix_FreeChunk(effect);
        effect = nullptr;
    }
    Mix_CloseAudio();
}
