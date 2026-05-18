#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H


#include <string>
#include <unordered_map>
#include <vector>
#include <SDL3_mixer/SDL_mixer.h>
#include "./sdlUtils/soundEffect.h"


class AudioManager {
    inline static bool created {false};

    MIX_Mixer* m_mixer {nullptr};

    MIX_Track* m_musicChannel {nullptr};
    MIX_Audio* m_musicAudio {nullptr};

    SDL_Event m_musicEndEvent{};

    int m_gameMusicTrackIdx {0};
    int m_gameMusicLoopFadeInMs {0};
    bool m_usingGameMusicLoopCb {false};
    std::vector<std::string> m_gameMusicFilePaths{};
    std::string m_menuMusicFilePath{};

    float m_soundVolume {1.0f};
    std::unordered_map<std::string, MIX_Track*> m_channelsMap{};
    std::unordered_map<std::string, MIX_Audio*> m_soundsMap{};
    
    std::string m_saveFilePath{};



public:
    AudioManager(
        const std::vector<std::string>& channels, 
        const std::unordered_map<std::string, std::string>& soundFilepaths,
        const std::vector<std::string>& gameMusicFilePaths={},
        const std::string& menuMusicFilePath="",
        const std::string& saveFilePath=""
    );

    explicit AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

    ~AudioManager();

    
    // Load the audio data from the given file path and binds it to the music channel.
    // This will unload any previously loaded audio in the mucic channel.
    // Throws an std::runtime_error if unable to load the audio. 
    void loadMusic(const std::string& musicFilePath);
    
    // Unloads any audio loaded via the loadMusic method.
    // This will trigger the given music end event if one was set and mucis was currently playing.
    void unloadMusic();
    
    // Play the current loaded audio in the music channel. This will restart any currently playing audio.
    void playMusic(int loops=0, int startMs=0, int fadeInMs=0);
    
    // Stops any currently playing audio in the music channel. This will trigger the music end event if one was set.
    void stopMusic();

    // Pause the currently playing music. This will nto trigger any music end event.
    void pauseMusic();
    
    // Resume paused music.
    void resumeMusic();
    
    // Query if the music channel is currently playing music.
    bool musicPlaying() const;

    // Get the current volume of the music channel.
    float getMusicVolume() const;
    
    // Set the volume of the music channel. Clamps the passed value to 0-1.
    void setMusicVolume(float volume);

    // Set the event to be placed on the event queue when music finishes playing or is stopped.
    // Calling this method will after calling playGameMusicLoop will disrupt the loop.
    void setMusicEndEvent(const SDL_Event& event);

    // Unset any event set with setMusicEndEvent.
    // Calling this method will after calling playGameMusicLoop will disrupt the loop.
    void unsetMusicEndEvent();
    
    // Load and play the audio provided by the menuMusicFilePath param in the constructor.
    // Does nothing if no menuMusicFilePath was provided.
    void playMenuMusic(int loops=0, int startMs=0, int fadeInMs=0);

    // Play a music track from the gameMusicFilePaths passed on construction. 
    // Tracks will be in the same order as given. Does nothing if are no gameMusicFilePaths.
    void playGameMusicTrack(int trackIdx, int loops=0, int startMs=0, int fadeInMs=0);

    // Play the game music (given via gameMusicFilePaths) on a loop starting at the given index.
    // Does nothing if there are no gameMusicFilePaths. 
    void playGameMusicLoop(int startingTrackIdx=0, int fadeInMs=0);

    // Get a sound effect with the specified channel and sound.
    // Throws an std::runtime_error if the channel or sound name don't exist.
    SoundEffect getSoundEffect(const std::string& channelName, const std::string& soundName);

    // Get the current sound volume.
    float getSoundVolume() const;

    // Set the current sound volume. The volume is clamped to 0-1.
    void setSoundVolume(float volume);

    // Save the current volume settings to file if a save path was given at construction.
    void saveVolumeSettings();

    
    
private:
    void loadSavedVolumeSettings();

    void createChannelsAndSounds(
        const std::vector<std::string>& channels, 
        const std::unordered_map<std::string, std::string>& soundFilepaths
    );

    float clampVolume(float volume);

    void unsetGameMusicLoopCb();

    void _loadMusic(const std::string& musicFilePath);

    void _unloadMusic();

    void _playMusic(int loops=0, int startMs=0, int fadeInMs=0);

    void _stopMusic();

    void _playGameMusicTrack(int trackIdx, int loops=0, int startMs=0, int fadeInMs=0);


    static void musicEndEventCallback(void* instancePtr, MIX_Track*);
    static void gameMusicTrackEndCallback(void* instancePtr, MIX_Track*);
};



#endif