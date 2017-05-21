#pragma once

#include "Core\Core.h"
#include "SFML/Audio.hpp"

#include <unordered_map>

#include "Core/EngineAssert.h"

/*
Wrapper class for audio SFML functions that we actually use
*/

enum class musicType {
	FOREGROUND,
	BACKGROUND
};

class SoundEngine
{
private:
	static SoundEngine* s_instance;

	std::unordered_map<string, sf::SoundBuffer*> soundMap;

	sf::Sound* m_sound;
	sf::Music* m_fgMusic;	//Foreground Music
	sf::Music* m_bgMusic;	//Ambient Music

public:
	SoundEngine();
	virtual ~SoundEngine();

	//Sound Manip
	void playSound(string);
	void stopSound();
	void pauseSound();

	//Music Manip
	void playMusic(string, bool isLooped, musicType);
	void stopMusic(musicType);
	void pauseMusic(musicType);
	void setMusicVolume(uint32, musicType);

	static SoundEngine* get() {
		ASSERT(s_instance, "SoundEngine must be initialized before use.");
		return s_instance;
	};

	static void init() {
		ASSERT(!s_instance, "SoundEngine is already initialized.");
		s_instance = new SoundEngine();
	}

	static void destroy() {
		delete s_instance;
	}
};

