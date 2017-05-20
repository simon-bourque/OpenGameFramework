#include "SoundEngine.h"

SoundEngine::SoundEngine() {
	m_fgMusic = new sf::Music();
	m_bgMusic = new sf::Music();
}

void SoundEngine::playSound(std::string soundName) {
	if (!m_soundBuffer.loadFromFile(soundName)) {
		throw std::runtime_error("Failed to load sound from file " + soundName);
	}

	sf::Sound sound;
	sound.setBuffer(m_soundBuffer);
	sound.play();

}

void SoundEngine::stopSound() {
	sf::Sound sound;
	sound.setBuffer(m_soundBuffer);
	sound.stop();
}

void SoundEngine::pauseSound() {
	sf::Sound sound;
	sound.setBuffer(m_soundBuffer);
	sound.pause();
}

void SoundEngine::playMusic(std::string musicName, bool isLooped, musicType type) {
	if (type == musicType::FOREGROUND) {
		if (!m_fgMusic->openFromFile(musicName)) {
			throw std::runtime_error("Failed to load foreground music from file " + musicName);
		}

		m_fgMusic->play();
		m_fgMusic->setLoop(isLooped);
	}

	else if (type == musicType::BACKGROUND) {
		if (!m_bgMusic->openFromFile(musicName)) {
			throw std::runtime_error("Failed to load background music from file " + musicName);
		}

		m_bgMusic->play();
		m_bgMusic->setLoop(isLooped);
	}
}

void SoundEngine::pauseMusic(musicType type) {
	if (type == musicType::FOREGROUND) {
		m_fgMusic->pause();
	}
	else if (type == musicType::BACKGROUND) {
		m_bgMusic->pause();
	}
}

void SoundEngine::stopMusic(musicType type) {
	if (type == musicType::FOREGROUND) {
		m_fgMusic->stop();
	}
	else if (type == musicType::BACKGROUND) {
		m_bgMusic->stop();
	}
}

SoundEngine::~SoundEngine() {
	delete m_fgMusic;
	delete m_bgMusic;
}
