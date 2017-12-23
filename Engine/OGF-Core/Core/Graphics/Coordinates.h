#pragma once

#include "Core/Core.h"

void convertPixelToOpenGLCoordinate(uint32 xIn, uint32 yIn, uint32 windowWidth, uint32 windowHeight, float32& xOut, float32& yOut);
void convertPixelToOpenGLLength(int32 lengthIn, uint32 windowLength, float32& lengthOut);