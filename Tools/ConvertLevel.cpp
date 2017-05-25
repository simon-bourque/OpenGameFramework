
#include <Core/Core.h>
#include <Scene/Tile.h>
#include <Math/Geometry/Rectangle.h>

#include <pugixml.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Utils.h"

using namespace std;

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

	writeFloat(output, rectX);
	writeFloat(output, rectY);
	writeFloat(output, width);
	writeFloat(output, height);


	// ######################### TILESETS ###########################
	auto tilesetNodes = mapNode.children("tileset");
	uint32 numTilesets = 0;
	for (const pugi::xml_node& tilesetNode : tilesetNodes) {
		numTilesets++;
	}

	writeUnsignedInt(output, numTilesets);

	vector<pair<uint32, uint32>> firstgids;

	for (const pugi::xml_node& tilesetNode : tilesetNodes) {
		pugi::xml_node imgNode = tilesetNode.child("image");
		string texturePath = imgNode.attribute("source").value();
		string textureName = texturePath.substr(texturePath.find_last_of("/\\") + 1);

		writeUnsignedInt(output, textureName.size());
		for (uint32 i = 0; i < textureName.size(); i++) {
			output.put(textureName[i]);
		}

		writeUnsignedInt(output, tilesetNode.attribute("margin").as_uint());
		writeUnsignedInt(output, tilesetNode.attribute("spacing").as_uint());
		writeUnsignedInt(output, tilesetNode.attribute("tilewidth").as_uint());
		writeUnsignedInt(output, tilesetNode.attribute("tileheight").as_uint());

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

	writeUnsignedInt(output, numTilesets);

	for (uint32 i = 0; i < numTilesets; i++) {
		const vector<Tile>& tiles = tileLayers[i];
		writeUnsignedInt(output, tiles.size());

		for (const Tile& tile : tiles) {
			writeFloat(output, tile.x);
			writeFloat(output, tile.y);
			writeInt(output, tile.index);
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

	writeUnsignedInt(output, colliders.size());

	for (const Rectangle& collider : colliders) {
		writeFloat(output, collider.getX());
		writeFloat(output, -collider.getY());
		writeFloat(output, collider.getWidth());
		writeFloat(output, collider.getHeight());
	}

	delete[] tileLayers;
	cout << "Done." << endl;

	output.close();
}