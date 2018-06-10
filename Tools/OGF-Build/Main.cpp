#include <iostream>
#include <queue>

#include <experimental/filesystem> // TODO Change this when microsoft decides to implement C++17

#include "ConvertLevel.h"
#include "Utils.h"
#include "BuildTextures.h"

using namespace std;
namespace fs = std::experimental::filesystem;

bool buildXML(const string& path);

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
		
		string path(readArgument(args));
		convertLevel(path);
	}
	else if (fileType == "xml") {
		if (args.empty()) {
			cout << "Error: invalid number of arguments for xml." << endl;
			return 1;
		}

		if (!buildXML(readArgument(args))) {
			return 1;
		}
	}
	else if (fileType == "dir") {
		if (args.empty()) {
			cout << "Error: invalid number of arguments for dir." << endl;
			return 1;
		}

		fs::recursive_directory_iterator iter(readArgument(args));
		for (const auto& entry : iter) {
			if (is_regular_file(entry.path()) && entry.path().extension().string() == ".xml") {
				buildXML(entry.path().string());
			}
		}
	}
	else {
		cout << "Error: invalid file type." << endl;
		return 1;
	}

	return 0;
}

bool buildXML(const string& path) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result) {
		cout << "Failed to load xml file \'" << path << "\'" << endl;
		return false;
	}

	if (string(doc.root().first_child().name()) == "texture") {
		buildTexture(doc.root().first_child());
	}
	else {
		cout << "Error: could not identify root xml node." << endl;
		return false;
	}

	return true;
}