#define STB_IMAGE_STATIC
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <pugixml.hpp>

#include <Resource/RawImage.h>
#include <Core/Core.h>
#include <Scene/Tile.h>
#include <Math/Geometry/Rectangle.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctype.h>

union Integer {
	int value;
	char bytes[sizeof(int)];
};

union UnsignedInteger {
	unsigned int value;
	char bytes[sizeof(unsigned int)];
};

union Float {
	float value;
	char bytes[sizeof(float)];
};

enum Headers : char {
	TEX2D = 0x1,
	TEX2DARRAY = 0x2
};

using namespace std;

int getResponse();
void choiceTexture();
void choiceLevel();
void convertImageToTexture2D(const string& path);
void choiceSplitInfoFile(const string& path);
void convertImageToTexture2DArray(const string& path, const string& infoPath, const string& name, const vector<uint32>& indices);
void convertLevel(const string& path);
RawImage* loadImage(const string& path);

int main() {
	cout << "What type of resource would you like to create?" << endl;
	cout << "1) Texture" << endl;
	cout << "2) Level" << endl;
	cout << "3) Exit" << endl;

	int response;
	while (true) {
		response = getResponse();
		if (response < 1 || response > 3) {
			cout << "Please enter a valid number." << endl;
		}
		else {
			break;
		}
	}

	try {
		switch (response) {
		case 1:
			choiceTexture();
			break;
		case 2:
			choiceLevel();
			break;
		case 3:
			break;
		default:
			break;
		}
	}
	catch (runtime_error& ex) {
		cout << ex.what() << endl;
	}

}

void choiceTexture() {
	cout << "Please enter the path to the image you wish to turn into a texture." << endl;
	cin >> std::ws;
	string path;
	getline(cin, path);

	cout << "Would you like to split the image into a texture array? (Splitting requires all the chosen sub images be the same dimensions)" << endl;
	cout << "1) No Split" << endl;
	cout << "2) Split using info file" << endl;
	cout << "3) Split using tile dimensions" << endl;
	cout << "4) Cancel" << endl;

	int response;
	while (true) {
		response = getResponse();
		if (response < 1 || response > 4) {
			cout << "Please enter a valid number." << endl;
		}
		else {
			break;
		}
	}

	switch (response) {
	case 1:
		convertImageToTexture2D(path);
		break;
	case 2:
		choiceSplitInfoFile(path);
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

void choiceLevel() {
	cout << "Please enter the path to the .tmx file you wish to convert." << endl;
	cin >> std::ws;
	string path;
	getline(cin, path);
	convertLevel(path);
}

void choiceSplitInfoFile(const string& path) {
	cout << "Please enter the path for the info file." << endl;
	cin >> std::ws;
	string infoPath;
	getline(cin, infoPath);

	cout << "Please enter a name for the output file." << endl;
	cin >> std::ws;
	string name;
	getline(cin, name);

	cout << "Enter the indices of the sub images you wish to use. (Seperated by commas)" << endl;
	vector<uint32> indices;
	cin >> std::ws;
	string indexStr;
	getline(cin, indexStr);

	stringstream ss;
	for (int i = 0; i < indexStr.length(); i++) {
		if (indexStr[i] != ',') {
			ss << indexStr[i];
		}
		else {
			indices.push_back(stoi(ss.str()));
			ss.str("");
		}
	}
	indices.push_back(stoi(ss.str()));

	convertImageToTexture2DArray(path, infoPath, name, indices);
}

int getResponse() {
	string responseStr;

	try {
		cin >> responseStr;
		int response = stoi(responseStr);
		return response;
	}
	catch (invalid_argument& ex) {
		return -1;
	}
	catch (out_of_range& ex) {
		return -1;
	}

	return -1;
}

RawImage* loadImage(const string& path) {
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (!data) {
		const char* reason = stbi_failure_reason();
		throw std::runtime_error("Failed to load texture \'" + path + "\': " + reason);
	}

	RawImage* img = new RawImage(data, width, height, channels);

	stbi_image_free(data);

	return img;
}

void convertImageToTexture2D(const string& path) {

	RawImage* img = loadImage(path);

	// Convert and save tx file
	string originalFileName = path.substr(path.find_last_of("/\\") + 1);
	string newFileName = originalFileName.substr(0, originalFileName.find_last_of(".")).append(".tx");
	string newPath = path.substr(0, path.find_last_of("/\\") + 1).append(newFileName);

	ofstream output(newPath);

	if (!output) {
		cout << "Failed to write new texture file." << endl;
		output.close();
		delete img;
		return;
	}

	UnsignedInteger integer;

	output.put(TEX2D);

	integer.value = img->getWidth();
	output.write(integer.bytes, sizeof(unsigned int));

	integer.value = img->getHeight();
	output.write(integer.bytes, sizeof(unsigned int));

	output.put(img->getChannels());

	output.write(reinterpret_cast<char*>(img->getData()), img->getSizeInBytes());

	cout << "Done." << endl;

	output.close();
	delete img;
}

void convertImageToTexture2DArray(const string& path, const string& infoPath, const string& name, const vector<uint32>& indices) {
	
	RawImage* img = loadImage(path);

	// Read info file
	std::ifstream input(infoPath);

	if (!input) {
		delete img;
		input.close();
		cout << "Failed to load texture \'" << path << "\': Failed to load info file." << endl;
		return;
	}

	string line;
	uint32 numberOfFrames = 0;
	input >> numberOfFrames;

	RawImage* imgs = new RawImage[numberOfFrames];

	for (uint32 i = 0; i < numberOfFrames; i++) {
		input >> line;
		input >> line;
		input >> line;
		uint32 x = stoul(line);
		input >> line;
		uint32 y = stoul(line);
		input >> line;
		uint32 width = stoul(line);
		input >> line;
		uint32 height = stoul(line);

		imgs[i] = img->getSubImage(x, y, width, height);
	}

	unsigned int dataSize = imgs[indices[0]].getWidth() * imgs[indices[0]].getHeight() * imgs[indices[0]].getChannels() * indices.size();
	unsigned char* _data = new unsigned char[dataSize];
	cout << dataSize << endl;
	for (uint32 i = 0; i < indices.size(); i++) {
		for (uint32 j = 0; j < imgs[indices[i]].getSizeInBytes(); j++) {
			_data[(i * imgs[indices[i]].getSizeInBytes()) + j] = imgs[indices[i]].getData()[j];
		}
	}

	// Convert and save tx file
	string originalFileName = path.substr(path.find_last_of("/\\") + 1);
	string newFileName = name + ".tx";
	string newPath = path.substr(0, path.find_last_of("/\\") + 1).append(newFileName);
	ofstream output(newPath);

	if (!output) {
		cout << "Failed to write new texture file." << endl;
		output.close();
		delete[] imgs;
		delete img;
		return;
	}

	UnsignedInteger integer;

	output.put((indices.size() > 1) ? TEX2DARRAY:TEX2D);

	integer.value = imgs[indices[0]].getWidth();
	output.write(integer.bytes, sizeof(unsigned int));

	integer.value = imgs[indices[0]].getHeight();
	output.write(integer.bytes, sizeof(unsigned int));

	output.put(imgs[indices[0]].getChannels());

	if (indices.size() > 1) {
		integer.value = indices.size();
		output.write(integer.bytes, sizeof(unsigned int));
	}

	output.write(reinterpret_cast<char*>(_data), dataSize);

	cout << "Done." << endl;

	output.close();
	delete[] imgs;
	delete img;
}

void convertLevel(const string& path) {
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result) {
		cout << "Failed to load tmx file \'" << path << "\'" << endl;
		return;
	}

	string originalFileName = path.substr(path.find_last_of("/\\") + 1);
	string newFileName = originalFileName.substr(0, originalFileName.find_last_of(".")).append(".lvl");
	string newPath = path.substr(0, path.find_last_of("/\\") + 1).append(newFileName);

	ofstream output(newPath, std::ios_base::out | std::ios_base::binary);

	if (!output) {
		cout << "Failed to write new lvl file." << endl;
		output.close();
		return;
	}

	pugi::xml_node mapNode = doc.child("map");
	float32 width = mapNode.attribute("width").as_float();
	float32 height = mapNode.attribute("height").as_float();
	float32 rectX = (width / 2.0f) - 0.5f;
	float32 rectY = (height / -2.0f) + 0.5f;

	Float floatUnion;
	UnsignedInteger uinteger;
	Integer integer;


	floatUnion.value = rectX;
	output.write(floatUnion.bytes, sizeof(float));
	floatUnion.value = rectY;
	output.write(floatUnion.bytes, sizeof(float));
	floatUnion.value = width;
	output.write(floatUnion.bytes, sizeof(float));
	floatUnion.value = height;
	output.write(floatUnion.bytes, sizeof(float));

	// ######################### TILESETS ###########################
	auto tilesetNodes = mapNode.children("tileset");
	uint32 numTilesets = 0;
	for (const pugi::xml_node& tilesetNode : tilesetNodes) {
		numTilesets++;
	}

	uinteger.value = numTilesets;
	output.write(uinteger.bytes, sizeof(unsigned int));

	vector<pair<uint32, uint32>> firstgids;

	for (const pugi::xml_node& tilesetNode : tilesetNodes) {
		pugi::xml_node imgNode = tilesetNode.child("image");
		string texturePath = imgNode.attribute("source").value();
		string textureName = texturePath.substr(texturePath.find_last_of("/\\") + 1);

		uinteger.value = textureName.size();
		output.write(uinteger.bytes, sizeof(unsigned int));
		for (uint32 i = 0; i < textureName.size(); i++) {
			output.put(textureName[i]);
		}

		uinteger.value = tilesetNode.attribute("margin").as_uint();
		output.write(uinteger.bytes, sizeof(unsigned int));
		uinteger.value = tilesetNode.attribute("spacing").as_uint();
		output.write(uinteger.bytes, sizeof(unsigned int));
		uinteger.value = tilesetNode.attribute("tilewidth").as_uint();
		output.write(uinteger.bytes, sizeof(unsigned int));
		uinteger.value = tilesetNode.attribute("tileheight").as_uint();
		output.write(uinteger.bytes, sizeof(unsigned int));

		firstgids.push_back({ tilesetNode.attribute("firstgid").as_uint(), tilesetNode.attribute("tilecount").as_uint() });
	}

	// ################################# TILES #########################################
	vector<Tile>* tileLayers = new vector<Tile>[numTilesets];

	pugi::xml_node dataNode = mapNode.child("layer").child("data");
	if (string(dataNode.attribute("encoding").value()) != "csv") {
		cout << "Failed to write new lvl file.  Encoding not supported." << endl;
		output.close();
		return;
	}

	string data = dataNode.child_value();
	stringstream ss;
	float32 x = 0;
	float32 y = 0;
	for (uint32 i = 0; i < data.size(); i++) {
		if (isdigit(data[i])) {
			ss << data[i];
		}
		else if (data[i] == ',') {
			int32 index = stoi(ss.str());
			uint32 layer = 0;
			for (uint32 i = 0; i < firstgids.size(); i++) {
				if (index >= firstgids[i].first && index < firstgids[i].second + firstgids[i].first) {
					layer = i;
					index -= firstgids[i].first;
					break;
				}
			}
			//index--;

			if (index > 0) {
				Tile tile(x, -y, index);
				tileLayers[layer].push_back(tile);
			}
			x++;
			if (x >= width) {
				x = 0;
				y++;
			}
			ss.str("");
		}
	}
	int32 index = stoi(ss.str());
	uint32 layer = 0;
	for (uint32 i = 0; i < firstgids.size(); i++) {
		if (index >= firstgids[i].first && index < firstgids[i].second + firstgids[i].first) {
			layer = i;
			index -= firstgids[i].first;
			break;
		}
	}
	//index--;

	if (index > -1) {
		Tile tile(x, -y, index);
		tileLayers[layer].push_back(tile);
	}

	uinteger.value = numTilesets;
	output.write(uinteger.bytes, sizeof(unsigned int));

	for (uint32 i = 0; i < numTilesets; i++) {
		const vector<Tile>& tiles = tileLayers[i];
		uinteger.value = tiles.size();
		output.write(uinteger.bytes, sizeof(unsigned int));

		for (const Tile& tile : tiles) {
			floatUnion.value = tile.x;
			output.write(floatUnion.bytes, sizeof(float));
			floatUnion.value = tile.y;
			output.write(floatUnion.bytes, sizeof(float));
			integer.value = tile.index;
			output.write(integer.bytes, sizeof(int));
		}
	}

	// ########################## COLLIDERS #############################
	vector<Rectangle> colliders;
	pugi::xml_node groupNode = mapNode.child("objectgroup");

	if (string(groupNode.attribute("name").value()) == "collision") {
		for (auto it = groupNode.begin(); it != groupNode.end(); it++) {
			float32 collX = it->attribute("x").as_int() / ((float32)mapNode.attribute("tilewidth").as_int());
			float32 collY = it->attribute("y").as_int() / ((float32)mapNode.attribute("tileheight").as_int());
			float32 collWidth = it->attribute("width").as_int() / ((float32)mapNode.attribute("tilewidth").as_int());
			float32 collHeight = it->attribute("height").as_int() / ((float32)mapNode.attribute("tileheight").as_int());

			// Shift origin to center
			collX += (collWidth / 2.0f) - 0.5f;
			collY += (collHeight / 2.0f) - 0.5f;

			Rectangle collider(collX, collY, collWidth, collHeight);
			colliders.push_back(collider);
		}
	}

	uinteger.value = colliders.size();
	output.write(uinteger.bytes, sizeof(unsigned int));

	for (const Rectangle& collider : colliders) {
		floatUnion.value = collider.getX();
		output.write(floatUnion.bytes, sizeof(float));
		floatUnion.value = -collider.getY();
		output.write(floatUnion.bytes, sizeof(float));
		floatUnion.value = collider.getWidth();
		output.write(floatUnion.bytes, sizeof(float));
		floatUnion.value = collider.getHeight();
		output.write(floatUnion.bytes, sizeof(float));
	}

	delete[] tileLayers;
	cout << "Done." << endl;

	output.close();
}