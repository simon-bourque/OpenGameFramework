#include "PlatformerGame.h"

#include "Core/Core.h"

#include "Core/Graphics/GraphicsContext.h"
#include "2D/Graphics/Renderer/ShapeRenderer.h"
#include "Core/Graphics/TextureCache.h"
#include "Core/Graphics/Color.h"

#include "2D/Scene/Background.h"

#include "Core/Math/Geometry/Rectangle.h"

#include "Core/Input/Input.h"

#include "2D/Scene/SceneManager2D.h"

#include "2D/Object/Component/AABBColliderComponent.h"
#include "2D/Object/Component/ComponentType.h"
#include "2D/Object/Component/RigidBodyComponent.h"

#include "2D/Physics/Collision/CollisionSystem.h"
#include "2D/Physics/Collision/QuadTree.h"

#include "Player.h"

PlatformerGame::PlatformerGame() : Game2D("Platformer", 720, 576, geo::Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {}

void PlatformerGame::init() {
	getInputInstance()->addKeyListener(KeyListener::create<PlatformerGame, &PlatformerGame::onKeyPress>(this));
	//getSceneManagerInstance()->loadTileLevel("level_0.lvl");
	getSceneManager2DInstance()->loadTileLevel("layer_test.lvl");
	getSceneManager2DInstance()->getCurrentScene().setGravity(9.8f * 4.0f);

	// Load background
	TextureRef bgTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("uncolored_forest.tx");
	getSceneManager2DInstance()->getCurrentScene().addBackground(new Background(bgTextureRef));

	// load player
	//m_playerSpawn = Vector2f(0.5f, -10.8f);
	m_playerSpawn = Vector2f(41.0f, -54.8f);

	Player* player = new Player(this, m_playerSpawn);
	getSceneManager2DInstance()->getCurrentScene().addGameObject(player);
	m_player = player;
}

void PlatformerGame::tick(float32 delta) {
	Game2D::tick(delta);
}

void PlatformerGame::render() {
	Game2D::render();
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

