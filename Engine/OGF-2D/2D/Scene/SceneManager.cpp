#include "SceneManager.h"

#include "Core/Resource/Resources.h"

#include "2D/Scene/TileScene.h"
#include "2D/Scene/Tile.h"
#include "2D/Scene/TileLayer.h"

#include "2D/Physics/Collision/CollisionSystem.h"

#include "Core/Resource/File/FileReader.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/Texture.h"
#include "Core/Graphics/TextureManager.h"

static TileScene* loadTileLevelFromDisk(string file);
const static string LEVEL_PATH = "res/level/";

SceneManager::SceneManager() : m_paused(false) {
	m_currentScene.reset(new Scene(geo::Rectangle(100,100)));
}


SceneManager::~SceneManager() {}

void SceneManager::loadTileLevel(const string& file) {
	TileScene* scene = loadTileLevelFromDisk(file);
	m_currentScene.reset(static_cast<Scene*>(scene));
}

void SceneManager::tickCurrentScene(float32 delta) {
	if (!m_paused) {
		m_currentScene->tick(delta);
	}
}

void SceneManager::renderCurrentScene() {
	m_currentScene->render();
}

static TileScene* loadTileLevelFromDisk(string file) {
	FileReader input(LEVEL_PATH + file);

	// Load level bounds
	float32 boundsX = input.read<float32>();
	float32 boundsY = input.read<float32>();
	float32 boundsWidth = input.read<float32>();
	float32 boundsHeight = input.read<float32>();

	// Load tilesets
	uint32 numTilesets = input.read<uint32>();
	Texture** textures = new Texture*[numTilesets];

	for (uint32 i = 0; i < numTilesets; i++) {

		uint32 stringLen = input.read<uint32>();

		char* texPath = new char[stringLen + 1];
		uint32 bytesRead = 0;
		input.read((uint8*)texPath, stringLen, bytesRead);
		texPath[stringLen] = '\0';

		DEBUG_LOG("--- TILE SHEET PATH ---");
		DEBUG_LOG(texPath);

		Texture* texture = getGraphicsContextInstance()->getTextureManager()->createTexture2DArray(texPath, Texture::Filter::NEAREST_NEIGHBOR);
		textures[i] = texture;
		delete[] texPath;
	}

	// Load tileLayers
	uint32 numLayers = input.read<uint32>();
	TileLayer** layers = new TileLayer*[numLayers];

	for (uint32 i = 0; i < numLayers; i++) {
		uint32 numTiles = input.read<uint32>();
		uint32 tilesetIndex = input.read<uint32>();

		Tile* tiles = new Tile[numTiles];
		for (int32 i = 0; i < numTiles; i++) {
			float32 x = input.read<float32>();
			float32 y = input.read<float32>();
			int32 index = input.read<int32>();

			tiles[i] = { x, y, index };
		}

		layers[i] = new TileLayer(tiles, numTiles, textures[tilesetIndex]);
		delete[] tiles;
	}

	// Load colliders
	uint32 numColliders = input.read<uint32>();
	geo::Rectangle* colliders = new geo::Rectangle[numColliders];

	for (int32 i = 0; i < numColliders; i++) {
		colliders[i].setX(input.read<float32>());
		colliders[i].setY(input.read<float32>());
		colliders[i].setWidth(input.read<float32>());
		colliders[i].setHeight(input.read<float32>());
	}

	input.close();

	TileScene* scene = new TileScene(geo::Rectangle(boundsX, boundsY, boundsWidth, boundsHeight));

	for (uint32 i = 0; i < numLayers; i++) {
		scene->addTileLayer(layers[i]);
	}

	for (uint32 i = 0; i < numColliders; i++) {
		scene->getCollisionSystem()->addStaticCollider(colliders[i]);
	}


	delete[] textures;
	delete[] colliders;
	delete[] layers;

	return scene;
}