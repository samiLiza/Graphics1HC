#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Camera.h"
#include "Pixel.h"
#include <vector>


using namespace std;
class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;

	/*
	mat4 mTransform;
	mat4 wTransform;
	mat4 cTransform;
	mat4 projection;
	*/
	void CreateBuffers(int width, int height);
	void CreateLocalBuffer();

	//////////////////////////////
	// openGL stuff. Don't touch.

	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////
public:
	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	void Init() {
		ClearColorBuffer();
		ClearDepthBuffer();
	}
	//void DrawTriangles(const vector<vec3>* vertices, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection, const vector<vec3>* normals=NULL) const;
	void DrawTriangles(const vector<vec3>* vertices, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection, bool showFaceNormals, const mat3& nTransform, const vector<vec3>* vertexNormals = NULL, const vector<vec3>* faceNormals = NULL) const;
	
	void SwapBuffers();
	void ClearColorBuffer();
	void ClearDepthBuffer();
	void SetDemoBuffer();

	void DrawLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/, int red = 1, int green = 1, int blue = 0) const;
	void DrawVerticalLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/, int red = 1, int green = 1, int blue = 0) const;
	void DrawSlopeLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/, int red = 1, int green = 1, int blue = 0) const;

	void DrawNormals(const vector<vec3>* normals, const mat3& normalModelTransform, const mat3& normalWorldTransform, const mat4& cTransform, const mat4& projection) const;
	vec3 applyNormalTrasformations(const vec3& p, const mat3& mTransform, const mat3& wTransform, const mat4& cTransform, const mat4& projection) const;
	
	void DrawCube(const vector<vec3>* vertices, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection, const vector<vec3>* normals = NULL) const;
	//void setCamera(Camera* camera);
	void viewPort(vec4& p) const;
	void viewPort(vec3& p) const;
	vec4 applyTrasformations(const vec3& p, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection) const;
	void setPixels(vector<Pixel> pixels) const;

};
