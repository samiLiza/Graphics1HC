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
	m_renderer->ClearColorBuffer();
	//m_renderer->ClearDepthBuffer();
	
	mat4 cameraTransform = cameras[activeCamera]->getCameraTransform();
	mat4 projection = cameras[activeCamera]->getProjection();
	
	// 2. Tell all models to draw themselves
	for (MeshModel* model : models) {
		model->draw(*m_renderer, cameraTransform, projection);
	}

	m_renderer->SwapBuffers();
}

void Scene::drawDemo()
{
	m_renderer->SetDemoBuffer();
	m_renderer->SwapBuffers();
}

void Scene::addCamera(CameraType type, float left, float right, float bottom, float top, float zNear, float zFar)
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

	cam->setCameraParams(left, right, bottom, top, zNear, zFar);
	cam->LookAt(vec4(3, -3, -3, 0), vec4(0, 0, 0, 0), vec4(1, -1, 1, 0));
	cameras.push_back(cam);
	activeCamera = cameras.size() - 1;
}

void Scene::addFovyAspectCamera(float fovy, float aspect, float zNear, float zFar)
{
	PerspectiveCamera cam;
	cam.perspective(fovy, aspect, zNear, zFar);
	cam.LookAt(vec4(0, 0, 0, 0), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0));
	cameras.push_back(&cam);
	activeCamera = cameras.size() - 1;
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
		addCamera(ORTHOGONAL);
		activeCamera = 0;
	}
	activeModel = models.size() - 1;
	draw();
}


void Scene::scaleModel(float x, float y, float z) 
{
	// ToDo: check if x y or z is 0 (less than 0.000001)
	if (activeModel >= 0) {
		mat4 scaleTransform = Scale(x, y, z);
		MeshModel* modelToScale = models[activeModel];
		modelToScale->addModelTransform(scaleTransform);
		if (modelToScale->boundingBox)
			modelToScale->boundingBox->addModelTransform(scaleTransform);
		
		mat4 inverse = Scale(1/x, 1/y, 1/z);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
				inverse[0][1], inverse[1][1], inverse[2][1],
				inverse[0][2], inverse[1][2], inverse[2][2]);
		
		modelToScale->addNormalObjectTransform(normalTransform);

		// ToDo add transform to vertex normals
	}
}

void Scene::translateModel(float x, float y, float z)
{
	if (activeModel >= 0) {
		mat4 translateTransform = Translate(x, y, z);
		MeshModel* modelToTranslate = models[activeModel];
		modelToTranslate->addModelTransform(translateTransform);
		if (modelToTranslate->boundingBox)
			modelToTranslate->boundingBox->addModelTransform(translateTransform);
		
		mat4 inverse = Translate(-x, -y, -z);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);
		
		modelToTranslate->addNormalObjectTransform(normalTransform);

		// ToDo add transform to vertex normals
	}
}

// Rotation!!!

void Scene::rotateModel(float angle)
{
	
}

void Scene::translateWorld(float x, float y, float z)
{
	if (activeModel >= 0) {
		mat4 translateTransform = Translate(x, y, z);
		MeshModel* modelToTranslate = models[activeModel];
		modelToTranslate->addWorldTransform(translateTransform);
		if (modelToTranslate->boundingBox)
			modelToTranslate->boundingBox->addWorldTransform(translateTransform);

		mat4 inverse = Translate(-x, -y, -z);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);
		
		modelToTranslate->addNormalWorldTransform(normalTransform);

		// ToDo add transform to vertex normals
	}
}

void Scene::scaleWorld(float x, float y, float z)
{
	// ToDo: check if x y or z is 0 (less than 0.000001)
	if (activeModel >= 0) {
		mat4 scaleTransform = Scale(x, y, z);
		MeshModel* modelToScale = models[activeModel];
		modelToScale->addWorldTransform(scaleTransform);
		if (modelToScale->boundingBox)
			modelToScale->boundingBox->addWorldTransform(scaleTransform);

		mat4 inverse = Scale(1 / x, 1 / y, 1 / z);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);
		
		modelToScale->addNormalWorldTransform(normalTransform);

		// ToDo add transform to vertex normals
	}
}

void Scene::rotateWorld(float angle)
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
	if (activeModel >= 0) {
		models[activeModel]->switchVertexNormals();
	}
}

void Scene::switchfaceNormals()
{
	if (activeModel >= 0) {
		models[activeModel]->switchFaceNormals();
	}
}



