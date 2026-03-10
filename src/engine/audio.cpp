#include <engine/audio.h>

namespace GEC {

AudioEngine::AudioEngine()
{
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to setup the miniaudio engine\n";
        fflush(stdout);
    }
}

bool AudioEngine::LoadSound(const char* id, const char* file_location)
{

    if (SoundExists(id)) {
        std::cerr << "A sound with a ID of " << id << " already exists" << std::endl;
        fflush(stdout);
        return false;
    }

    ma_sound* soundObject = new ma_sound;

    ma_result result = ma_sound_init_from_file(
        &engine,
        file_location,
        0,
        NULL,
        NULL,
        soundObject);

    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load sound: " << file_location << std::endl;
        delete soundObject;
        return false;
    }
    sounds.insert({ id, soundObject });

    return true;
}

bool AudioEngine::PlaySound(const char* id)
{
    if (SoundExists(id)) {
        ma_sound* soundObject = GetSoundObj(id);
        ma_sound_start(soundObject);
        return true;
    } else {
        std::cerr << "Failed to play " << id << " no assignment" << std::endl;
        fflush(stdout);
        return false;
    }
}
bool AudioEngine::PauseSound(const char* id)
{
    if (SoundExists(id)) {
        ma_sound* soundObject = GetSoundObj(id);
        ma_sound_stop(soundObject);
        return true;
    } else {
        std::cerr << "Failed to pause " << id << " no assignment" << std::endl;
        fflush(stdout);
        return false;
    }
}
bool AudioEngine::StopSound(const char* id)
{
    if (SoundExists(id)) {
        ma_sound* soundObject = GetSoundObj(id);
        ma_sound_stop(soundObject);
        ma_sound_seek_to_pcm_frame(soundObject, 0);
        return true;
    } else {
        std::cerr << "Failed to pause " << id << " no assignment" << std::endl;
        fflush(stdout);
        return false;
    }
}
bool AudioEngine::SoundFinished(const char* id)
{
    if (SoundExists(id)) {
        ma_sound* soundObject = GetSoundObj(id);
        return ma_sound_at_end(soundObject);
    } else {
        std::cerr << "Failed to pause " << id << " no assignment" << std::endl;
        fflush(stdout);
        return false;
    }
}

ma_sound* AudioEngine::GetSoundObj(const char* id)
{
    return sounds[id];
}

bool AudioEngine::SoundExists(const char* id)
{
    return sounds.count(id) != 0;
}

AudioEngine::~AudioEngine()
{
    for (auto& pair : sounds) {
        ma_sound_uninit(pair.second);
        delete pair.second;
    }

    ma_engine_uninit(&engine);
}

}