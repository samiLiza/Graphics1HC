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

void Renderer::SetDemoBuffer()
{
	//vertical line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,256,i,0)]=1;	m_outBuffer[INDEX(m_width,256,i,1)]=0;	m_outBuffer[INDEX(m_width,256,i,2)]=0;

	}
	//DrawLine(511, 511, 0, 0);
	//DrawLine(0, 511, 511, 0);
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,i,256,0)]=1;	m_outBuffer[INDEX(m_width,i,256,1)]=0;	m_outBuffer[INDEX(m_width,i,256,2)]=1;

	}

	vector<vec3> vertices;

	vertices.push_back(vec3(200, 200, 0));
	vertices.push_back(vec3(256, 256, 10));
	vertices.push_back(vec3(400, 300, -3));
	DrawTriangles(&vertices);
}

void Renderer::DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals) const 
{
	for (int i = 0; i < vertices->size(); i += 3) {
		vec4 p1 = applyTrasformations(vertices->at(i));
		vec4 p2 = applyTrasformations(vertices->at(i+1));
		vec4 p3 = applyTrasformations(vertices->at(i+2));
		viewPort(p1);
		viewPort(p2);
		viewPort(p3);
		vector<Pixel> pixels;
		DrawLine(p1.x, p1.y, p2.x, p2.y, pixels);
		DrawLine(p1.x, p1.y, p3.x, p3.y, pixels);
		DrawLine(p2.x, p2.y, p3.x, p3.y, pixels);
		setPixels(pixels);



	}

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

void Renderer::DrawLine(int x0, int y0, int x1, int y1, vector<Pixel>& pixels /* Pixels to fill ?*/) const
{
	// assert: y0 > y1
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int deltaX = std::abs(x1 - x0); // What if x0 > x1??
	int deltaY = std::abs(y1 - y0);// What if y0 > y1??
	bool mirrored = false;
	int delta;
	int threshold;
	int threshInc;
	int yInc = ( y0 > y1 ) ? -1 : 1;
	if (deltaY > deltaX)
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		mirrored = true;
		delta = 2 * deltaX; // check again
		threshold = deltaY;
		threshInc = 2 * deltaY;
	}
	else {
		delta = 2 * deltaY; // check again
		threshold = deltaX;
		threshInc = 2 * deltaX;
	}

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
		
		p.setColor(1, 1, 0); // The colors are random

		realY += delta;
		if (realY >= threshold)
		{
			currY+=yInc;
			threshold += threshInc;
		}
		pixels.push_back(p);
	}

}

void Renderer::viewPort(vec4& p) const {
	p.x = (m_width / 2)*(p.x + 1);
	p.y = (m_height / 2)*(p.y + 1);
}

vec4 Renderer::applyTrasformations(const vec3& p) const {
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
		m_outBuffer[INDEX(m_width, p.X(), p.Y(), 0)] = p.red();
		m_outBuffer[INDEX(m_width, p.X(), p.Y(), 1)] = p.green();
		m_outBuffer[INDEX(m_width, p.X(), p.Y(), 2)] = p.blue();
	}
}

void Renderer::SetCameraTransform(const mat4& cTransform) {
	this->cTransform = cTransform;
}

void Renderer::SetProjection(const mat4& projection) {
	this->projection = projection;
}

void Renderer::SetObjectMatrices(const mat4& mTransform, mat4& wTransform, mat3& nTransform) {
	this->mTransform = mTransform;
	this->wTransform = wTransform;
}
