#include "Coordinates.h"

void convertPixelToOpenGLCoordinate(uint32 xIn, uint32 yIn, uint32 windowWidth, uint32 windowHeight, float32& xOut, float32& yOut) {
	xOut = ((xIn * 2) / (float32)windowWidth) - 1;
	yOut = -(((yIn * 2) / (float32)windowHeight) - 1);
}

void convertPixelToOpenGLLength(int32 lengthIn, uint32 windowLength, float32& lengthOut) {
	lengthOut = (lengthIn * 2) / (float32)windowLength;
}