#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

MeshModel::MeshModel(string fileName) : boundingBox(NULL), showBox(false)
{
	loadFile(fileName);
}

MeshModel::~MeshModel(void)
{
}

void MeshModel::loadFile(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> faces;
	vector<vec3> vertices;
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") /*BUG?*/
			vertices.push_back(vec3fFromStream(issLine));
		else if (lineType == "f") /*BUG?*/
			faces.push_back(issLine);
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			cout<< "Found unknown line Type \"" << lineType << "\"";
		}
	}
	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertex_positions should contain:
	//vertex_positions={v1,v2,v3,v1,v3,v4}
	int n = faces.size();
	//vertex_positions = new vec3[n]; /*BUG?*/
	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < 3; i++)
		{
			vertex_positions.push_back(vertices[it->v[i] - 1]); /*BUG?*/
		}
	}
}

void MeshModel::setShowBox()
{
	showBox = true;
}
void MeshModel::unsetShowBox()
{
	showBox = false;
}


void MeshModel::initBoundingBox()
{
	if(boundingBox){
		cout << "initBoundingBox: bounding box already exists! Check memory leaks" << endl;
		return;
	}
	vector<vec3> vertices;
	float xMax = getXmax();
	float xMin = getXmin();

	float yMax = getYmax();
	float yMin = getYmin();

	float zMax = getZmax();
	float zMin = getZmin();

	vertices.push_back(vec3(xMax, yMax, zMax));
	vertices.push_back(vec3(xMax, yMax, zMin));
	vertices.push_back(vec3(xMin, yMax, zMax));
	vertices.push_back(vec3(xMin, yMax, zMin));

	vertices.push_back(vec3(xMax, yMin, zMax));
	vertices.push_back(vec3(xMax, yMin, zMin));
	vertices.push_back(vec3(xMin, yMin, zMax));
	vertices.push_back(vec3(xMin, yMin, zMin));

	boundingBox = new Cube(&vertices);

	boundingBox->addModelTransform(getModelTransform());
	boundingBox->addWorldTransform(getWorldTransform());
	boundingBox->addNormalTransform(getNormalTransform());
}


void MeshModel::draw(const Renderer& rend, const mat4& cTransform, const mat4& projection)
{
	// ToDo: think about normal
	if (showBox)
	{
		if (!boundingBox) {
			// compute max coordinates of the model to initiate the cube
			// copy all the transformations of the model to the cube
			initBoundingBox();
		}
		rend.DrawCube(&boundingBox->vertex_positions, boundingBox->getModelTransform(), boundingBox->getWorldTransform(), cTransform, projection /*, boundingBox->getNormalTransform()*/);
	}
	rend.DrawTriangles(&vertex_positions, _modelTransform, _worldTransform, cTransform, projection);
}

void MeshModel::addWorldTransform(const mat4& transform) 
{
	_worldTransform = transform * _worldTransform;
}

void MeshModel::addNormalTransform(const mat3& transform)
{
	_normalTransform = transform * _normalTransform;
}


void MeshModel::addModelTransform(const mat4& transform)
{
	_modelTransform = transform * _modelTransform;
}

mat4 MeshModel::getModelTransform() const
{
	return mat4(_modelTransform);
}
mat4 MeshModel::getWorldTransform() const
{
	return mat4(_worldTransform);
}

mat3 MeshModel::getNormalTransform() const
{
	return mat3(_normalTransform);
}

float MeshModel::getXmax() const
{
	float xMax = vertex_positions[0].x;
	for (vec3 vertex : vertex_positions) 
	{
		if (vertex.x > xMax)
			xMax = vertex.x;
	}
	return xMax;
}
float MeshModel::getXmin()const
{
	float xMin = vertex_positions[0].x;
	for (vec3 vertex : vertex_positions)
	{
		if (vertex.x < xMin)
			xMin = vertex.x;
	}
	return xMin;
}

float MeshModel::getYmax()const
{
	float yMax = vertex_positions[0].y;
	for (vec3 vertex : vertex_positions)
	{
		if (vertex.y > yMax)
			yMax = vertex.y;
	}
	return yMax;
}
float MeshModel::getYmin()const
{
	float yMin = vertex_positions[0].y;
	for (vec3 vertex : vertex_positions)
	{
		if (vertex.y < yMin)
			yMin = vertex.y;
	}
	return yMin;
}

float MeshModel::getZmax()const
{
	float zMax = vertex_positions[0].z;
	for (vec3 vertex : vertex_positions)
	{
		if (vertex.z > zMax)
			zMax = vertex.z;
	}
	return zMax;
}
float MeshModel::getZmin()const
{
	float zMin = vertex_positions[0].z;
	for (vec3 vertex : vertex_positions)
	{
		if (vertex.z < zMin)
			zMin = vertex.z;
	}
	return zMin;
}


