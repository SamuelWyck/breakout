#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H


#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_error.h>
#include "./soundEffect.h"


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

    std::unordered_map<std::string, MIX_Track*> m_channelsMap{};
    std::unordered_map<std::string, MIX_Audio*> m_soundsMap{};
    


public:
    AudioManager(
        const std::vector<std::string>& channels, 
        const std::unordered_map<std::string, std::string>& soundFilepaths,
        const std::vector<std::string>& gameMusicFilePaths={},
        const std::string& menuMusicFilePath=""
    ) : m_gameMusicFilePaths{gameMusicFilePaths}, m_menuMusicFilePath{menuMusicFilePath} {
        if (AudioManager::created) {
            throw std::runtime_error("AudioManager already initialized.\n");
        }

        bool initSuccess {MIX_Init()};
        if (!initSuccess) {
            throw std::runtime_error(SDL_GetError());
        }

        m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!m_mixer) {
            throw std::runtime_error(SDL_GetError());
        }

        createChannelsAndSounds(channels, soundFilepaths);
        m_musicChannel = MIX_CreateTrack(m_mixer);

        AudioManager::created = true;
    };

    explicit AudioManager() : AudioManager({}, {}) {
    };

    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

    ~AudioManager() {
        for (auto& entry : m_channelsMap) {
            auto& [channelName, channelTrack] {entry};
            MIX_SetTrackAudio(channelTrack, nullptr);
            MIX_DestroyTrack(channelTrack);
        }

        for (auto& entry : m_soundsMap) {
            auto& [soundName, soundAudio] {entry};
            MIX_DestroyAudio(soundAudio);
        }

        MIX_SetTrackAudio(m_musicChannel, nullptr);
        MIX_DestroyTrack(m_musicChannel);
        MIX_DestroyAudio(m_musicAudio);

        MIX_DestroyMixer(m_mixer);
        MIX_Quit();
        AudioManager::created = false;
    };

    
    // Load the audio data from the given file path and binds it to the music channel.
    // This will unload any previously loaded audio in the mucic channel.
    // Throws an std::runtime_error if unable to load the audio. 
    void loadMusic(const std::string& musicFilePath) {
        unsetGameMusicLoopCb();
        _loadMusic(musicFilePath);
    };
    
    // Unloads any audio loaded via the loadMusic method.
    // This will trigger the given music end event if one was set and mucis was currently playing.
    void unloadMusic() {
        unsetGameMusicLoopCb();
        _unloadMusic();
    };
    
    // Play the current loaded audio in the music channel. This will restart any currently playing audio.
    void playMusic(int loops=0, int startMs=0, int fadeInMs=0) {
        unsetGameMusicLoopCb();
        _playMusic(loops, startMs, fadeInMs);
    };
    
    // Stops any currently playing audio in the music channel. This will trigger the music end event if one was set.
    void stopMusic() {
        unsetGameMusicLoopCb();
        _stopMusic();
    };

    // Pause the currently playing music. This will nto trigger any music end event.
    void pauseMusic() {
        MIX_PauseTrack(m_musicChannel);
    };
    
    // Resume paused music.
    void resumeMusic() {
        MIX_ResumeTrack(m_musicChannel);
    };
    
    // Query if the music channel is currently playing music.
    bool musicPlaying() const {
        return MIX_TrackPlaying(m_musicChannel);
    };

    // Get the current volume of the music channel.
    float getMusicVolume() const {
        return MIX_GetTrackGain(m_musicChannel);
    };
    
    // Set the volume of the music channel. Clamps the passed value to 0-1.
    void setMusicVolume(float volume) {
        if (volume < 0.0f) {
            volume = 0.0f;
        } else if (volume > 1.0f) {
            volume = 1.0f;
        }
        
        MIX_SetTrackGain(m_musicChannel, volume);
    };

    // Set the event to be placed on the event queue when music finishes playing or is stopped.
    // Calling this method will after calling playGameMusicLoop will disrupt the loop.
    void setMusicEndEvent(const SDL_Event& event) {
        m_musicEndEvent = event;
        MIX_SetTrackStoppedCallback(m_musicChannel, AudioManager::musicEndEventCallback, this);
    };

    // Unset any event set with setMusicEndEvent.
    // Calling this method will after calling playGameMusicLoop will disrupt the loop.
    void unsetMusicEndEvent() {
        m_musicEndEvent = SDL_Event{};
        MIX_SetTrackStoppedCallback(m_musicChannel, nullptr, nullptr);
    };
    
    // Load and play the audio provided by the menuMusicFilePath param in the constructor.
    // Does nothing if no menuMusicFilePath was provided.
    void playMenuMusic(int loops=0, int startMs=0, int fadeInMs=0) {
        unsetGameMusicLoopCb();
        if (m_menuMusicFilePath.size() == 0) {
            return;
        }

        _loadMusic(m_menuMusicFilePath);
        _playMusic(loops, startMs, fadeInMs);
    };

    // Play a music track from the gameMusicFilePaths passed on construction. 
    // Tracks will be in the same order as given. Does nothing if are no gameMusicFilePaths.
    void playGameMusicTrack(int trackIdx, int loops=0, int startMs=0, int fadeInMs=0) {
        unsetGameMusicLoopCb();
        _playGameMusicTrack(trackIdx, loops, startMs, fadeInMs);
    };

    // Play the game music (given via gameMusicFilePaths) on a loop starting at the given index.
    // Does nothing if there are no gameMusicFilePaths. 
    void playGameMusicLoop(int startingTrackIdx=0, int fadeInMs=0) {
        if (m_gameMusicFilePaths.size() == 0) {
            return;
        }

        if (startingTrackIdx < 0) {
            startingTrackIdx = 0;
        } else if (startingTrackIdx >= static_cast<int>(m_gameMusicFilePaths.size())) {
            startingTrackIdx %= static_cast<int>(m_gameMusicFilePaths.size());
        }
        m_gameMusicTrackIdx = startingTrackIdx;

        m_gameMusicLoopFadeInMs = fadeInMs;
        constexpr int loops {0};
        constexpr int startMs {0};
        _playGameMusicTrack(m_gameMusicTrackIdx, loops, startMs, m_gameMusicLoopFadeInMs);
        MIX_SetTrackStoppedCallback(m_musicChannel, AudioManager::gameMusicTrackEndCallback, this);

        m_usingGameMusicLoopCb = true;
    };

    // Get a sound effect with the specified channel and sound.
    // Throws an std::runtime_error if the channel or sound name don't exist.
    SoundEffect getSoundEffect(const std::string& channelName, const std::string& soundName) {
        if (!m_channelsMap.contains(channelName)) {
            throw std::runtime_error("Channel: '" + channelName + "' doesn't exist.\n");
        }
        if (!m_soundsMap.contains(soundName)) {
            throw std::runtime_error("Sound: '" + soundName + "' doesn't exist\n");
        }

        return SoundEffect{m_channelsMap[channelName], m_soundsMap[soundName]};
    };



private:
    void createChannelsAndSounds(
        const std::vector<std::string>& channels, 
        const std::unordered_map<std::string, std::string>& soundFilepaths
    ) {
        for (const std::string& channelName : channels) {
            MIX_Track* channelTrack {MIX_CreateTrack(m_mixer)};
            if (!channelTrack) {
                throw std::runtime_error(SDL_GetError());
            }

            m_channelsMap[channelName] = channelTrack;
        }


        constexpr bool decode {true};
        for (const auto& entry : soundFilepaths) {
            auto& [soundName, soundFilepath] {entry};
            MIX_Audio* soundAudio {MIX_LoadAudio(m_mixer, soundFilepath.data(), decode)};
            if (!soundAudio) {
                throw std::runtime_error(SDL_GetError());
            }

            m_soundsMap[soundName] = soundAudio;
        }
    };


    void unsetGameMusicLoopCb() {
        if (m_usingGameMusicLoopCb) {
            MIX_SetTrackStoppedCallback(m_musicChannel, nullptr, nullptr);
            m_usingGameMusicLoopCb = false;
        }
    };


    void _loadMusic(const std::string& musicFilePath) {
        _unloadMusic();
        
        constexpr bool decode {true};
        m_musicAudio = MIX_LoadAudio(m_mixer, musicFilePath.data(), !decode);
        if (!m_musicAudio) {
            throw std::runtime_error(SDL_GetError());
        }
        
        MIX_SetTrackAudio(m_musicChannel, m_musicAudio);
    };
    

    void _unloadMusic() {
        MIX_SetTrackAudio(m_musicChannel, nullptr);
        MIX_DestroyAudio(m_musicAudio);
        m_musicAudio = nullptr;
    };


    void _playMusic(int loops=0, int startMs=0, int fadeInMs=0) {
        SDL_PropertiesID properties {SDL_CreateProperties()};
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, startMs);
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fadeInMs);
        
        MIX_PlayTrack(m_musicChannel, properties);
        
        SDL_DestroyProperties(properties);
    };
    

    void _stopMusic() {
        constexpr int fadeOutFrames {0};
        MIX_StopTrack(m_musicChannel, fadeOutFrames);
    };


    void _playGameMusicTrack(int trackIdx, int loops=0, int startMs=0, int fadeInMs=0) {
        if (m_gameMusicFilePaths.size() == 0) {
            return;
        }

        if (trackIdx < 0) {
            trackIdx = 0;
        } else if (trackIdx >= static_cast<int>(m_gameMusicFilePaths.size())) {
            trackIdx %= static_cast<int>(m_gameMusicFilePaths.size());
        }

        _loadMusic(m_gameMusicFilePaths[trackIdx]);
        _playMusic(loops, startMs, fadeInMs);
    };



    static void musicEndEventCallback(void* instancePtr, MIX_Track*) {
        AudioManager* audioManager {static_cast<AudioManager*>(instancePtr)};
        SDL_PushEvent(&audioManager->m_musicEndEvent);
    };


    static void gameMusicTrackEndCallback(void* instancePtr, MIX_Track*) {
        AudioManager* audioManager {static_cast<AudioManager*>(instancePtr)};

        audioManager->m_gameMusicTrackIdx += 1;
        audioManager->m_gameMusicTrackIdx %= static_cast<int>(audioManager->m_gameMusicFilePaths.size());
        
        constexpr int loops {0};
        constexpr int startMs {0};
        audioManager->_playGameMusicTrack(
            audioManager->m_gameMusicTrackIdx,
            loops, 
            startMs, 
            audioManager->m_gameMusicLoopFadeInMs
        );
    };
};



#endif