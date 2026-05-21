#ifndef SOUNDS_LOADER_H
#define SOUNDS_LOADER_H


#include "../sdlUtils/audioManager.h"
#include "../sdlUtils/soundEffect.h"


class Sounds {
public:
    SoundEffect ballBounce{};

    Sounds(AudioManager& audioManager) {
        ballBounce = audioManager.getSoundEffect("BALL", "BALL_SOUND");
    };
};



#endif