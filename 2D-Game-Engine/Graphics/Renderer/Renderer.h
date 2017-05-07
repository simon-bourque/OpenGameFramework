#pragma once
#ifndef RENDERER_H
#define RENDERER_H

class RenderSystem;

class Renderer
{
protected:
	RenderSystem* m_rs;
	explicit Renderer(RenderSystem* rs);
public:
	virtual ~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
};

#endif

