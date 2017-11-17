#include "stdafx.h"
#include "assert.h"
#include "MeshModel.h"

Cube::Cube(const vector<vec3>* vertices) 
{
	assert(vertices && vertices->size() >= 8);
	for (int i = 0; i < 8; i++)
		vertex_positions.push_back((*vertices)[i]);
}

void Cube::draw(const Renderer& rend, const mat4& cTransform, const mat4& projection)
{

		rend.DrawCube(&vertex_positions, _modelTransform, _worldTransform, cTransform, projection, NULL);

}





