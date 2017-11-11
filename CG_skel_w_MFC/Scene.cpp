#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;
void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	if (cameras.empty()) {
		addCamera(PERSPECTIVE);
		activeCamera = 0;
	}
	activeModel = models.size() - 1;
	draw();
}

void Scene::draw()
{
	if (cameras.empty()) {
		return;
	}
	// 1. Send the renderer the current camera transform and the projection
	m_renderer->SetCameraTransform(cameras[activeCamera]->getCameraTransform());
	m_renderer->SetProjection(cameras[activeCamera]->getProjection());
	
	// 2. Tell all models to draw themselves
	for (Model* model : models) {
		model->draw(*m_renderer);
	}


	m_renderer->SwapBuffers();
}

void Scene::drawDemo()
{
	m_renderer->SetDemoBuffer();
	m_renderer->SwapBuffers();
}

void Scene::addCamera(CameraType type)
{
	Camera* cam;
	if (type == ORTHOGONAL)
	{
		cam = new OrthoCamera();
	}
	else
	{
		cam = new PerspectiveCamera();
	}

	cam->setCameraParams(-1, 1, -1, 1, -1, 1);
	cam->LookAt(vec4(0, 0, 0, 0), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0));
	cameras.push_back(cam);
}

