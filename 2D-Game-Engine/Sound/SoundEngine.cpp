#include "SoundEngine.h"
SoundEngine* SoundEngine::s_instance = nullptr;

SoundEngine::SoundEngine() {
	m_fgMusic = new sf::Music();
	m_bgMusic = new sf::Music();
	m_sound = new sf::Sound();

	m_sound->setVolume(50);

	//Initialize all sounds here
	soundMap["res/sound/walk.wav"] = new sf::SoundBuffer();
	soundMap["res/sound/swish.wav"] = new sf::SoundBuffer();
	soundMap["res/sound/inventory.wav"] = new sf::SoundBuffer();

	//Loading sounds in memory
	for (auto &sound : soundMap) {
		sound.second->loadFromFile(sound.first);
	}
}

void SoundEngine::playSound(string soundName) {
	auto iter = soundMap.find(soundName);

	if (iter == soundMap.end()) {
		throw std::runtime_error("Could not find sound sample in map" + soundName);
	}

	m_sound->setBuffer(*iter->second);
	m_sound->play();
}

void SoundEngine::stopSound() {
	m_sound->stop();
}

void SoundEngine::pauseSound() {
	m_sound->pause();
}

void SoundEngine::setSoundVolume(uint32 volume) {
	m_sound->setVolume(volume);
}

void SoundEngine::playMusic(string musicName, bool isLooped, musicType type) {
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

void SoundEngine::setMusicVolume(uint32 volume, musicType type) {
	if (type == musicType::FOREGROUND) {
		m_fgMusic->setVolume(volume);
	}
	else if (type == musicType::BACKGROUND) {
		m_bgMusic->setVolume(volume);
	}
}

SoundEngine::~SoundEngine() {
	delete m_sound;
	delete m_bgMusic;
	delete m_fgMusic;

	for (const std::pair<string, sf::SoundBuffer*>& pair : soundMap) {
		delete pair.second;
	}
}
