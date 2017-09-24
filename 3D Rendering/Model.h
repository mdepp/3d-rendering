#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

// A simple class to store data for each material in the object. For now uses only textures, but maybe add lighting and such later on
class Material
{
public:
	Material(string texturefile)
	{
		loadTextureFromFile(texturefile);
		ambientColour = diffuseColour = glm::vec3(1);
	}
	void loadTextureFromFile(string texturefile)
	{
		if (texture == NULL) texture = new Texture();
		if (!texture->loadFromFile(texturefile))
		{
			delete texture;
			texture = NULL;
		}
	}
	Material(): texture(NULL) {}

	Texture* texture = NULL;
	glm::vec3 ambientColour, diffuseColour; // As a percentage (0-1)
};

class Mesh
{
public:
	vector<Triangle> triangles;
	vector<Vertex*> vertices;
	int matIndex;
};

class Model
{
public:
	Model();
	void loadFromFile(string filename, string filepath);
	vector<Mesh>* getMeshes() { return &meshes; }
	Material* getMaterial(int index) { return materials[index]; }
	glm::mat4 getModelMatrix();
	void translate(glm::vec3 translation);
	void setTranslation(glm::vec3 translation);
	void rotate(float angle, glm::vec3 axis);
	void setRotation(float angle, glm::vec3 axis);
	void scale(glm::vec3 factor);
	void setScale(glm::vec3 factor);
	virtual ~Model()
	{
		for (unsigned int i = 0; i < materials.size(); i++)
			delete materials[i];

		for (unsigned int i = 0; i < meshes.size(); i++)
			for (unsigned int j = 0; j < meshes[i].vertices.size(); j++)
				delete meshes[i].vertices[j];
	}

protected:
	vector<Mesh> meshes;
	vector<Material*> materials;

	glm::mat4 modelMatrix;
	glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;
	bool modelMatrixUpdated;
};