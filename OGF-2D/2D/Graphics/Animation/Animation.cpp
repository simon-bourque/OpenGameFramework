#include "Animation.h"


Animation::Animation(uint32* frames, float32* delays, uint32 numberOfFrames) : m_numberOfFrames(numberOfFrames), m_currentFrameIndex(0), m_timeElapsed(0.0f) {
	m_frames = new uint32[numberOfFrames];
	m_delays = new float32[numberOfFrames];

	for (uint32 i = 0; i < numberOfFrames; i++) {
		m_frames[i] = frames[i];
		m_delays[i] = delays[i];
	}
}

Animation::Animation(const Animation& anim) : m_numberOfFrames(anim.m_numberOfFrames), m_currentFrameIndex(anim.m_currentFrameIndex), m_timeElapsed(anim.m_timeElapsed) {
	m_frames = new uint32[m_numberOfFrames];
	m_delays = new float32[m_numberOfFrames];

	for (uint32 i = 0; i < m_numberOfFrames; i++) {
		m_frames[i] = anim.m_frames[i];
		m_delays[i] = anim.m_delays[i];
	}
}

Animation::Animation(Animation&& anim) : m_numberOfFrames(anim.m_numberOfFrames), m_currentFrameIndex(anim.m_currentFrameIndex), m_timeElapsed(anim.m_timeElapsed) {
	m_frames = anim.m_frames;
	m_delays = anim.m_delays;
	anim.m_frames = nullptr;
	anim.m_delays = nullptr;
}

Animation::~Animation() {
	delete[] m_frames;
	delete[] m_delays;
}

void Animation::tick(float32 delta) {
	m_timeElapsed += delta;
	if (m_timeElapsed >= m_delays[m_currentFrameIndex]) {
		m_timeElapsed -= m_delays[m_currentFrameIndex++];

		if (m_currentFrameIndex >= m_numberOfFrames) {
			m_currentFrameIndex = 0;
		}
	}
}

void Animation::reset() {
	m_currentFrameIndex = 0;
	m_timeElapsed = 0.0f;
}

Animation& Animation::operator=(const Animation& anim) {
	m_numberOfFrames = anim.m_numberOfFrames;
	m_currentFrameIndex = anim.m_currentFrameIndex;
	m_timeElapsed = anim.m_timeElapsed;

	delete[] m_frames;
	delete[] m_delays;

	m_frames = new uint32[m_numberOfFrames];
	m_delays = new float32[m_numberOfFrames];

	for (uint32 i = 0; i < m_numberOfFrames; i++) {
		m_frames[i] = anim.m_frames[i];
		m_delays[i] = anim.m_delays[i];
	}

	return *this;
}

Animation& Animation::operator=(Animation&& anim) {
	m_numberOfFrames = anim.m_numberOfFrames;
	m_currentFrameIndex = anim.m_currentFrameIndex;
	m_timeElapsed = anim.m_timeElapsed;

	delete[] m_frames;
	delete[] m_delays;

	m_frames = anim.m_frames;
	m_delays = anim.m_delays;

	anim.m_frames = nullptr;
	anim.m_delays = nullptr;

	return *this;
}