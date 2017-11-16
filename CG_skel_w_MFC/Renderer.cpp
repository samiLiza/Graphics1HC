#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "assert.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

Renderer::Renderer() :m_width(512), m_height(512)
{
	InitOpenGLRendering();
	CreateBuffers(512,512);
}
Renderer::Renderer(int width, int height) :m_width(width), m_height(height)
{
	InitOpenGLRendering();
	CreateBuffers(width,height);
}

Renderer::~Renderer(void)
{
}



void Renderer::CreateBuffers(int width, int height)
{
	m_width=width;
	m_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
}

void Renderer::ClearColorBuffer() {
	for (int i = 0; i < 3 * m_width*m_height; i++) {
		m_outBuffer[i] = 0;
	}
}

void Renderer::ClearDepthBuffer() {
	for (int i = 0; i < m_width*m_height; i++) {
		m_zbuffer[i] = 0;
	}
}

void Renderer::SetDemoBuffer()
{
	//vertical line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,256,i,0)]=1;	m_outBuffer[INDEX(m_width,256,i,1)]=0;	m_outBuffer[INDEX(m_width,256,i,2)]=0;

	}
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,i,256,0)]=1;	m_outBuffer[INDEX(m_width,i,256,1)]=0;	m_outBuffer[INDEX(m_width,i,256,2)]=1;

	}

}

// get rip of bool value, check if normals NULL instead
// add normals for cube primitive
void Renderer::DrawTriangles(const vector<vec3>* vertices, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection, bool showFaceNormals, const mat3& nTransform, const vector<vec3>* normals) const
{
	for (int i = 0; i < vertices->size(); i += 3) {
		vec4 p1 = applyTrasformations(vertices->at(i), mTransform, wTransform, cTransform, projection);
		vec4 p2 = applyTrasformations(vertices->at(i+1), mTransform, wTransform, cTransform, projection);
		vec4 p3 = applyTrasformations(vertices->at(i+2), mTransform, wTransform, cTransform, projection);
		
		vector<Pixel> pixels;
		if (showFaceNormals) {
			vec3 mc = (vertices->at(i) + vertices->at(i + 1) + vertices->at(i + 2)) / 3;
			vec4 mcTransformed = applyTrasformations(mc, mTransform, wTransform, cTransform, projection);
			vec4 normal = vec4(normalize(nTransform * (*normals)[i / 3]));
			normal *= 15;
			normal.w = 0;
			viewPort(mcTransformed);
			viewPort(mcTransformed + normal);
			DrawLine(mcTransformed.x, mcTransformed.y, mcTransformed.x + normal.x, mcTransformed.y + normal.y, pixels, 1, 0, 0);
		}
		viewPort(p1);
		viewPort(p2);
		viewPort(p3);
		
		
		DrawLine(p1.x, p1.y, p2.x, p2.y, pixels, 1, 0, 1);
		DrawLine(p1.x, p1.y, p3.x, p3.y, pixels, 0, 0, 1);
		DrawLine(p2.x, p2.y, p3.x, p3.y, pixels, 0, 1, 0);

		
		setPixels(pixels);
	}

}

/*
void Renderer::DrawNormals(const vector<vec3>* normals, const mat3 & normalModelTransform, const mat3 & normalWorldTransform, const mat4 & cTransform, const mat4 & projection) const
{
	
	for (int i = 0; i < normals->size(); i += 2) {
		vec3 p1 = applyNormalTrasformations(normals->at(i), normalModelTransform, normalWorldTransform, cTransform, projection);
		vec3 p2 = applyNormalTrasformations(normals->at(i + 1), normalModelTransform, normalWorldTransform, cTransform, projection);
		viewPort(p1);
		viewPort(p2);
		vector<Pixel> pixels;
		DrawLine(p1.x, p1.y, p1.x + p2.x, p1.y + p2.y, pixels);
		setPixels(pixels);

	}
	
}

*/
/*
vec3 Renderer::applyNormalTrasformations(const vec3 & from, const vec3 & normal, const mat3 & mTransform, const mat3 & wTransform, const mat4 & cTransform, const mat4 & projection) const
{
	vec3 vertex(p);
	mat3 cTransformReduced = mat3(cTransform[0][0], cTransform[1][0], cTransform[2][0], cTransform[0][1], cTransform[1][1], cTransform[2][1], cTransform[0][2], cTransform[1][2], cTransform[2][2]);
	mat3 projectionReduced = mat3(projection[0][0], projection[1][0], projection[2][0], projection[0][1], projection[1][1], projection[2][1], projection[0][2], projection[1][2], projection[2][2]);
	vertex = mTransform * vertex; //model
	vertex = wTransform * vertex; //world
	vertex = cTransformReduced * vertex; //camera
	vertex = projectionReduced * vertex; //camera
	
	return vertex;
}
*/
// ONLY FOR CUBE!!!
void Renderer::DrawCube(const vector<vec3>* vertices, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection, const vector<vec3>* normals) const
{
	if (!vertices) {
		cout << "DrawCube error: vertices is invalid" << endl;
	}
	vector<vec4> tempVertices;
	vector<Pixel> pixels;
	//for (vec3 vertex : *vertices)
	for(int i = 0; i < vertices->size(); i++)
	{
		vec4 p = applyTrasformations((*vertices)[i], mTransform, wTransform, cTransform, projection);
		viewPort(p);
		tempVertices.push_back(p);
	}

	DrawLine(tempVertices[0].x, tempVertices[0].y, tempVertices[1].x, tempVertices[1].y, pixels);
	DrawLine(tempVertices[0].x, tempVertices[0].y, tempVertices[2].x, tempVertices[2].y, pixels);
	DrawLine(tempVertices[3].x, tempVertices[3].y, tempVertices[1].x, tempVertices[1].y, pixels); 
	DrawLine(tempVertices[3].x, tempVertices[3].y, tempVertices[2].x, tempVertices[2].y, pixels);

	DrawLine(tempVertices[4].x, tempVertices[4].y, tempVertices[5].x, tempVertices[5].y, pixels); // red
	DrawLine(tempVertices[4].x, tempVertices[4].y, tempVertices[6].x, tempVertices[6].y, pixels); // green
	DrawLine(tempVertices[7].x, tempVertices[7].y, tempVertices[5].x, tempVertices[5].y, pixels); // blue
	DrawLine(tempVertices[7].x, tempVertices[7].y, tempVertices[6].x, tempVertices[6].y, pixels);

	DrawLine(tempVertices[0].x, tempVertices[0].y, tempVertices[4].x, tempVertices[4].y, pixels);
	DrawLine(tempVertices[1].x, tempVertices[1].y, tempVertices[5].x, tempVertices[5].y, pixels);
	DrawLine(tempVertices[2].x, tempVertices[2].y, tempVertices[6].x, tempVertices[6].y, pixels);
	DrawLine(tempVertices[3].x, tempVertices[3].y, tempVertices[7].x, tempVertices[7].y, pixels);

	setPixels(pixels);
}

//void Renderer::setCamera(Camera* camera) 
//{
	//assert(camera);
	//this->activeCamera = camera;
//}





/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

void Renderer::InitOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &gScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &gScreenVtc);
	GLuint buffer;
	glBindVertexArray(gScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[]={
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0 );

	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *) sizeof(vtc) );
	glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
	a = glGetError();
}

void Renderer::CreateOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, m_width, m_height);
}

void Renderer::SwapBuffers()
{

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}


void Renderer::DrawVerticalLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/, int red, int green, int blue) const
{
	assert(x0 == x1);
	int currX = x0;
	Pixel p;
	if (y1 < y0)
		std::swap(y0, y1);
	for (int currY = y0; currY <= y1; currY++)
	{	
		p.setCordinates(currX, currY);
		p.setColor(red, green, blue);
		pixels.push_back(p);
	}
}

void Renderer::DrawSlopeLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/, int red, int green, int blue) const
{
	// assert: y0 > y1
	
	int deltaX = std::abs(x1 - x0); // What if x0 > x1??
	int deltaY = std::abs(y1 - y0);// What if y0 > y1??
	bool mirrored = false;
	int delta;
	int threshold;
	int threshInc;
	if (deltaY > deltaX)
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		mirrored = true;
		delta = 2 * deltaX; // check again
		threshold = deltaY;
		threshInc = 2 * deltaY;
	}
	else 
	{
		delta = 2 * deltaY; // check again
		threshold = deltaX;
		threshInc = 2 * deltaX;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);

	}
	int yInc = (y0 > y1) ? -1 : 1;
	int realY = 0;
	int currY = y0;
	Pixel p;
	for (int currX = x0; currX <= x1; currX++)
	{
		// do smth with pixel
		if (mirrored) 
		{
			//m_outBuffer[INDEX(m_width, currY, currX, 0)] = 1;	m_outBuffer[INDEX(m_width, currY, currX, 1)] = 1;	m_outBuffer[INDEX(m_width, currY, currX, 2)] = 0;
			p.setCordinates(currY, currX);
		} 
		else 
		{
			//m_outBuffer[INDEX(m_width, currX, currY, 0)] = 1;	m_outBuffer[INDEX(m_width, currX, currY, 1)] = 1;	m_outBuffer[INDEX(m_width, currX, currY, 2)] = 0;
			p.setCordinates(currX, currY);
		}
		
		p.setColor(red, green, blue); // The colors are random

		realY += delta;
		if (realY >= threshold)
		{
			currY+=yInc;
			threshold += threshInc;
		}
		pixels.push_back(p);
	}
}


void Renderer::DrawLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/, int red, int green, int blue) const
{
	if (x0 == x1)
		DrawVerticalLine(x0, y0, x1, y1, pixels, red, green, blue);
	else
		DrawSlopeLine(x0, y0, x1, y1, pixels, red, green, blue);
}

void Renderer::viewPort(vec4& p) const {
	p.x = (m_width / 2)*(p.x + 1);
	p.y = (m_height / 2)*(p.y + 1);
}
/*
void Renderer::viewPort(vec3& p) const {
	p.x = (m_width / 2)*(p.x + 1);
	p.y = (m_height / 2)*(p.y + 1);
}
*/
vec4 Renderer::applyTrasformations(const vec3& p, const mat4& mTransform, const mat4& wTransform, const mat4& cTransform, const mat4& projection) const {
	vec4 homog(p);
	homog = mTransform*homog; //model
	homog = wTransform*homog; //world
	homog = cTransform*homog; //camera
	homog = projection*homog; //camera
	if(homog.w != 0)
		homog /= homog.w;

	return homog;

}

void Renderer::setPixels(vector<Pixel> pixels) const {
	for (Pixel p : pixels) {
		if (p.X() < 0 || p.X() > (m_width - 1) || p.Y() < 0 || p.Y() > (m_height - 1)) {
			continue;
		}
		m_outBuffer[INDEX(m_width, p.X(), p.Y(), 0)] = p.red();
		m_outBuffer[INDEX(m_width, p.X(), p.Y(), 1)] = p.green();
		m_outBuffer[INDEX(m_width, p.X(), p.Y(), 2)] = p.blue();
	}
}
