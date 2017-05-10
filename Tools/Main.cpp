#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Resource/RawImage.h>
#include <Core/Core.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

union UnsignedInteger {
	unsigned int value;
	char bytes[sizeof(unsigned int)];
};

enum Headers : char {
	TEX2D = 0x1,
	TEX2DARRAY = 0x2
};

using namespace std;

int getResponse();
void choiceTexture();
void convertImageToTexture2D(const string& path);
void choiceSplitInfoFile(const string& path);
void convertImageToTexture2DArray(const string& path, const string& infoPath, const string& name, const vector<uint32>& indices);
RawImage* loadImage(const string& path);

int main() {
	cout << "What type of resource would you like to create?" << endl;
	cout << "1) Texture" << endl;
	cout << "2) Exit" << endl;

	int response;
	while (true) {
		response = getResponse();
		if (response < 1 || response > 2) {
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