
#include "Core.h"

#include "RawImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

RawImage loadImage(string fileName) {
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;
	uint8* data = stbi_load(fileName.c_str(), &width, &height, &channels, 4);
	
	return RawImage(data, width, height, channels);
}