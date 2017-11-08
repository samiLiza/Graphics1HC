#include "StdAfx.h"
#include "Camera.h"


void Camera::setParamsAux(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) 
{
	cParams.clear();
	cParams[LEFT] = left;
	cParams[RIGHT] = right;
	cParams[BOTTOM] = bottom;
	cParams[TOP] = top;
	cParams[zNEAR] = zNear;
	cParams[zFAR] = zFar;
}


// ORTHO
void OrthoCamera::setCameraParams(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) 
{
	setParamsAux(left, right, bottom, top, zNear, zFar);
	vec3 scaleVec = vec3(2 / (cParams[RIGHT] - cParams[LEFT]), 2 / (cParams[TOP] - cParams[BOTTOM]), 2/(cParams[zNEAR] - cParams[zFAR]));
	vec3 translateVec = vec3(-(cParams[LEFT] + cParams[RIGHT]) / 2, -(cParams[BOTTOM] + cParams[TOP]) / 2, (cParams[zNEAR] + cParams[zFAR]) / 2);
	cNormalize = Scale(scaleVec);
	cNormalize = cNormalize * Translate(translateVec);
}

// FRUSTUM
void PerspectiveCamera::setCameraParams(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) 
{
	setParamsAux(left, right, bottom, top, zNear, zFar);
} 


mat4 PerspectiveCamera::Perspective(const float fovy, const float aspect,
	const float zNear, const float zFar) 
{

}
										  