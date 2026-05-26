#ifndef SOUNDS_LOADER_H
#define SOUNDS_LOADER_H


#include "../sdlUtils/audio/audioManager.h"
#include "../sdlUtils/audio/soundEffect.h"


class Sounds {
public:
    SoundEffect ballBounce{};

    Sounds(AudioManager& audioManager) {
        ballBounce = audioManager.getSoundEffect("BALL", "BALL_SOUND");
    };
};



#endif