#include "TileLayer.h"

#include "Core/Graphics/Memory/VertexArrayObject.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/SpriteRenderer.h"
#include "2D/Graphics/Renderer/SceneRenderer.h"

#include "Core/Math/Matrix3f.h"

#include "2D/Scene/Tile.h"

TileLayer::TileLayer(Tile* tiles, int32 numberOfTiles, Texture* tileSheet) : m_tileSheet(tileSheet), m_numberOfTiles(numberOfTiles) {
	float32* matrices = new float32[numberOfTiles * 9];
	int32* indices = new int32[numberOfTiles];

	for (int32 i = 0; i < numberOfTiles; i++) {
		Tile tile = tiles[i];
		Matrix3f matrix = Matrix3f::translation(tile.x, tile.y).transpose();

		matrices[i * 9] = matrix.m00;
		matrices[(i * 9) + 1] = matrix.m01;
		matrices[(i * 9) + 2] = matrix.m02;
		matrices[(i * 9) + 3] = matrix.m10;
		matrices[(i * 9) + 4] = matrix.m11;
		matrices[(i * 9) + 5] = matrix.m12;
		matrices[(i * 9) + 6] = matrix.m20;
		matrices[(i * 9) + 7] = matrix.m21;
		matrices[(i * 9) + 8] = matrix.m22;

		indices[i] = tile.index;
	}

	m_tileVAO.reset(new VertexArrayObject());

	m_tileVAO->bind();

	m_tileVAO->addArrayBuffer(0, SpriteRenderer::SPRITE_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_tileVAO->addArrayBuffer(1, SpriteRenderer::SPRITE_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);

	Buffer* matrixBuffer = new Buffer(Buffer::Target::ARRAY_BUFFER);
	Buffer* indexBuffer = new Buffer(Buffer::Target::ARRAY_BUFFER);

	indexBuffer->bind();
	indexBuffer->bufferData(indices, sizeof(int32) * numberOfTiles, Buffer::Usage::STATIC_DRAW);
	indexBuffer->vertexAttributeIPointer(2, 1, GL_INT, 0, nullptr);
	indexBuffer->vertexAttributeDivisor(2, 1);

	m_tileVAO->enableAttribute(2);
	m_tileVAO->addArrayBuffer(indexBuffer);

	matrixBuffer->bind();
	matrixBuffer->bufferData(matrices, sizeof(float32) * numberOfTiles * 9, Buffer::Usage::STATIC_DRAW);

#ifdef DEBUG_BUILD
	static uint32 labelCount = 0;

	string indexLabel("TILE_INDEX_BUFFER_");
	string matrixLabel("TILE_MATRIX_BUFFER_");

	indexLabel.append(std::to_string(labelCount));
	matrixLabel.append(std::to_string(labelCount));

	indexBuffer->setDebugLabel(indexLabel);
	matrixBuffer->setDebugLabel(matrixLabel);

	labelCount++;
#endif

	for (int i = 0; i < 3; i++) {
		matrixBuffer->vertexAttributePointer(3 + i, 3, GL_FLOAT, false, sizeof(float32) * 9 /* size of float * num elements in matrix */, reinterpret_cast<GLvoid*>(sizeof(float) * 3 * i) /* size of float * num elements in row */);
		m_tileVAO->enableAttribute(3 + i);
		matrixBuffer->vertexAttributeDivisor(3 + i, 1);
	}

	m_tileVAO->addArrayBuffer(matrixBuffer);

	matrixBuffer->unbind();
	m_tileVAO->unbind();

	delete[] matrices;
	delete[] indices;

}


TileLayer::~TileLayer() {}

void TileLayer::render() {
	getGraphics2DInstance()->getSceneRenderer().renderTiles(m_tileVAO.get(), m_tileSheet, m_numberOfTiles);
}