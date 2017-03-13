#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core.h"
#include "Camera.h"

#include "Texture.h"

#include <vector>

class SpriteRenderer;
class RawImage;

class RenderSystem
{
private:
	Camera m_camera;
	
	SpriteRenderer* m_spriteRenderer;

	vector<Texture*> m_loadedTextures;
public:
	RenderSystem(const Camera& camera);
	~RenderSystem();

	Camera& getCamera() { return m_camera; };

	SpriteRenderer* getSpriteRenderer() const { return m_spriteRenderer; };

	Texture* createTexture2D(const RawImage& img, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT);
	Texture* createTexture2DArray(RawImage* imgs, uint32 layers, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT);
};

#endif