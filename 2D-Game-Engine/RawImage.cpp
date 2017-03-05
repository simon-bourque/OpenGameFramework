#include "RawImage.h"


RawImage::RawImage(uint8* data, uint32 width, uint32 height, uint8 channels) : m_data(data), m_width(width), m_height(height), m_channels(channels) {}

RawImage::RawImage(const RawImage& img) : m_width(img.m_width), m_height(img.m_height), m_channels(img.m_channels) {
	
	const uint32 dataSize = m_width * m_height * m_channels;
	m_data = new uint8[dataSize];

	for (uint32 i = 0; i < dataSize; i++) {
		m_data[i] = img.m_data[i];
	}
}

RawImage::RawImage(RawImage&& img) : m_width(img.m_width), m_height(img.m_height), m_channels(img.m_channels) {
	m_data = img.m_data;
	img.m_data = nullptr;
	img.m_width = 0;
	img.m_height = 0;
	img.m_channels = 0;
}

RawImage::~RawImage()
{
	delete[] m_data;
}

void RawImage::getPixel(uint32 x, uint32 y, uint8* buff, uint32 buffSize) const {
	
	uint32 offset = (x + (y * m_width)) * m_channels;

	for (uint32 i = 0; i < buffSize && i < m_channels; i++) {
		buff[i] = m_data[offset + i];
	}
}

RawImage RawImage::getSubImage(uint32 x, uint32 y, uint32 width, uint32 height) const {

	uint8* data = new uint8[width * height * m_channels];
	uint8* dataPtr = data;

	for (uint32 i = y; i < y + height; i++) {
		for (uint32 j = x; i < x + width; j++) {
			
			uint8* pixel = new uint8[m_channels];

			getPixel(j, i, pixel, m_channels);

			for (uint32 k = 0; k < m_channels; k++) {
				(*dataPtr) = pixel[k];
				dataPtr++;
			}

			delete pixel;
		}
	}

	return RawImage(data, width, height, m_channels);
}

RawImage& RawImage::operator=(const RawImage& img) {

	if (this != &img) {
		m_width = img.m_width;
		m_height = img.m_height;
		m_channels = img.m_channels;

		delete[] m_data;

		const uint32 dataSize = m_width * m_height * m_channels;
		m_data = new uint8[dataSize];

		for (uint32 i = 0; i < dataSize; i++) {
			m_data[i] = img.m_data[i];
		}
	}

	return *this;
}

RawImage& RawImage::operator=(RawImage&& img) {

	if (this != &img) {
		m_width = img.m_width;
		m_height = img.m_height;
		m_channels = img.m_channels;

		delete[] m_data;

		const uint32 dataSize = m_width * m_height * m_channels;
		m_data = new uint8[dataSize];

		for (uint32 i = 0; i < dataSize; i++) {
			m_data[i] = img.m_data[i];
		}

		delete[] img.m_data;
		img.m_width = 0;
		img.m_height = 0;
		img.m_channels = 0;
	}

	return *this;
}
