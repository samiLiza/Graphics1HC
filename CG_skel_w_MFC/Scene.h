#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Camera.h"
#include "MeshModel.h"
#include "PrimMeshModel.h"
using namespace std;


class Light {

};


class Scene {

	vector<MeshModel*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *m_renderer;

	int sceneWidth;
	int sceneHeight;

public:
	Scene() {};
	Scene(Renderer *renderer, int width = 512, int height = 512) : m_renderer(renderer), sceneWidth(width), sceneHeight(height) {};
	void loadOBJModel(string fileName);
	void draw();
	void drawDemo();
	void addCamera(CameraType type);
	void addPrimitive(PrimitiveModelType type, float size = 1.0);
	
	void scaleModel(float x, float y, float z); // scale active model
	void translateModel(float x, float y, float z);
	void rotateModel(float angle);

	int activeModel;
	int activeLight;
	int activeCamera;

	void switchBoundingBox();
	void switchVertexNormals();
	void switchfaceNormals();

	void setActiveModel(int modelIdx);
};