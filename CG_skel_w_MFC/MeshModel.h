#pragma once
#include "vec.h"
#include "mat.h"
#include <string>
#include "Renderer.h"

using namespace std;

class Model {
public:
	virtual ~Model() {}
	void virtual draw(const Renderer& rend) = 0;
};


class MeshModel 
{
protected :
	MeshModel() : boundingBox(NULL), showBox(false) {}
	vector<vec3> vertex_positions;
	//add more attributes
	mat4 _worldTransform;
	mat3 _normalTransform;
	mat4 _modelTransform;
	
	bool showBox;

public:

	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
	void virtual draw(const Renderer& rend, const mat4& cTransform, const mat4& projection );
	
	MeshModel* boundingBox;

	void addWorldTransform(const mat4& transform);
	void addNormalTransform(const mat3& transform);
	void addModelTransform(const mat4& transform);

	void switchBoundingBox();

	void initBoundingBox();

	float getXmax() const;
	float getXmin() const;

	float getYmax() const;
	float getYmin() const;

	float getZmax() const;
	float getZmin() const;

	mat4 getModelTransform() const;
	mat4 getWorldTransform() const;
	mat3 getNormalTransform() const;
	
};

class Cube : public MeshModel
{
	


public:
	Cube(float size = 1.0)
	{
		/*1*/vertex_positions.push_back(size * vec3(1.0, 1.0, 1.0));
		/*2*/vertex_positions.push_back(size * vec3(1.0, 1.0, -1.0));
		/*3*/vertex_positions.push_back(size * vec3(-1.0, 1.0, 1.0));
		/*4*/vertex_positions.push_back(size * vec3(-1.0, 1.0, -1.0));

		/*5*/vertex_positions.push_back(size * vec3(1.0, -1.0, 1.0));
		/*6*/vertex_positions.push_back(size * vec3(1.0, -1.0, -1.0));
		/*7*/vertex_positions.push_back(size * vec3(-1.0, -1.0, 1.0));
		/*8*/vertex_positions.push_back(size * vec3(-1.0, -1.0, -1.0));
	};

	Cube(const vector<vec3>* vertices);

	void virtual draw(const Renderer& rend, const mat4& cTransform, const mat4& projection) override;

};