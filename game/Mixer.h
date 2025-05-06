#ifndef MIXER_H_
#define MIXER_H_

#include <SDL_mixer.h>
#include <string>
#include <iostream>


class SoundManager {
public:
    static bool init();
    static void playBackgroundMusic(const std::string& path);
    static void setMusicVolume(int volume);
    static void playFootstep();
    static void stopMusic();
    static void clean();


private:
    static Mix_Music* bgMusic;
    static Mix_Chunk* footstepSound; // tieng buoc chan
    static int currentVolume;

};

#endif
