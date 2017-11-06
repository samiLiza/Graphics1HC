#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>
#include "Renderer.h"

using namespace std;

class MeshModel : public Model
{
protected :
	MeshModel() {}
	vector<vec3> vertex_positions;
	//add more attributes
	mat4 _worldTransform;
	mat3 _normalTransform;
	mat4 _modelTransform;

public:

	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
	void draw(const Renderer& rend);
	void addWorldTransform(const mat4& transform);
	void addNormalTransform(const mat3& transform);
	void addModelTransform(const mat4& transform);
	
};
