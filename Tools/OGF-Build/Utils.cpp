
#include <Core/Core.h>

#include <fstream>
#include <cstring>

using namespace std;

void writeFloat(ofstream& output, float32 f) {
	char bytes[4] = { 0,0,0,0 };

	memcpy(bytes, &f, sizeof(float32));

	output.write(bytes, sizeof(float32));
}

void writeUnsignedInt(ofstream& output, uint32 ui) {
	char bytes[4] = { 0,0,0,0 };

	memcpy(bytes, &ui, sizeof(uint32));

	output.write(bytes, sizeof(uint32));
}

void writeInt(ofstream& output, int32 i) {
	char bytes[4] = { 0,0,0,0 };

	memcpy(bytes, &i, sizeof(int32));

	output.write(bytes, sizeof(int32));
}