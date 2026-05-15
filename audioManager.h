#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H


#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_error.h>


class AudioManager {
    inline static bool created {false};

    MIX_Mixer* m_mixer {nullptr};
    MIX_Track* m_musicChannel {nullptr};
    MIX_Audio* m_musicAudio {nullptr};

    std::unordered_map<std::string, MIX_Track*> m_channelsMap{};
    std::unordered_map<std::string, MIX_Audio*> m_soundsMap{};
    std::vector<std::string> m_gameMusicFilePaths{};
    std::string m_menuMusicFilePath{};
    

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

        m_musicAudio = MIX_LoadAudio(m_mixer, m_menuMusicFilePath.data(), false);
        MIX_SetTrackAudio(m_musicChannel, m_musicAudio);
        MIX_PlayTrack(m_musicChannel, 0);
    };

    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

    ~AudioManager() {
        for (auto& entry : m_channelsMap) {
            auto [channelName, channelTrack] {entry};
            MIX_SetTrackAudio(channelTrack, nullptr);
            MIX_DestroyTrack(channelTrack);
        }

        for (auto& entry : m_soundsMap) {
            auto [soundName, soundAudio] {entry};
            MIX_DestroyAudio(soundAudio);
        }

        MIX_SetTrackAudio(m_musicChannel, nullptr);
        MIX_DestroyTrack(m_musicChannel);
        MIX_DestroyAudio(m_musicAudio);

        MIX_DestroyMixer(m_mixer);
        MIX_Quit();
        AudioManager::created = false;
    };


    void playMenuMusic(bool loop=true, int fadeInMs=0) {
    };


    void loadMusic(const std::string& musicFilePath) {
        unloadMusic();
    
        constexpr bool decode {true};
        m_musicAudio = MIX_LoadAudio(m_mixer, musicFilePath.data(), !decode);
        if (!m_musicAudio) {
            throw std::runtime_error(SDL_GetError());
        }
    
        MIX_SetTrackAudio(m_musicChannel, m_musicAudio);
    };


    void unloadMusic() {
        MIX_SetTrackAudio(m_musicChannel, nullptr);
        MIX_DestroyAudio(m_musicAudio);
        m_musicAudio = nullptr;
    };


    void playMusic(int loops=0, int startMs=0, int fadeInMs=0) {
        SDL_PropertiesID properties {SDL_CreateProperties()};
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, startMs);
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fadeInMs);

        MIX_PlayTrack(m_musicChannel, properties);

        SDL_DestroyProperties(properties);
    };


    void stopMusic() {
        constexpr int fadeOutFrames {0};
        MIX_StopTrack(m_musicChannel, fadeOutFrames);
    };


    void pauseMusic() {
        MIX_PauseTrack(m_musicChannel);
    };


    void resumeMusic() {
        MIX_ResumeTrack(m_musicChannel);
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
            auto [soundName, soundFilepath] {entry};
            MIX_Audio* soundAudio {MIX_LoadAudio(m_mixer, soundFilepath.data(), decode)};
            if (!soundAudio) {
                throw std::runtime_error(SDL_GetError());
            }

            m_soundsMap[soundName] = soundAudio;
        }
    };
};




#endif