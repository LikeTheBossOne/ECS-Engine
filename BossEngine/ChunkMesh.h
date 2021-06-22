#pragma once

struct ChunkMesh
{
	unsigned int vertexCount;
	unsigned int dataIndex;
	unsigned int indicesIndex;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	float* dataBuffer;
	unsigned int* indexBuffer;
};