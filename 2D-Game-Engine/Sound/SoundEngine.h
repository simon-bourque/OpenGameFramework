#pragma once

#include <string>
#include "SFML/Audio.hpp"

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

	sf::SoundBuffer m_soundBuffer;
	sf::Sound* sound;
	sf::Music* m_fgMusic;	//Foreground Music
	sf::Music* m_bgMusic;	//Ambient Music

public:
	SoundEngine();
	virtual ~SoundEngine();

	//Sound Manip
	void playSound(std::string);
	void stopSound();
	void pauseSound();

	//Music Manip
	void playMusic(std::string, bool isLooped, musicType);
	void stopMusic(musicType);
	void pauseMusic(musicType);
	void setMusicVolume(int, musicType);

	static SoundEngine* get() {
		ASSERT(s_instance, "Window must be initialized before use.");
		return s_instance;
	};

	static void init() {
		ASSERT(!s_instance, "Window is already initialized.");
		s_instance = new SoundEngine();
	}

	static void destroy() {
		delete s_instance;
	}
};

