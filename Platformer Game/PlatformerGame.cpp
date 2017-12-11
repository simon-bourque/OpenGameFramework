#include "PlatformerGame.h"

#include "Core/Core.h"

#include "Graphics/Renderer/ShapeRenderer.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Color.h"
#include "Graphics/Background.h"

#include "Math/Geometry/Rectangle.h"

#include "Input/Input.h"

#include "Scene/SceneManager.h"

#include "Object/Component/AABBColliderComponent.h"
#include "Object/Component/ComponentType.h"
#include "Object/Component/RigidBodyComponent.h"

#include "Physics/Collision/CollisionSystem.h"
#include "Physics/Collision/QuadTree.h"

#include "Player.h"

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, geo::Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {}

void PlatformerGame::init() {
	getInputInstance()->addKeyListener(KeyListener::create<PlatformerGame, &PlatformerGame::onKeyPress>(this));
	//getSceneManagerInstance()->loadTileLevel("level_0.lvl");
	getSceneManagerInstance()->loadTileLevel("layer_test.lvl");
	getSceneManagerInstance()->getCurrentScene().setGravity(9.8f * 4.0f);

	// Load background
	Texture* bgTexture = getRenderSystemInstance()->getTextureManager()->createTexture2D("uncolored_forest.tx", Texture::Filter::NEAREST_NEIGHBOR);
	getSceneManagerInstance()->getCurrentScene().addBackground(new Background(bgTexture));

	// load player
	//m_playerSpawn = Vector2f(0.5f, -10.8f);
	m_playerSpawn = Vector2f(41.0f, -54.8f);

	Player* player = new Player(this, m_playerSpawn);
	getSceneManagerInstance()->getCurrentScene().addGameObject(player);
	m_player = player;
}

void PlatformerGame::tick(float32 delta) {
	Game::tick(delta);
}

void PlatformerGame::render() {
	Game::render();
}

void PlatformerGame::resetPlayer() {
	RigidBodyComponent* rb = m_player->findComponent<RigidBodyComponent>();
	if (rb) {
		rb->stop();
	}

	m_player->transform.xPos = m_playerSpawn.x;
	m_player->transform.yPos = m_playerSpawn.y;
}

void PlatformerGame::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Keys::KEY_ESCAPE && action == Actions::PRESS) {
		shutdown();
	}
	if (key == Keys::KEY_R && action == Actions::PRESS) {
		resetPlayer();
	}
}

