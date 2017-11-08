#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;
void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
}

void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	
	
	// 2. Tell all models to draw themselves


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
		cam->setCameraParams(-1, 1, -1, 1, -1, 1);
	}
	else
	{
		cam = new PerspectiveCamera();
	}
	cameras.push_back(cam);

}
