#include <string>
#include <stdexcept>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_error.h>
#include "./sdlUtils/soundEffect.h"
#include "./audioManager.h"

// #include <filesystem>
// #include <SDL3/SDL.h>
// #include "./sdlUtils/fCircle.h"
// #include "./framework/framework.h"



AudioManager::AudioManager(
    const std::vector<std::string>& channels, 
    const std::unordered_map<std::string, std::string>& soundFilepaths,
    const std::vector<std::string>& gameMusicFilePaths,
    const std::string& menuMusicFilePath,
    const std::string& saveFilePath
) : m_gameMusicFilePaths{gameMusicFilePaths}, m_menuMusicFilePath{menuMusicFilePath}, m_saveFilePath{saveFilePath} {
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
    if (!m_musicChannel) {
        throw std::runtime_error(SDL_GetError());
    }

    loadSavedVolumeSettings();

    AudioManager::created = true;
};

AudioManager::AudioManager() : AudioManager({}, {}) {
};

AudioManager::~AudioManager() {
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
void AudioManager::loadMusic(const std::string& musicFilePath) {
    unsetGameMusicLoopCb();
    _loadMusic(musicFilePath);
};


// Unloads any audio loaded via the loadMusic method.
// This will trigger the given music end event if one was set and mucis was currently playing.
void AudioManager::unloadMusic() {
    unsetGameMusicLoopCb();
    _unloadMusic();
};


// Play the current loaded audio in the music channel. This will restart any currently playing audio.
void AudioManager::playMusic(int loops, int startMs, int fadeInMs) {
    unsetGameMusicLoopCb();
    _playMusic(loops, startMs, fadeInMs);
};


// Stops any currently playing audio in the music channel. This will trigger the music end event if one was set.
void AudioManager::stopMusic() {
    unsetGameMusicLoopCb();
    _stopMusic();
};


// Pause the currently playing music. This will nto trigger any music end event.
void AudioManager::pauseMusic() {
    MIX_PauseTrack(m_musicChannel);
};


// Resume paused music.
void AudioManager::resumeMusic() {
    MIX_ResumeTrack(m_musicChannel);
};


// Query if the music channel is currently playing music.
bool AudioManager::musicPlaying() const {
    return MIX_TrackPlaying(m_musicChannel);
};


// Get the current volume of the music channel.
float AudioManager::getMusicVolume() const {
    return MIX_GetTrackGain(m_musicChannel);
};


// Set the volume of the music channel. Clamps the passed value to 0-1.
void AudioManager::setMusicVolume(float volume) {
    MIX_SetTrackGain(m_musicChannel, clampVolume(volume));
};


// Set the event to be placed on the event queue when music finishes playing or is stopped.
// Calling this method will after calling playGameMusicLoop will disrupt the loop.
void AudioManager::setMusicEndEvent(const SDL_Event& event) {
    m_musicEndEvent = event;
    MIX_SetTrackStoppedCallback(m_musicChannel, AudioManager::musicEndEventCallback, this);
};


// Unset any event set with setMusicEndEvent.
// Calling this method will after calling playGameMusicLoop will disrupt the loop.
void AudioManager::unsetMusicEndEvent() {
    m_musicEndEvent = SDL_Event{};
    MIX_SetTrackStoppedCallback(m_musicChannel, nullptr, nullptr);
};


// Load and play the audio provided by the menuMusicFilePath param in the constructor.
// Does nothing if no menuMusicFilePath was provided.
void AudioManager::playMenuMusic(int loops, int startMs, int fadeInMs) {
    unsetGameMusicLoopCb();
    if (m_menuMusicFilePath.size() == 0) {
        return;
    }

    _loadMusic(m_menuMusicFilePath);
    _playMusic(loops, startMs, fadeInMs);
};


// Play a music track from the gameMusicFilePaths passed on construction. 
// Tracks will be in the same order as given. Does nothing if are no gameMusicFilePaths.
void AudioManager::playGameMusicTrack(int trackIdx, int loops, int startMs, int fadeInMs) {
    unsetGameMusicLoopCb();
    _playGameMusicTrack(trackIdx, loops, startMs, fadeInMs);
};


// Play the game music (given via gameMusicFilePaths) on a loop starting at the given index.
// Does nothing if there are no gameMusicFilePaths. 
void AudioManager::playGameMusicLoop(int startingTrackIdx, int fadeInMs) {
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
SoundEffect AudioManager::getSoundEffect(const std::string& channelName, const std::string& soundName) {
    if (!m_channelsMap.contains(channelName)) {
        throw std::runtime_error("Channel: '" + channelName + "' doesn't exist.\n");
    }
    if (!m_soundsMap.contains(soundName)) {
        throw std::runtime_error("Sound: '" + soundName + "' doesn't exist\n");
    }

    return SoundEffect{m_channelsMap[channelName], m_soundsMap[soundName]};
};


// Get the current sound volume.
float AudioManager::getSoundVolume() const {
    return m_soundVolume;
};


// Set the current sound volume. The volume is clamped to 0-1.
void AudioManager::setSoundVolume(float volume) {
    m_soundVolume = clampVolume(volume);

    for (auto& entry : m_channelsMap) {
        MIX_Track* channel {entry.second};
        MIX_SetTrackGain(channel, m_soundVolume);
    }
};



void AudioManager::createChannelsAndSounds(
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


float AudioManager::clampVolume(float volume) {
    if (volume < 0.0f) {
        volume = 0.0f;
    } else if (volume > 1.0f) {
        volume = 1.0f;
    }
    return volume;
};


void AudioManager::unsetGameMusicLoopCb() {
    if (m_usingGameMusicLoopCb) {
        MIX_SetTrackStoppedCallback(m_musicChannel, nullptr, nullptr);
        m_usingGameMusicLoopCb = false;
    }
};


void AudioManager::_loadMusic(const std::string& musicFilePath) {
    _unloadMusic();
    
    constexpr bool decode {true};
    m_musicAudio = MIX_LoadAudio(m_mixer, musicFilePath.data(), !decode);
    if (!m_musicAudio) {
        throw std::runtime_error(SDL_GetError());
    }
    
    MIX_SetTrackAudio(m_musicChannel, m_musicAudio);
};


void AudioManager::_unloadMusic() {
    MIX_SetTrackAudio(m_musicChannel, nullptr);
    MIX_DestroyAudio(m_musicAudio);
    m_musicAudio = nullptr;
};


void AudioManager::_playMusic(int loops, int startMs, int fadeInMs) {
    SDL_PropertiesID properties {SDL_CreateProperties()};
    SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    SDL_SetNumberProperty(properties, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, startMs);
    SDL_SetNumberProperty(properties, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fadeInMs);
    
    MIX_PlayTrack(m_musicChannel, properties);
    
    SDL_DestroyProperties(properties);
};


void AudioManager::_stopMusic() {
    constexpr int fadeOutFrames {0};
    MIX_StopTrack(m_musicChannel, fadeOutFrames);
};


void AudioManager::_playGameMusicTrack(int trackIdx, int loops, int startMs, int fadeInMs) {
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



void AudioManager::musicEndEventCallback(void* instancePtr, MIX_Track*) {
    AudioManager* audioManager {static_cast<AudioManager*>(instancePtr)};
    SDL_PushEvent(&audioManager->m_musicEndEvent);
};


void AudioManager::gameMusicTrackEndCallback(void* instancePtr, MIX_Track*) {
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


void AudioManager::loadSavedVolumeSettings() {
    std::ifstream file{m_saveFilePath};
    if (file.fail()) {
        file.close();
        return;
    }

    try {
        std::string savedData{};
        std::getline(file, savedData);
        std::size_t delimPos {savedData.find(',')};
    
        std::string musicData{savedData, 0, delimPos};
        std::string soundData{savedData, delimPos + 1};
    
        setMusicVolume(std::stof(musicData));
        setSoundVolume(std::stof(soundData));

    } catch (...) {
        file.close();
        return;
    }
};


void AudioManager::saveVolumeSettings() {
    if (m_saveFilePath == "") {
        return;
    }

    try {
        std::ofstream file{m_saveFilePath};
    
        std::string volumeData{std::to_string(getMusicVolume())};
        volumeData += ',';
        volumeData += std::to_string(getSoundVolume());
    
        file << volumeData;

    } catch (...) {
        return;
    }
};



// int main() {
//     namespace fs = std::filesystem;
//     // Display display {"Test", 0, 0, 1536, 864, SDL_WINDOW_FULLSCREEN};
//     std::string path{SDL_GetBasePath()};
//     path += "lose.wav";
//     std::cout << path;

//     AudioManager audioManager{
//         {"TEST"}, {{"SOUND", path}}, 
//         {},{},fs::absolute(fs::path{"./savedVolume.txt"}).string()
//     };
//     audioManager.saveVolumeSettings();
//     // Uint32 musicEnd {SDL_RegisterEvents(1)};
//     // SDL_Event userEvent{musicEnd};
//     // audioManager.setMusicEndEvent(userEvent);
    
//     // int trackNum {0};
//     // // audioManager.playGameMusicLoop();
//     // audioManager.playMenuMusic();
//     // audioManager.unloadMusic();

//     SoundEffect sound{audioManager.getSoundEffect("TEST", "SOUND")};

//     FRect rect{500, 500, 200, 500};
//     FCircle circle{0, 0, 14};
//     FRect circleRect{0, 0, 30, 30};

//     bool running {true};
//     while (running) {
//         SDL_Event event{};
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_EVENT_QUIT) {
//                 running = false;
//             } else if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
//                 if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
//                     running = false;
//                 } else if (event.key.scancode == SDL_SCANCODE_E) {
//                     if (audioManager.musicPlaying()) {
//                         audioManager.pauseMusic();
//                     } else {
//                         audioManager.resumeMusic();
//                     }
//                 }  else if (event.key.scancode == SDL_SCANCODE_A) {
//                     audioManager.setSoundVolume(audioManager.getSoundVolume() - .1f);
//                 } else if (event.key.scancode == SDL_SCANCODE_D) {
//                     audioManager.setSoundVolume(audioManager.getSoundVolume() + .1f);
//                 } else if (event.key.scancode == SDL_SCANCODE_R) {
//                     sound.forcePlay();
//                 } else if (event.key.scancode == SDL_SCANCODE_Q) {
//                     sound.stop();
//                 }
//             }
//         }


//         float mX;
//         float mY;
//         SDL_GetGlobalMouseState(&mX, &mY);
//         circle.setCenter(mX, mY);
//         circleRect.setCenter(circle.center());


//         int red {255};
//         int green {0};
//         if (circle.hasRectIntersection(&rect)) {
//             red = 0;
//             green = 255;
//         }


//         SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
//         SDL_RenderClear(Framework::display.renderer());
        
//         SDL_SetRenderDrawColor(Framework::display.renderer(), red, green, 0, 255);
//         SDL_RenderFillRect(Framework::display.renderer(), &rect.getSDLFRect());
        
//         SDL_RenderTexture(Framework::display.renderer(), Framework::images.ballImg, nullptr, &circleRect.getSDLFRect());

//         SDL_RenderPresent(Framework::display.renderer());
//     }
//     return 0;
// };