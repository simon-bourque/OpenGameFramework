#include "Renderer.h"

#include "EngineAssert.h"

Renderer::Renderer(RenderSystem* rs) : m_rs(rs) {
	ASSERT(rs, "Rendersystem should not be null");
}


Renderer::~Renderer() {}
