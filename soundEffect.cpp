#include <SDL3_mixer/SDL_mixer.h>
#include "./soundEffect.h"



SoundEffect::SoundEffect(MIX_Track* channel, MIX_Audio* sound) : m_channel{channel}, m_sound{sound} {
};


// Play sound effect. Interrupts any sound currently playing on the channel.
void SoundEffect::forcePlay() const {
    MIX_SetTrackAudio(m_channel, m_sound);
    constexpr SDL_PropertiesID properties {0};
    MIX_PlayTrack(m_channel, properties);
};

// Play the sound effect if the channel isn't currently busy.
void SoundEffect::deferPlay() const {
    if (MIX_TrackPlaying(m_channel)) {
        return;
    }
    forcePlay();
};

// Plays the sound effect the first time this method is called. (Interrupting any other sound on the channel.)
// Subsequent calls to this method will do nothing until resetPlayed is called.
void SoundEffect::playOnce() {
    if (m_playedOnce) {
        return;
    }

    m_playedOnce = true;
    forcePlay();
};

// Reset the sound effect's state so that playOnce can be called again.
void SoundEffect::resetPlayed() {
    m_playedOnce = false;
};