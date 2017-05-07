#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include "Core/Core.h"

class Animation {
private:
	uint32* m_frames;
	float32* m_delays;
	uint32 m_numberOfFrames;

	uint32 m_currentFrameIndex;
	float32 m_timeElapsed;
public:
	Animation(uint32* frames, float32* delays, uint32 numberOfFrames);
	Animation(const Animation& anim);
	Animation(Animation&& anim);
	virtual ~Animation();

	void tick(float32 delta);
	void reset();

	uint32 getCurrentFrame() { return m_frames[m_currentFrameIndex]; };

	Animation& operator=(const Animation& anim);
	Animation& operator=(Animation&& anim);
};

#endif