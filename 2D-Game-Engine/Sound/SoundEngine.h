#pragma once

#include <string>
#include "SFML\Audio.hpp"

enum class musicType { FOREGROUND, BACKGROUND };

class SoundEngine
{
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

private:
	sf::SoundBuffer m_soundBuffer;
	sf::Music* m_fgMusic;	//Foreground Music
	sf::Music* m_bgMusic;	//Ambient Music
};

