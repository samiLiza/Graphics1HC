#include "stdafx.h"
#include "Scene.h"
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
	
	/* ToDo Review the 2 lines*/
	//m_renderer->ClearColorBuffer();
	//m_renderer->ClearDepthBuffer();
	
	mat4 cameraTransform = cameras[activeCamera]->getCameraTransform();
	mat4 projection = cameras[activeCamera]->getProjection();
	// 2. Tell all models to draw themselves
	for (MeshModel* model : models) {
		//mat4 modelTransform = model->getModelTransform;
		//mat4 worldTransform = model->getWorldTransform;
		model->draw(*m_renderer, cameraTransform, projection);
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
	float middleW = sceneWidth / 2;
	float middleH = sceneHeight / 2;
	Camera* cam;
	if (type == ORTHOGONAL)
	{
		cam = new OrthoCamera();
	}
	else
	{
		cam = new PerspectiveCamera();
	}

	//cam->setCameraParams(-0.01 * middleW, 0.01 * middleW, -0.01 * middleH, 0.01 * middleH, -10, 10);
	//cam->setCameraParams(-0.01 * middleW, 0.01 * middleW, -0.01 * middleH, 0.01 * middleH, -10, 10);
	cam->setCameraParams(-1 , 1, -1 , 1 , -1, 1);
	cam->LookAt(vec4(-4, 2, 2, 0), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0));
	cameras.push_back(cam);
}

void Scene::setActiveModel(int modelIdx) 
{
	if (modelIdx < 0 || modelIdx >= models.size()) 
	{
		cout << "setActiveModel: invalid index " << modelIdx << endl;
		return;
	}
	activeModel = modelIdx;
}

void Scene::addPrimitive(PrimitiveModelType type, float size)
{
	// fix if new primitives added
	MeshModel* primModel = new Cube(size);
	models.push_back(primModel);
	if (cameras.empty()) {
		addCamera(PERSPECTIVE);
		activeCamera = 0;
	}
	activeModel = models.size() - 1;
	draw();
}


void Scene::scaleModel(float x, float y, float z) 
{
	// ToDo : scaling in world frame
	// ToDo :if showBounding box add transform to the bounding box
	if (activeModel >= 0) {
		mat4 scaleTransform = Scale(x, y, z);
		MeshModel* modelToScale = models[activeModel];
		modelToScale->addModelTransform(scaleTransform);
		if (modelToScale->boundingBox)
			modelToScale->boundingBox->addModelTransform(scaleTransform);
	}
}

void Scene::translateModel(float x, float y, float z)
{
	// ToDo :if showBounding box add transform to the bounding box
	if (activeModel >= 0) {
		mat4 translateTransform = Translate(x, y, z);
		MeshModel* modelToTranslate = models[activeModel];
		modelToTranslate->addModelTransform(translateTransform);
		if (modelToTranslate->boundingBox)
			modelToTranslate->boundingBox->addModelTransform(translateTransform);
	}
}

// Rotation!!!

void Scene::rotateModel(float angle)
{
	
}

void Scene::switchBoundingBox() 
{
	if (activeModel >= 0) {
		models[activeModel]->switchBoundingBox();
	}
}

void Scene::switchVertexNormals()
{
	//TODO: add functionality
}

void Scene::switchfaceNormals()
{
	//TODO: add functionality
}



