#define STB_IMAGE_STATIC
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Resource/RawImage.h>
#include <Core/Core.h>
#include <Math/Geometry/Rectangle.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "ConvertLevel.h"
#include "Utils.h"

enum Headers : char {
	TEX2D = 0x1,
	TEX2DARRAY = 0x2
};

using namespace std;

void convertImageToTexture2D(const RawImage& img, const string& newPath);
void convertImageToTexture2DArray(const RawImage& img, const string& newPath, const vector<geo::Rectangle>& dims);
void convertImageToTexture2DArray(const RawImage& img, const string& newPath, uint32 margin, uint32 spacing, uint32 tileWidth, uint32 tileHeight);
RawImage loadImage(const string& path);


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
	string fileType(args.front());
	args.pop();

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
		if (args.empty()) {
			cout << "Error: invalid number of arguments for texture." << endl;
			return 1;
		}
		string path(args.front());
		args.pop();
		RawImage img = loadImage(path);

		// If there are no more arguments assume user wants to convert whole image
		if (args.empty()) {
			string originalFileName = path.substr(path.find_last_of("/\\") + 1);
			string newFileName = originalFileName.substr(0, originalFileName.find_last_of(".")).append(".tx");
			string newPath = path.substr(0, path.find_last_of("/\\") + 1).append(newFileName);

			convertImageToTexture2D(img, newPath);
		}
		else {
			if (args.size() != 5) {
				cout << "Error: invalid number of arguments for texture." << endl;
				return 1;
			}
			string newPath(args.front());
			args.pop();

			uint32 x = 0;
			uint32 y = 0;
			uint32 width = 0;
			uint32 height = 0;

			try {
				x = stoul(args.front());
				args.pop();
				y = stoul(args.front());
				args.pop();
				width = stoul(args.front());
				args.pop();
				height = stoul(args.front());
				args.pop();
			}
			catch (std::invalid_argument& ex) {
				cout << "Error: invalid arguments for texture." << endl;
				return 1;
			}

			RawImage subImg = img.getSubImage(x, y, width, height);
			convertImageToTexture2D(subImg, newPath);
		}
	}
	else if (fileType == "ta") {
		if (args.size() < 6) {
			cout << "Error: invalid number of arguments for texture array." << endl;
			return 1;
		}
		string path(args.front());
		args.pop();
		RawImage img = loadImage(path);

		string newPath(args.front());
		args.pop();

		if (args.front() == "auto") {
			args.pop();
			if (args.size() == 4) {
				uint32 margin = 0;
				uint32 spacing = 0;
				uint32 tileWidth = 0;
				uint32 tileHeight = 0;
				try {
					margin = stoul(args.front());
					args.pop();
					spacing = stoul(args.front());
					args.pop();
					tileWidth = stoul(args.front());
					args.pop();
					tileHeight = stoul(args.front());
					args.pop();
				}
				catch (std::invalid_argument& ex) {
					cout << "Error: invalid arguments for texture array." << endl;
					return 1;
				}

				convertImageToTexture2DArray(img, newPath, margin, spacing, tileWidth, tileHeight);
			}
			else {
				cout << "Error: invalid number of arguments for texture array." << endl;
				return 1;
			}
		}
		else {
			std::vector<geo::Rectangle> dims;

			while (!args.empty()) {
				if (args.size() >= 4) {
					try {
						uint32 x = stoul(args.front());
						args.pop();
						uint32 y = stoul(args.front());
						args.pop();
						uint32 width = stoul(args.front());
						args.pop();
						uint32 height = stoul(args.front());
						args.pop();
						dims.push_back(geo::Rectangle(x, y, width, height));
					}
					catch (std::invalid_argument& ex) {
						cout << "Error: invalid arguments for texture array." << endl;
						return 1;
					}
				}
				else {
					cout << "Error: invalid number of arguments for texture array." << endl;
					return 1;
				}
			}

			convertImageToTexture2DArray(img, newPath, dims);
		}
	}
	else {
		cout << "Error: invalid file type." << endl;
		return 1;
	}

	return 0;
}

RawImage loadImage(const string& path) {
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (!data) {
		const char* reason = stbi_failure_reason();
		throw std::runtime_error("Failed to load texture \'" + path + "\': " + reason);
	}

	RawImage img(data, width, height, channels);

	stbi_image_free(data);

	return std::move(img);
}

void convertImageToTexture2D(const RawImage& img, const string& newPath) {

	// Convert and save tx file
	ofstream output(newPath, std::ios_base::out | std::ios_base::binary);

	if (!output) {
		cout << "Failed to write new texture file." << endl;
		output.close();
		return;
	}

	output.put(TEX2D);

	writeUnsignedInt(output, img.getWidth()); // WIDTH
	writeUnsignedInt(output, img.getHeight()); // HEIGHT
	output.put(img.getChannels()); // CHANNELS

	output.write(reinterpret_cast<char*>(img.getData()), img.getSizeInBytes());

	cout << "Done." << endl;

	output.close();
}

void convertImageToTexture2DArray(const RawImage& img, const string& newPath, const vector<geo::Rectangle>& dims) {
	
	RawImage* imgs = new RawImage[dims.size()];

	for (int32 i = 0; i < dims.size(); i++) {
		geo::Rectangle rect = dims[i];
		imgs[i] = img.getSubImage(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
	}

	uint32 dataSize = imgs[0].getWidth() * imgs[0].getHeight() * imgs[0].getChannels() * dims.size();
	uint8* data = new uint8[dataSize];

	for (int32 i = 0; i < dims.size(); i++) {
		for (int32 j = 0; j < imgs[i].getSizeInBytes(); j++) {
			data[(i * imgs[i].getSizeInBytes()) + j] = imgs[i].getData()[j];
		}
	}

	// Convert and save tx file
	ofstream output(newPath, std::ios_base::out | std::ios_base::binary);

	if (!output) {
		cout << "Failed to write new texture file." << endl;
		output.close();
		delete[] imgs;
		delete[] data;
		return;
	}

	output.put(TEX2DARRAY);

	writeUnsignedInt(output, imgs[0].getWidth()); // WIDTH
	writeUnsignedInt(output, imgs[0].getHeight()); // HEIGHT
	output.put(imgs[0].getChannels()); // CHANNELS
	writeUnsignedInt(output, dims.size()); // DEPTH

	output.write(reinterpret_cast<char*>(data), dataSize);

	cout << "Done." << endl;

	output.close();

	delete[] imgs;
	delete[] data;
}

void convertImageToTexture2DArray(const RawImage& img, const string& newPath, uint32 margin, uint32 spacing, uint32 tileWidth, uint32 tileHeight) {
	int32 numTilesPerWidth = (img.getWidth() - (margin * 2) + spacing) / (tileWidth + spacing);
	int32 numTilesPerHeight = (img.getHeight() - (margin * 2) + spacing) / (tileHeight + spacing);

	int32 maxWidth = (numTilesPerWidth * tileWidth) + (spacing * (numTilesPerWidth - 1)) + margin;
	int32 maxHeight = (numTilesPerHeight * tileHeight) + (spacing * (numTilesPerHeight - 1)) + margin;

	uint32 numImgs = numTilesPerWidth * numTilesPerHeight;
	RawImage* imgs = new RawImage[numImgs];


	int32 tileCount = 0;
	for (int32 y = margin; y < maxHeight; y += tileHeight + spacing) {
		for (int32 x = margin; x < maxWidth; x += tileWidth + spacing) {

			RawImage subImage = img.getSubImage(x, y, tileWidth, tileHeight);

			imgs[tileCount++] = subImage;
		}
	}

	uint32 dataSize = imgs[0].getWidth() * imgs[0].getHeight() * imgs[0].getChannels() * numImgs;
	uint8* data = new uint8[dataSize];

	for (int32 i = 0; i < numImgs; i++) {
		for (int32 j = 0; j < imgs[i].getSizeInBytes(); j++) {
			data[(i * imgs[i].getSizeInBytes()) + j] = imgs[i].getData()[j];
		}
	}

	// Convert and save tx file
	ofstream output(newPath, std::ios_base::out | std::ios_base::binary);

	if (!output) {
		cout << "Failed to write new texture file." << endl;
		output.close();
		delete[] imgs;
		delete[] data;
		return;
	}

	output.put(TEX2DARRAY);

	writeUnsignedInt(output, imgs[0].getWidth()); // WIDTH
	writeUnsignedInt(output, imgs[0].getHeight()); // HEIGHT
	output.put(imgs[0].getChannels()); // CHANNELS
	writeUnsignedInt(output, numImgs); // DEPTH

	output.write(reinterpret_cast<char*>(data), dataSize);

	cout << "Done." << endl;

	output.close();

	delete[] imgs;
	delete[] data;
}