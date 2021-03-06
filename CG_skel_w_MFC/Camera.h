#include "mat.h"
#include "vec.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef enum {ORTHOGONAL, PERSPECTIVE} CameraType;
typedef enum { LEFT, RIGHT, BOTTOM, TOP, zNEAR, zFAR } CameraParams;

class Camera {
	
protected:
	mat4 cTransform; // to camera frame
	mat4 cNormalize;
	mat4 projection; 
	bool renderIt;

	void setParamsAux(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);

public:
	Camera() : renderIt(false) {};
	virtual ~Camera() {}
	map<CameraParams, float> cParams;
	vec4 cameraPosition;
	vec4 up;

	void addTransformation(const mat4& transform);
	void addInversedTransformation(const mat4& inversed);
	
	void LookAt(const vec4& eye, const vec4& at, const vec4& up);
	
	void virtual setCameraParams(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar) = 0;

	void virtual perspective(const float fovy, const float aspect,
		const float zNear, const float zFar) = 0;

	void reshape(float wRatio, float hRatio);
	void switchRenderIt();
	bool getRenderIt();


	mat4 getCameraTransform() {
		return cTransform;
	}

	mat4 getProjection() {
		return projection;
	}

};

class OrthoCamera : public Camera {
public:

	void setCameraParams(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar); // ortho

	void virtual perspective(const float fovy, const float aspect,
		const float zNear, const float zFar) {}

};

class PerspectiveCamera : public Camera {
public:

	void setCameraParams(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar); // Frustum

	void perspective(const float fovy, const float aspect,
		const float zNear, const float zFar);

};
#pragma once
