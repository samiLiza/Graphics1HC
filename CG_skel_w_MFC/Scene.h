#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Camera.h"
using namespace std;

typedef enum { CUBE, PYRAMID } PrimitiveModelType;

class Model {
public:
	virtual ~Model() {}
	void virtual draw(const Renderer& rend)=0;
};


class Light {

};


class Scene {

	vector<Model*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *m_renderer;

public:
	Scene() {};
	Scene(Renderer *renderer) : m_renderer(renderer) {};
	void loadOBJModel(string fileName);
	void draw();
	void drawDemo();
	void TranslateModel();
	void addCamera(CameraType type);
	void loadPrimMeshModel(PrimitiveModelType type);
	
	int activeModel;
	int activeLight;
	int activeCamera;
};