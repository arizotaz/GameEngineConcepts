// #############################################################################
// # audio.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/15/2026
// # Last Modification:  02/17/2026
// #############################################################################
// # Audio Controller Single Structure
// #############################################################################
// # This code provides a simple and easy mechanism for loading and playing
// # audio with the miniaudio.h library.
// #
// # Calling GEC::AudioEngine::GetInstance() returns the single object of the
// # structure.
// #
// # The Calls are as follow:
// #     LoadSound(id, location)
// #         Registers a audio fine to be used
// #         id       - soundID
// #         location - absolute file location
// #         returns - success flag
// #
// #     PlaySound(id)
// #         Plays the sound loaded with a matching id
// #         id       - soundID
// #         returns - success flag
// #
// #     PausesSound(id)
// #         Stops the sound loaded with a matching id
// #         id       - soundID
// #         returns - success flag
// #
// #     StopSound(id)
// #         Stops the and returns the read index to 0
// #         of the loaded sound with a matching id
// #         id       - soundID
// #         returns - success flag
// #
// #     SoundFinished(id)
// #         Returns true if the sound read head has reached
// #         the end of the file
// #         id       - soundID
// #         returns - success flag
// #
// #     SoundExists(id)
// #         Returns true if the sound has with a matchind id
// #         has been registered
// #         id       - soundID
// #         returns - success flag
// #
// #     GetSoundObj(id)
// #         Returns the miniaudio sound object pointer.
// #         nullptr if no sound exists
// #         id       - soundID
// #         returns - ma_sound*
// #
// #
// #############################################################################

#ifndef AUDIO_H
#define AUDIO_H 1

#include <iostream>
#include <map>
#include <miniaudio.h>

namespace GEC {


/**
 * Audio Controller Single Structure
 */
class AudioEngine {
public:

    /**
     * Returns the Singleton Instance
     */
    static AudioEngine& GetInstance()
    {
        static AudioEngine engine;
        return engine;
    }

    /**
     * Loads a sound file at {file_location} and maps it to
     * an ID of {id}
     * @param {id} ID of the sound
     * @param {location} File location of the sound
     * @returns {success} Completed Successfully
     */
    bool LoadSound(const char* id, const char* file_location);

    /**
     * Plays the sound with an ID of {ID}
     * @param {id} ID of the sound
     * @returns {success} Completed Successfully
     */
    bool PlaySound(const char* id);

    /**
     * Stops the sound with an ID of {ID} but doesn't change
     * read head location
     * @param {id} ID of the sound
     * @returns {success} Completed Successfully
     */
    bool PauseSound(const char* id);

    /**
     * Stops the sound with an ID of {ID} and returns
     * the read index to 0, restarting the sound if
     * played again
     * @param {id} ID of the sound
     * @returns {success} Completed Successfully
     */
    bool StopSound(const char* id);

    /**
     * Returns true when the sound read head has
     * reached the end of the file
     * @param {id} ID of the sound
     * @returns {completed} Sound has played entirely
     */
    bool SoundFinished(const char* id);

    /**
     * Returns true if a sound with the given ID
     * has been registered to the Audio Engine
     * @param {id} ID of the sound
     * @returns {registered} Sound registered
     */
    bool SoundExists(const char* id);

    /**
     * Returns the miniaudio sound object
     * @param {id} ID of the sound
     * @returns {ma_sound*} miniaudio sound object or nullptr
     */
    ma_sound* GetSoundObj(const char* id);

    // Remove Copy Constructor
    AudioEngine(const AudioEngine&) = delete;

    // Remove Assignment Operator
    AudioEngine& operator=(const AudioEngine&) = delete;

private:
    AudioEngine();

    ~AudioEngine();

    ma_engine engine;

    std::map<std::string, ma_sound*> sounds;
};
}

#endif