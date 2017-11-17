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
	Scene(Renderer *renderer, int width = 512, int height = 512) : m_renderer(renderer), sceneWidth(width), sceneHeight(height), activeModel(-1), activeLight(-1), activeCamera(-1)  {};

	int activeModel;
	int activeLight;
	int activeCamera;	

	void loadOBJModel(string fileName);
	void draw();
	void drawDemo();
	void addCamera(CameraType type, float left = -1.0, float right = 1.0, float bottom = -1.0, float top = 1.0, float zNear = -1.0, float zFar = 1.0);
	void addFovyAspectCamera(float fovy = 30.0, float aspect = 1.0, float zNear = 1.0, float zFar = 100.0);
	void setCameraParams(float left, float right, float bottom, float top, float zNear, float zFar);
	void setCameraFovyAspect(float fovy, float aspect, float zNear, float zFar);
	void cameraTranslate(float x, float y, float z);
	void addPrimitive(PrimitiveModelType type, float size = 1.0);

	void scaleModel(float x, float y, float z); // scale active model
	void scaleWorld(float x, float y, float z);

	void translateModel(float x, float y, float z);
	void translateWorld(float x, float y, float z);
	
	void rotateModelXaxis(float angle);
	void rotateModelYaxis(float angle);
	void rotateModelZaxis(float angle);

	void rotateWorldXaxis(float angle);
	void rotateWorldYaxis(float angle);
	void rotateWorldZaxis(float angle);


	void switchBoundingBox();
	void switchVertexNormals();
	void switchfaceNormals();

	void setActiveModel(int modelIdx);
	void reshape(int width, int height);
};
