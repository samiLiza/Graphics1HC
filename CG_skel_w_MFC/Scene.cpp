#include "stdafx.h"
#include "Scene.h"
#include <string>

using namespace std;
void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	if (cameras.empty())
		addCamera(PERSPECTIVE);

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
		if(models[activeModel] == model)
			model->draw(*m_renderer, cameraTransform, projection, true);
		else 
			model->draw(*m_renderer, cameraTransform, projection, false);
	}

	for (Camera* camera : cameras) 
	{
		if (camera->getRenderIt() && (camera != cameras[activeCamera]) )
			m_renderer->DrawCamera(camera->cameraPosition, camera->getCameraTransform(), cameraTransform, projection);
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
	cam->LookAt(vec4(0, 0, 10, 0), vec4(0, 0, 0, 0), vec4(0, 1, 0, 0));
	//cam->LookAt(vec4(10, 0, 2, 0), vec4(0, 0, 0, 0), vec4(0, 1, 0, 0));
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

void Scene::setCameraParams(float left, float right, float bottom, float top, float zNear, float zFar)
{
	cameras[activeCamera]->setCameraParams(left, right, bottom, top, zNear, zFar);
}

void Scene::setCameraFovyAspect(float fovy, float aspect, float zNear, float zFar)
{
	cameras[activeCamera]->perspective(fovy, aspect, zNear, zFar);
}

void Scene::cameraTranslate(float x, float y, float z)
{
	if (cameras.empty()) {
		AfxMessageBox(_T("Please add a camera"));
		return;
	}

	Camera* cam = cameras[activeCamera];
	mat4 inversedTranslate = Translate(-x, -y, -z);
	cam->addInversedTransformation(inversedTranslate);
}

void Scene::cameraRotate(float angleX, float angleY, float angleZ)
{
	if (cameras.empty()) {
		AfxMessageBox(_T("Please add a camera"));
		return;
	}
	Camera* cam = cameras[activeCamera];
	vec4 homoPosition = cam->cameraPosition;
	homoPosition.x = homoPosition.w == 0 ? homoPosition.x : homoPosition.x /= homoPosition.w;
	homoPosition.y = homoPosition.w == 0 ? homoPosition.y : homoPosition.y /= homoPosition.w;
	homoPosition.z = homoPosition.w == 0 ? homoPosition.z : homoPosition.z /= homoPosition.w;
	vec3 position(homoPosition.x, homoPosition.y, homoPosition.z);

	mat4 rotateTransformX = Translate(position) * RotateX(-angleX) * Translate(-position);
	cam->addInversedTransformation(rotateTransformX);

	mat4 rotateTransformY = Translate(position) * RotateY(-angleY) * Translate(-position);
	cam->addInversedTransformation(rotateTransformY);

	mat4 rotateTransformZ = Translate(position) * RotateZ(-angleZ) * Translate(-position);
	cam->addInversedTransformation(rotateTransformZ);
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

void Scene::reshape(int width, int height)
{
	delete[] m_renderer->m_outBuffer;
	m_renderer->CreateBuffers(width, height);
	float wRatio = (sceneWidth ==0) ? width : width / (float)sceneWidth;
	float hRatio = (sceneHeight == 0) ? height : height / (float)sceneHeight;
	sceneWidth = width;
	sceneHeight = height;
	for (Camera* cam : cameras) {
		cam->reshape(wRatio, hRatio);
	}
}

void Scene::addPrimitive(PrimitiveModelType type, float size)
{
	// fix if new primitives added
	MeshModel* primModel = new PrimitiveCube(size);
	models.push_back(primModel);

	if (cameras.empty()) {
		addCamera(PERSPECTIVE);
		activeCamera = 0;
	}
	activeModel = models.size() - 1;
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


void Scene::translateModel(float x, float y, float z)
{
	if (activeModel >= 0) {
		mat4 translateTransform = Translate(x, y, z);
		MeshModel* modelToTranslate = models[activeModel];
		modelToTranslate->addModelTransform(translateTransform);
		modelToTranslate->setModelFramePosition(vec3(x, y, z));
		
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

void Scene::translateWorld(float x, float y, float z)
{
	if (activeModel >= 0) {
		mat4 translateTransform = Translate(x, y, z);
		MeshModel* modelToTranslate = models[activeModel];
		modelToTranslate->addWorldTransform(translateTransform);
		modelToTranslate->setWorldFramePosition(vec3(x, y, z));
		
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

void Scene::addModelSteps(int xr, int yr)
{
	MeshModel* model = models[activeModel];
	model->addSteps(xr, yr);
}

// Rotation!!!


void Scene::rotateModelXaxis(float angle)
{
	if (activeModel >= 0)
	{
		MeshModel* modelToRotate = models[activeModel];
		vec3 position = modelToRotate->getModelFramePosition();
		mat4 rotateTransform = Translate(position) * RotateX(angle) * Translate(-position);

		modelToRotate->addModelTransform(rotateTransform);
		if (modelToRotate->boundingBox)
			modelToRotate->boundingBox->addModelTransform(rotateTransform);
		mat4 inverse = Translate(position) * RotateX(-angle) * Translate(-position);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);

		modelToRotate->addNormalObjectTransform(normalTransform);
		// ToDo add transform to vertex normals
	}
	
}

void Scene::rotateModelYaxis(float angle)
{
	if (activeModel >= 0)
	{
		MeshModel* modelToRotate = models[activeModel];
		vec3 position = modelToRotate->getModelFramePosition();
		mat4 rotateTransform = Translate(position) * RotateY(angle) * Translate(-position);

		modelToRotate->addModelTransform(rotateTransform);
		if (modelToRotate->boundingBox)
			modelToRotate->boundingBox->addModelTransform(rotateTransform);
		mat4 inverse = Translate(position) * RotateY(-angle) * Translate(-position);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);

		modelToRotate->addNormalObjectTransform(normalTransform);
		// ToDo add transform to vertex normals
	}
}

void Scene::rotateModelZaxis(float angle)
{
	if (activeModel >= 0)
	{
		MeshModel* modelToRotate = models[activeModel];
		vec3 position = modelToRotate->getModelFramePosition();
		mat4 rotateTransform = Translate(position) * RotateZ(angle) * Translate(-position);

		modelToRotate->addModelTransform(rotateTransform);
		modelToRotate->addModelTransform(rotateTransform);
		if (modelToRotate->boundingBox)
			modelToRotate->boundingBox->addModelTransform(rotateTransform);
		mat4 inverse = Translate(position) * RotateZ(-angle) * Translate(-position);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);

		modelToRotate->addNormalObjectTransform(normalTransform);
		// ToDo add transform to vertex normals
	}
}



void Scene::rotateWorldXaxis(float angle)
{
	if (activeModel >= 0)
	{
		MeshModel* modelToRotate = models[activeModel];
		vec3 position = modelToRotate->getWorldFramePosition();
		mat4 rotateTransform = Translate(position) * RotateX(angle) * Translate(-position);

		modelToRotate->addWorldTransform(rotateTransform);
		if (modelToRotate->boundingBox)
			modelToRotate->boundingBox->addWorldTransform(rotateTransform);
		mat4 inverse = Translate(position) * RotateX(-angle) * Translate(-position);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);

		modelToRotate->addNormalWorldTransform(normalTransform);
		// ToDo add transform to vertex normals
	}
}

void Scene::rotateWorldYaxis(float angle)
{
	if (activeModel >= 0)
	{
		MeshModel* modelToRotate = models[activeModel];
		vec3 position = modelToRotate->getWorldFramePosition();
		mat4 rotateTransform = Translate(position) * RotateY(angle) * Translate(-position);

		modelToRotate->addWorldTransform(rotateTransform);
		if (modelToRotate->boundingBox)
			modelToRotate->boundingBox->addWorldTransform(rotateTransform);
		mat4 inverse = Translate(position) * RotateY(-angle) * Translate(-position);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);

		modelToRotate->addNormalWorldTransform(normalTransform);
			// ToDo add transform to vertex normals
	}
}

void Scene::rotateWorldZaxis(float angle)
{
	if (activeModel >= 0)
	{
		MeshModel* modelToRotate = models[activeModel];
		vec3 position = modelToRotate->getWorldFramePosition();
		mat4 rotateTransform = Translate(position) * RotateZ(angle) * Translate(-position);

		modelToRotate->addWorldTransform(rotateTransform);
		if (modelToRotate->boundingBox)
			modelToRotate->boundingBox->addWorldTransform(rotateTransform);
		mat4 inverse = Translate(position) * RotateZ(-angle) * Translate(-position);
		inverse = transpose(inverse);
		mat3 normalTransform(inverse[0][0], inverse[1][0], inverse[2][0],
			inverse[0][1], inverse[1][1], inverse[2][1],
			inverse[0][2], inverse[1][2], inverse[2][2]);

		modelToRotate->addNormalWorldTransform(normalTransform);
			// ToDo add transform to vertex normals
	}
}



void Scene::zoom(float step)
{
	// step <= 1 - zoomIn
	// step > 1 - zoomOut
	if (activeCamera >= 0) 
	{
		Camera* activeCam = cameras[activeCamera];
		activeCam->setCameraParams(step * activeCam->cParams[LEFT], step * activeCam->cParams[RIGHT], step * activeCam->cParams[BOTTOM], step * activeCam->cParams[TOP], activeCam->cParams[zNEAR], activeCam->cParams[zFAR]);
	}
}

void Scene::renderCamera()
{
	if (activeCamera >= 0)
		cameras[activeCamera]->switchRenderIt();
}

void Scene::lookAtActiveModel()
{
	if ((activeCamera >= 0) && (activeModel >= 0))
	{
		for (MeshModel* model : models) {
			if (model != models[activeModel]) {
				model->addSteps(-models[activeModel]->steps.first, -models[activeModel]->steps.second);
			}
		}
		models[activeModel]->clearSteps();
		Camera* camera = cameras[activeCamera];
		vec4 eye = camera->cameraPosition;
		vec4 up = camera->up;
		vec4 at = vec4((models[activeModel])->getModelFramePosition() + (models[activeModel])->getWorldFramePosition());
		camera->LookAt(eye, at, up);
	}
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



