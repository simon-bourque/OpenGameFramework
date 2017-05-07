#pragma once
#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include "Core/Core.h"

class RawImage
{
private:
	uint8* m_data;

	uint32 m_width;
	uint32 m_height;
	uint8 m_channels;
public:
	RawImage();
	RawImage(uint8* data, uint32 width, uint32 height, uint8 m_channels);
	RawImage(const RawImage& img);
	RawImage(RawImage&& img);
	~RawImage();

	uint8* getData() const { return m_data; }
	uint32 getWidth() const { return m_width; }
	uint32 getHeight() const { return m_height; }
	uint8 getChannels() const { return m_channels; }
	uint32 getSizeInBytes() const { return m_width * m_height * m_channels; };

	void getPixel(uint32 x, uint32 y, uint8* buff, uint32 buffSize) const;
	RawImage getSubImage(uint32 x, uint32 y, uint32 width, uint32 height) const;

	RawImage& operator=(const RawImage& img);
	RawImage& operator=(RawImage&& img);
};

#endif

