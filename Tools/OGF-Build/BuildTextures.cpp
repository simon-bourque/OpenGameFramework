#include "BuildTextures.h"

#include "Core/Graphics/Texture.h"
#include "Core/Resource/RawImage.h"
#include "Core/Time.h"

#include "Utils.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <fstream>

#define STB_IMAGE_STATIC
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;

using child_nodes = pugi::xml_object_range<pugi::xml_node_iterator>;

void loadImages(const child_nodes& imageNodes, vector<RawImage>& images);
void loadSubImage(const pugi::xml_node& subImageNode, const RawImage& image, vector<RawImage>& images);
void loadTilesheet(const pugi::xml_node& tilesheetNode, const RawImage& image, vector<RawImage>& images);

Texture::Target stringToTarget(const string& str);
Texture::Filter stringToFilter(const string& str);
Texture::Wrap stringToWrap(const string& str);

void buildTexture(const pugi::xml_node& textureRoot) {
	string name = textureRoot.attribute("name").as_string();

	cout << std::left << setw(45) << "Building texture " + name + "...";
	float64 time = getCurrentTime();

	string typeStr = textureRoot.attribute("type").as_string();
	string filteringStr = textureRoot.attribute("filtering").as_string();
	string horizontalWrapStr = textureRoot.attribute("horizontalwrap").as_string();
	string verticalWrapStr = textureRoot.attribute("verticalwrap").as_string();

	Texture::Target target;
	Texture::Filter filtering;
	Texture::Wrap horizontalWrap;
	Texture::Wrap verticalWrap;

	vector<RawImage> images;
	
	auto error = [&name](const string& reason) {
		cout << endl << "[ERROR] failed to build " << name << ": " << reason << endl;
	};

	try {
		target = stringToTarget(typeStr);
		filtering = stringToFilter(filteringStr);
		horizontalWrap = stringToWrap(horizontalWrapStr);
		verticalWrap = stringToWrap(verticalWrapStr);
	
		loadImages(textureRoot.children(), images);
	}
	catch (const std::runtime_error& ex) {
		error(ex.what());
		return;
	}


	if (images.empty()) {
		error("No source image");
		return;
	}

	// Write to file
	ofstream output(name + ".tx", std::ios_base::out | std::ios_base::binary);

	if (!output) {
		error("Failed to open output file stream");
		return;
	}

	writeUnsignedInt(output, static_cast<uint32>(target));
	writeUnsignedInt(output, static_cast<uint32>(filtering));
	writeUnsignedInt(output, static_cast<uint32>(horizontalWrap));
	writeUnsignedInt(output, static_cast<uint32>(verticalWrap));
	output.put(images[0].getChannels());
	writeUnsignedInt(output, images[0].getWidth());
	writeUnsignedInt(output, images[0].getHeight());

	if (target == Texture::Target::TEXTURE_2D) {
		if (images.size() > 1) {
			error("Too many source images for texture type 2d");
			return;
		}

		output.write(reinterpret_cast<char*>(images[0].getData()), images[0].getSizeInBytes());
	}
	else if (target == Texture::Target::TEXTURE_2D_ARRAY) {
		writeUnsignedInt(output, images.size());
		for (const RawImage& image : images) {

			// Validate width and height
			if (image.getWidth() != images[0].getWidth() || image.getHeight() != images[0].getHeight()) {
				error("All source images for texture type 2darray must have the same dimensions");
				return;
			}

			output.write(reinterpret_cast<char*>(image.getData()), image.getSizeInBytes());
		}
	}

	output.close();

	float64 timeTakenMs = (getCurrentTime() - time) * 1000.0;
	cout << "Took " << std::fixed << std::setprecision(2) << timeTakenMs << "ms" << endl;
}

static void loadImages(const child_nodes& imageNodes, vector<RawImage>& images) {
	for (const pugi::xml_node& imageNode : imageNodes) {
		if (string(imageNode.name()) == "image") {
			string path = imageNode.attribute("path").as_string();

			// Load image data
			int32 width = 0;
			int32 height = 0;
			int32 channels = 0;
			uint8* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

			if (data == nullptr) {
				const char* reason = stbi_failure_reason();
				throw std::runtime_error("Failed to read image \'" + path + "\', " + reason);
			}

			RawImage img(data, width, height, channels);

			stbi_image_free(data);

			if (imageNode.first_child().empty()) {
				images.push_back(std::move(img));
			}
			else {
				for (const pugi::xml_node& childNode : imageNode.children()) {
					if (string(childNode.name()) == "subimage") {
						loadSubImage(childNode, img, images);
					}
					else if (string(childNode.name()) == "tilesheet") {
						loadTilesheet(childNode, img, images);
					}
				}
			}
		}
	}
}

static void loadSubImage(const pugi::xml_node& subImageNode, const RawImage& image, vector<RawImage>& images) {
	uint32 x = subImageNode.attribute("x").as_uint();
	uint32 y = subImageNode.attribute("y").as_uint();
	uint32 width = subImageNode.attribute("width").as_uint();
	uint32 height = subImageNode.attribute("height").as_uint();

	images.push_back(std::move(image.getSubImage(x, y, width, height)));
}

static void loadTilesheet(const pugi::xml_node& tilesheetNode, const RawImage& image, vector<RawImage>& images) {
	uint32 margin = tilesheetNode.attribute("margin").as_uint();
	uint32 spacing = tilesheetNode.attribute("spacing").as_uint();
	uint32 tileWidth = tilesheetNode.attribute("tilewidth").as_uint();
	uint32 tileHeight = tilesheetNode.attribute("tileheight").as_uint();

	int32 numTilesPerWidth = (image.getWidth() - (margin * 2) + spacing) / (tileWidth + spacing);
	int32 numTilesPerHeight = (image.getHeight() - (margin * 2) + spacing) / (tileHeight + spacing);
	int32 maxWidth = (numTilesPerWidth * tileWidth) + (spacing * (numTilesPerWidth - 1)) + margin;
	int32 maxHeight = (numTilesPerHeight * tileHeight) + (spacing * (numTilesPerHeight - 1)) + margin;

	uint32 numImages = numTilesPerWidth * numTilesPerHeight;

	for (int32 y = margin; y < maxHeight; y += tileHeight + spacing) {
		for (int32 x = margin; x < maxWidth; x += tileWidth + spacing) {
			images.push_back(std::move(image.getSubImage(x, y, tileWidth, tileHeight)));
		}
	}
}

static Texture::Target stringToTarget(const string& str) {
	if (str == "2d") { return Texture::Target::TEXTURE_2D; }
	if (str == "2darray") { return Texture::Target::TEXTURE_2D_ARRAY; }
	throw std::runtime_error("Invalid texture target " + str);
}

static Texture::Filter stringToFilter(const string& str) {
	if (str == "nearestneighbor") { return Texture::Filter::NEAREST_NEIGHBOR; }
	if (str == "linear") { return Texture::Filter::LINEAR; }
	throw std::runtime_error("Invalid texture filter " + str);
}

static Texture::Wrap stringToWrap(const string& str) {
	if (str == "repeat") { return Texture::Wrap::REPEAT; }
	if (str == "mirroredrepeat") { return Texture::Wrap::MIRRORED_REPEAT; }
	if (str == "clamptoedge") { return Texture::Wrap::CLAMP_TO_EDGE; }
	if (str == "clamptoborder") { return Texture::Wrap::CLAMP_TO_BORDER; }
	throw std::runtime_error("Invalid texture wrap " + str);
}