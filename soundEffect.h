#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H


#include <SDL3_mixer/SDL_mixer.h>


class SoundEffect {
    friend class AudioManager;

    MIX_Track* m_channel {nullptr};
    MIX_Audio* m_sound {nullptr};

    bool m_playedOnce {false};


    SoundEffect(MIX_Track* channel, MIX_Audio* sound);


public:
    SoundEffect(const SoundEffect&) = default;
    SoundEffect(SoundEffect&&) = default;
    ~SoundEffect() = default;
    SoundEffect& operator=(const SoundEffect&) = default;
    SoundEffect& operator=(SoundEffect&&) = default;


    // Play sound effect. Interrupts any sound currently playing on the channel.
    void forcePlay() const;

    // Play the sound effect if the channel isn't currently busy.
    void deferPlay() const;

    // Plays the sound effect the first time this method is called. (Interrupting any other sound on the channel.)
    // Subsequent calls to this method will do nothing until resetPlayed is called.
    void playOnce();

    // Reset the sound effect's state so that playOnce can be called again.
    void resetPlayed();
};



#endif