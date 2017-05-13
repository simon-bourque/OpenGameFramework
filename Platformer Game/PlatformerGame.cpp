#include "PlatformerGame.h"

#include "Core/Core.h"

#include "Graphics/Renderer/ShapeRenderer.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Window.h"
#include "Graphics/Color.h"
#include "Graphics/Background.h"

#include "Math/Geometry/Rectangle.h"
#include "Math/Vector2f.h"

#include "Input/Input.h"

#include "Scene/SceneManager.h"

#include "Object/Component/AABBColliderComponent.h"
#include "Object/Component/ComponentType.h"

#include "Physics/Collision/CollisionSystem.h"
#include "Physics/Collision/QuadTree.h"

#include "Player.h"

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {}

void PlatformerGame::init() {
	Input::get()->addKeyListener(this, &PlatformerGame::onKeyPress);
	//RenderSystem::get()->getCamera().getTransform().translate(11.0f,-5.5f);
	SceneManager::get()->loadTileLevel("level_0.lvl");
	SceneManager::get()->getCurrentScene().setGravity(9.8f * 4.0f);

	// Load background
	Texture* bgTexture = RenderSystem::get()->getTextureManager()->createTexture2D("uncolored_forest.tx", Texture::Filter::NEAREST_NEIGHBOR);
	SceneManager::get()->getCurrentScene().addBackground(new Background(bgTexture));

	// load player
	Player* player = new Player(this, Vector2f(0.5f, -10.8f));
	SceneManager::get()->getCurrentScene().addGameObject(player);
	m_player = player;
}

void PlatformerGame::tick(float32 delta) {
	Game::tick(delta);

	const static float32 SPEED = 25.0f;

	if (up) {
		//getRenderSystem()->getCamera().getTransform().translate(0.0f, SPEED * delta);
	}
	if (down) {
		//getRenderSystem()->getCamera().getTransform().translate(0.0f, -SPEED * delta);
	}
	if (left) {
		//getRenderSystem()->getCamera().getTransform().translate(-SPEED * delta, 0.0f);
	}
	if (right) {
		//getRenderSystem()->getCamera().getTransform().translate(SPEED * delta, 0.0f);
	}
}

void PlatformerGame::render() {
	Game::render();

	ObjectComponent* ptr = m_player->findComponent(ComponentType::AABB_COLLIDER_COMPONENT);
	if (ptr) {
		AABBColliderComponent* collider = static_cast<AABBColliderComponent*>(ptr);
		RenderSystem::get()->getShapeRenderer()->drawRectangle(collider->getRectangle(), Color::MAGENTA, false);

		std::vector<Rectangle> rects;
		SceneManager::get()->getCurrentScene().getCollisionSystem()->getQuadTree()->retrieve(collider->getRectangle(), rects);
		for (const Rectangle& rect : rects) {
			RenderSystem::get()->getShapeRenderer()->drawRectangle(rect, Color::YELLOW, true);
		}
	}
}

void PlatformerGame::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
	if ((key == Input::KEY_UP || key == Input::KEY_W)) {
		if (action == Input::PRESS) {
			up = true;
		}
		if (action == Input::RELEASE) {
			up = false;
		}
	}
	if ((key == Input::KEY_DOWN || key == Input::KEY_S)) {
		if (action == Input::PRESS) {
			down = true;
		}
		if (action == Input::RELEASE) {
			down = false;
		}
	}
	if ((key == Input::KEY_LEFT || key == Input::KEY_A)) {
		if (action == Input::PRESS) {
			left = true;
		}
		if (action == Input::RELEASE) {
			left = false;
		}
	}
	if ((key == Input::KEY_RIGHT || key == Input::KEY_D)) {
		if (action == Input::PRESS) {
			right = true;
		}
		if (action == Input::RELEASE) {
			right = false;
		}
	}
}

