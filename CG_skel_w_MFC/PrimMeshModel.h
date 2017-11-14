#pragma once
#include "MeshModel.h"

typedef enum {CUBE, PYRAMID} PrimitiveModelType;



/*
class Box : public MeshModel
{
protected:
public:
	Cube(float size = 1.0)
	{
		vertex_positions.push_back(size * vec3(1.0, 1.0, 1.0));
		vertex_positions.push_back(size * vec3(1.0, 1.0, -1.0));
		vertex_positions.push_back(size * vec3(-1.0, 1.0, 1.0));
		vertex_positions.push_back(size * vec3(-1.0, 1.0, -1.0));

		vertex_positions.push_back(size * vec3(1.0, -1.0, 1.0));
		vertex_positions.push_back(size * vec3(1.0, -1.0, -1.0));
		vertex_positions.push_back(size * vec3(-1.0, -1.0, 1.0));
		vertex_positions.push_back(size * vec3(-1.0, -1.0, -1.0));
	}

	void draw(const Renderer& rend);

};
*/