#include "StdAfx.h"
#include "Camera.h"
#include "mat.h"


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

void Camera::LookAt(const vec4& eye, const vec4& at, const vec4& up)
{	
	vec4 zero = vec4(0.0000001);
	if ((eye - at < zero) || (up < zero))
	{
		cout << "LookAt : Invalid vector" << endl;
		return;
	}
	
	this->cameraPosition = eye;
	this->up = up;
	vec4 n = normalize(eye - at);
	vec4 u = normalize(cross(up, n));
	vec4 v = normalize(cross(n, u));
	vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
	mat4 c = mat4(u, v, n, t);
	cTransform = c * Translate(-eye);

}


void Camera::reshape(float wRatio, float hRatio)
{
	float left = cParams[LEFT] * wRatio;
	float right = cParams[RIGHT] * wRatio;
	float bottom = cParams[BOTTOM] * hRatio;
	float top = cParams[TOP] * hRatio;
	setCameraParams(left, right, bottom, top, cParams[zNEAR], cParams[zFAR]);
}

void Camera::switchRenderIt()
{
	renderIt = !renderIt;
}

bool Camera::getRenderIt()
{
	return renderIt;
}

void Camera::addTransformation(const mat4& transform)
{
mat4 inversed;
	if (inverse(transform, inversed))
		cTransform = inversed * cTransform;
	else
		cout << "Camera::addTransformation error: matrix is non invertable" << endl;
}

void Camera::addInversedTransformation(const mat4 & inversed)
{
	cTransform = inversed * cTransform;
}

// ORTHO
void OrthoCamera::setCameraParams(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) 
{
	setParamsAux(left, right, bottom, top, zNear, zFar);
	vec3 scaleVec = vec3(2 / (cParams[RIGHT] - cParams[LEFT]), 2 / (cParams[TOP] - cParams[BOTTOM]), 2/(cParams[zNEAR] - cParams[zFAR]));
	vec3 translateVec = vec3(-(cParams[LEFT] + cParams[RIGHT]) / 2, -(cParams[BOTTOM] + cParams[TOP]) / 2, (cParams[zNEAR] + cParams[zFAR]) / 2);
	projection = mat4();
	projection[2][2] = 0;
	projection = projection * Scale(scaleVec);
	projection = projection * Translate(translateVec);
}

// FRUSTUM
void PerspectiveCamera::setCameraParams(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) 
{
	setParamsAux(left, right, bottom, top, zNear, zFar);
	
	float alpha = -(cParams[zNEAR] + cParams[zFAR]) / (cParams[zNEAR] - cParams[zFAR]);
	float betha = -2*(cParams[zNEAR] * cParams[zFAR]) / (cParams[zNEAR] - cParams[zFAR]);
	
	projection = mat4();
	projection[0][0] = -2 * cParams[zNEAR] / (cParams[RIGHT] - cParams[LEFT]);
	projection[1][1] = -2 * cParams[zNEAR] / (cParams[TOP] - cParams[BOTTOM]);
	projection[2][2] = alpha;
	projection[3][3] = 0;
	projection[0][2] = (cParams[LEFT] + cParams[RIGHT]) / (cParams[RIGHT] - cParams[LEFT]);
	projection[1][2] = (cParams[TOP] + cParams[BOTTOM]) / (cParams[TOP] - cParams[BOTTOM]);
	projection[2][3] = betha;
	projection[3][2] = -1;
} 

void PerspectiveCamera::perspective(const float fovy, const float aspect,
	const float zNear, const float zFar) 
{
	{
		float top = tan((fovy / 180) * M_PI)*zNear;
		float bottom = -top;
		float right = aspect*top;
		float left = -right;
		setCameraParams(left, right, bottom, top, zNear, zFar);
	}
}
										  