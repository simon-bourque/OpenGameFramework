#pragma once

#include <Core/Core.h>

#include <fstream>

void writeFloat(std::ofstream& output, float32 f);
void writeUnsignedInt(std::ofstream& output, uint32 ui);
void writeInt(std::ofstream& output, int32 i);