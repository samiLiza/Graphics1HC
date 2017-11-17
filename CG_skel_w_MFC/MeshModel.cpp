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

void MeshModel::loadFile(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> faces;
	vector<vec3> vertices;
	vector<vec3> vertexNormals;
	vec3 massCenter;
	vec3 normal;
	vector<vec3> triangle;
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
		else if (lineType == "vn")
			vertexNormals.push_back(normalize(vec3fFromStream(issLine)));
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
		triangle.clear();
		for (int i = 0; i < 3; i++)
		{
			vertex_positions.push_back(vertices[it->v[i] - 1]); /*BUG?*/
			triangle.push_back(vertices[it->v[i] - 1]); // triangle vertices
			if (!(vertexNormals.empty())) {
				vertexNormalPositions.push_back(vertices[it->v[i] - 1]);
				vertexNormalPositions.push_back(vertexNormals[it->vn[i] - 1]);
			}
		}
		normal = cross(triangle[0] - triangle[2], triangle[1] - triangle[2]);
		normal = normalize(normal);
		normalFaces.push_back(normal);
	}
}

void MeshModel::switchBoundingBox()
{
	showBox = !showBox;
}

void MeshModel::switchFaceNormals()
{
	showFaceNormals = !showFaceNormals;
}

void MeshModel::switchVertexNormals()
{
	showVertexNormals = !showVertexNormals;
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
	boundingBox->addNormalObjectTransform(getNormalObjectTransform());
	boundingBox->addNormalWorldTransform(getNormalWorldTransform());

}


void MeshModel::draw(const Renderer& rend, const mat4& cTransform, const mat4& projection)
{
	// ToDo: think about normal
	mat3 normalTransform;
	if (showBox)
	{
		if (!boundingBox) {
			// compute max coordinates of the model to initiate the cube
			// copy all the transformations of the model to the cube
			initBoundingBox();
		}
		rend.DrawCube(&boundingBox->vertex_positions, boundingBox->getModelTransform(), boundingBox->getWorldTransform(), cTransform, projection, NULL, steps);
	}
	if (showFaceNormals && showVertexNormals)
	{
		normalTransform = _normalWorldTransform * _normalObjectTransform;
		rend.DrawTriangles(&vertex_positions, _modelTransform, _worldTransform, cTransform, projection, showFaceNormals, normalTransform, &vertexNormalPositions, &normalFaces, steps);
	}
	else if (showFaceNormals)
	{
		normalTransform = _normalWorldTransform * _normalObjectTransform;
		rend.DrawTriangles(&vertex_positions, _modelTransform, _worldTransform, cTransform, projection, showFaceNormals, normalTransform, NULL, &normalFaces, steps);
	}
	else if (showVertexNormals)
	{
		normalTransform = _normalWorldTransform * _normalObjectTransform;
		rend.DrawTriangles(&vertex_positions, _modelTransform, _worldTransform, cTransform, projection, showFaceNormals, normalTransform, &vertexNormalPositions, NULL, steps);
	}
	else
	{
		normalTransform = _normalWorldTransform * _normalObjectTransform;
		rend.DrawTriangles(&vertex_positions, _modelTransform, _worldTransform, cTransform, projection, showFaceNormals, normalTransform, NULL, NULL, steps);
	}

}

void MeshModel::addWorldTransform(const mat4& transform) 
{
	_worldTransform = transform * _worldTransform;
}

void MeshModel::addNormalObjectTransform(const mat3& transform)
{
	_normalObjectTransform = transform * _normalObjectTransform;
}

void MeshModel::addNormalWorldTransform(const mat3 & transform)
{
	_normalWorldTransform = transform * _normalWorldTransform;
}

void MeshModel::addModelTransform(const mat4& transform)
{
	_modelTransform = transform * _modelTransform;
}

void MeshModel::addSteps(int xr, int yr)
{
	steps.first += xr;
	steps.second += yr;
}

mat4 MeshModel::getModelTransform() const
{
	return mat4(_modelTransform);
}
mat4 MeshModel::getWorldTransform() const
{
	return mat4(_worldTransform);
}

mat3 MeshModel::getNormalObjectTransform() const
{
	return mat3(_normalObjectTransform);
}

mat3 MeshModel::getNormalWorldTransform() const
{
	return mat3(_normalWorldTransform);
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


PrimitiveCube::PrimitiveCube(float size)
{
	vector<vec3> vertices;
	vec3 massCenter;
	vec3 normal;
	vector<vec3> triangle;
	vector<FaceIdcs> faces;

	/*1*/vertices.push_back(size * vec3(0.0, 0.0, 0.0));
	/*2*/vertices.push_back(size * vec3(0.0, 0.0, 1.0));
	/*3*/vertices.push_back(size * vec3(0.0, 1.0, 0.0));
	/*4*/vertices.push_back(size * vec3(0.0, 1.0, 1.0));

	/*5*/vertices.push_back(size * vec3(1.0, 0.0, 0.0));
	/*6*/vertices.push_back(size * vec3(1.0, 0.0, 1.0));
	/*7*/vertices.push_back(size * vec3(1.0, 1.0, 0.0));
	/*8*/vertices.push_back(size * vec3(1.0, 1.0, 1.0));

	faces.push_back(FaceIdcs(istringstream("1 7 5")));
	faces.push_back(FaceIdcs(istringstream("1 3 7")));
	faces.push_back(FaceIdcs(istringstream("1 4 3")));
	faces.push_back(FaceIdcs(istringstream("1 2 4")));
	faces.push_back(FaceIdcs(istringstream("3 8 7")));
	faces.push_back(FaceIdcs(istringstream("3 4 8")));
	faces.push_back(FaceIdcs(istringstream("5 7 8")));
	faces.push_back(FaceIdcs(istringstream("5 8 6")));
	faces.push_back(FaceIdcs(istringstream("1 5 6")));
	faces.push_back(FaceIdcs(istringstream("1 6 2")));
	faces.push_back(FaceIdcs(istringstream("2 6 8")));
	faces.push_back(FaceIdcs(istringstream("2 8 4")));

	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		triangle.clear();
		for (int i = 0; i < 3; i++)
		{
			vertex_positions.push_back(vertices[it->v[i] - 1]); /*BUG?*/
			triangle.push_back(vertices[it->v[i] - 1]); // triangle vertices
		}
		normal = cross(triangle[0] - triangle[2], triangle[1] - triangle[2]);
		normal = normalize(normal);
		normalFaces.push_back(normal);
	}
}

