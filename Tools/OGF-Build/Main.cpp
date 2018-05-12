#define STB_IMAGE_STATIC
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Core/Resource/RawImage.h>
#include <Core/Core.h>
#include <Core/Math/Geometry/Rectangle.h>

#include "Core/Graphics/Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "ConvertLevel.h"
#include "Utils.h"

using namespace std;

void convertTexture(std::queue<string>& args);
void convertTexture2D(const RawImage& img, ofstream& output, std::queue<string>& args);
void convertTexture2DArray(const RawImage& img, ofstream& output, std::queue<string>& args);


string readArgument(std::queue<string>& args) {
	string arg(args.front());
	args.pop();
	return arg;
}

int main(int argc, char *argv[]) {
	std::queue<string> args;
	for (int32 i = 1; i < argc; i++) {
		args.push(string(argv[i]));
	}
	
	if (args.empty()) {
		cout << "Error: invalid number of arguments." << endl;
		return 1;
	}

	// Check first argument for type of file
	string fileType(readArgument(args));

	if (fileType == "l") {
		if (args.empty()) {
			cout << "Error: invalid number of arguments for level." << endl;
			return 1;
		}
		string path(args.front());
		args.pop();
		convertLevel(path);
	}
	else if (fileType == "t") {
		if (args.size() < 6) {
			cout << "Error: invalid number of arguments for texture." << endl;
			return 1;
		}

		try {
			convertTexture(args);
		}
		catch (runtime_error& ex) {
			cout << ex.what() << endl;
			return 1;
		}
	}
	else {
		cout << "Error: invalid file type." << endl;
		return 1;
	}

	return 0;
}

Texture::Target stringToTarget(const string& str) {
	if (str == "2d") { return Texture::Target::TEXTURE_2D; }
	if (str == "2da") { return Texture::Target::TEXTURE_2D_ARRAY; }
	throw std::runtime_error("Error: invalid texture target.");
}

Texture::Filter stringToFilter(const string& str) {
	if (str == "n") { return Texture::Filter::NEAREST_NEIGHBOR; }
	if (str == "l") { return Texture::Filter::LINEAR; }
	throw std::runtime_error("Error: invalid texture filter.");
}

Texture::Wrap stringToWrap(const string& str) {
	if (str == "r") { return Texture::Wrap::REPEAT; }
	if (str == "mr") { return Texture::Wrap::MIRRORED_REPEAT; }
	if (str == "ce") { return Texture::Wrap::CLAMP_TO_EDGE; }
	if (str == "cb") { return Texture::Wrap::CLAMP_TO_BORDER; }
	throw std::runtime_error("Error: invalid texture wrap.");
}

void convertTexture(std::queue<string>& args) {
	string srcFile(readArgument(args));
	string destFile(readArgument(args));

	// Load image data
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = stbi_load(srcFile.c_str(), &width, &height, &channels, 4);

	if (!data) {
		const char* reason = stbi_failure_reason();
		throw std::runtime_error("Failed to load texture \'" + srcFile + "\': " + reason);
	}

	RawImage img(data, width, height, channels);

	stbi_image_free(data);
	
	Texture::Target target = stringToTarget(readArgument(args));
	Texture::Filter filter = stringToFilter(readArgument(args));
	Texture::Wrap wrapS = stringToWrap(readArgument(args));
	Texture::Wrap wrapT = stringToWrap(readArgument(args));

	ofstream output(destFile, std::ios_base::out | std::ios_base::binary);
	if (!output) {
		cout << "Failed to write new texture file." << endl;
		output.close();
	}

	// TODO write header
	writeUnsignedInt(output, static_cast<uint32>(target));
	writeUnsignedInt(output, static_cast<uint32>(filter));
	writeUnsignedInt(output, static_cast<uint32>(wrapS));
	writeUnsignedInt(output, static_cast<uint32>(wrapT));
	output.put(img.getChannels());
	
	switch (target) {
	case Texture::Target::TEXTURE_2D:
		convertTexture2D(img, output, args);
		break;
	case Texture::Target::TEXTURE_2D_ARRAY:
		convertTexture2DArray(img, output, args);
		break;
	default:
		output.close();
		throw std::runtime_error("Error: invalid texture target.");
	}

	output.close();
}

void convertTexture2D(const RawImage& img, ofstream& output, std::queue<string>& args) {
	if (args.empty()) {
		writeUnsignedInt(output, img.getWidth());
		writeUnsignedInt(output, img.getHeight());
		output.write(reinterpret_cast<char*>(img.getData()), img.getSizeInBytes());
	}
	else if (args.size() == 4){
		uint32 x = 0;
		uint32 y = 0;
		uint32 width = 0;
		uint32 height = 0;
		try {
			x = stoul(readArgument(args));
			y = stoul(readArgument(args));
			width = stoul(readArgument(args));
			height = stoul(readArgument(args));
		}
		catch (std::invalid_argument& ex) {
			output.close();
			throw std::runtime_error("Error: invalid texture dimensions for TEXTURE_2D target.");
		}

		RawImage subImage = img.getSubImage(x, y, width, height);

		writeUnsignedInt(output, subImage.getWidth());
		writeUnsignedInt(output, subImage.getHeight());
		output.write(reinterpret_cast<char*>(subImage.getData()), subImage.getSizeInBytes());
	}
	else {
		output.close();
		throw std::runtime_error("Error: invalid texture arguments.");
	}
}

void convertTexture2DArray(const RawImage& img, ofstream& output, std::queue<string>& args) {
	if (args.front() == "auto") {
		args.pop();

		uint32 margin = 0;
		uint32 spacing = 0;
		uint32 tileWidth = 0;
		uint32 tileHeight = 0;
		try {
			margin = stoul(readArgument(args));
			spacing = stoul(readArgument(args));
			tileWidth = stoul(readArgument(args));
			tileHeight = stoul(readArgument(args));
		}
		catch (std::invalid_argument& ex) {
			output.close();
			throw std::runtime_error("Error: invalid texture dimensions for TEXTURE_2D target.");
		}

		int32 numTilesPerWidth = (img.getWidth() - (margin * 2) + spacing) / (tileWidth + spacing);
		int32 numTilesPerHeight = (img.getHeight() - (margin * 2) + spacing) / (tileHeight + spacing);

		int32 maxWidth = (numTilesPerWidth * tileWidth) + (spacing * (numTilesPerWidth - 1)) + margin;
		int32 maxHeight = (numTilesPerHeight * tileHeight) + (spacing * (numTilesPerHeight - 1)) + margin;

		uint32 numImgs = numTilesPerWidth * numTilesPerHeight;

		writeUnsignedInt(output, tileWidth);
		writeUnsignedInt(output, tileHeight);
		writeUnsignedInt(output, numImgs);

		int32 tileCount = 0;
		for (int32 y = margin; y < maxHeight; y += tileHeight + spacing) {
			for (int32 x = margin; x < maxWidth; x += tileWidth + spacing) {

				RawImage subImage = img.getSubImage(x, y, tileWidth, tileHeight);

				output.write(reinterpret_cast<char*>(subImage.getData()), subImage.getSizeInBytes());
			}
		}
	}
	else if (!args.empty()) {

		std::vector<geo::Rectangle> dimensions;

		while (!args.empty()) {
			uint32 x = 0;
			uint32 y = 0;
			uint32 width = 0;
			uint32 height = 0;

			try {
				x = stoul(readArgument(args));
				y = stoul(readArgument(args));
				width = stoul(readArgument(args));
				height = stoul(readArgument(args));

				dimensions.push_back(geo::Rectangle(x, y, width, height));
			}
			catch (std::invalid_argument& ex) {
				output.close();
				throw std::runtime_error("Error: invalid texture dimensions.");
			}
		}

		writeUnsignedInt(output, dimensions[0].getWidth());
		writeUnsignedInt(output, dimensions[0].getHeight());
		writeUnsignedInt(output, dimensions.size());

		for (const geo::Rectangle& dimension : dimensions) {
			RawImage subImage = img.getSubImage(dimension.getX(), dimension.getY(), dimension.getWidth(), dimension.getHeight());
			output.write(reinterpret_cast<char*>(subImage.getData()), subImage.getSizeInBytes());
		}
	}
	else {
		output.close();
		throw std::runtime_error("Error: invalid texture arguments.");
	}
}