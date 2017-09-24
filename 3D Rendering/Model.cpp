#include "Model.h"


Model::Model()
{
	translationMatrix = rotationMatrix = scaleMatrix = modelMatrix = glm::mat4();
	modelMatrixUpdated = true;
}
 
static float toFloat(string str)
{
	stringstream ss;
	ss.str(str);
	float f;
	ss >> f;
	return f;
}

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
static vector<string> split(string str, char delimiter)
{
	std::vector<std::string> elems;
	split(str, delimiter, elems);
	return elems;
}

/*void Model::loadFromFile(string filename, string filepath)
{
	ifstream file((filepath + filename).c_str());

	meshes.clear();
	materials.clear();

	vector<Vertex*> vertices;
	vector<glm::vec2> uvcoords;
	vector<glm::vec3> normals;

	string line;
	istringstream iss;
	float x, y, z;

	map<string, int> materialMap;
	int currMat = 0;

	while (true)
	{
		getline(file, line);
		if (file.eof()) break;

		iss.clear();
		iss.str(line);
		vector<string> tokens{ istream_iterator<string>{iss},
			istream_iterator<string>{} };

		if (tokens.size() <= 1) continue;

		if (tokens[0] == "mtllib")
		{
			if (materials.empty()) // Only if the first lib
			{
				// Load the material map from the material library file
				ifstream matFile(string(filepath + tokens[1]).c_str());
				if (!matFile)
				{
					cerr << "mtlib " << tokens[1] << " not loaded" << endl;
				}
				else
				{
					cout << "mtlib " << tokens[1] << " loading" << endl;
				}
				int matIndex = 0;
				while (true)
				{
					getline(matFile, line);
					if (matFile.eof())
					{
						cout << "End of " + tokens[1] << endl;
						break;
					}

					iss.clear();
					iss.str(line);
					vector<string> mtokens{ istream_iterator < string > {iss},
						istream_iterator < string > {} };
					if (mtokens.size() <= 1) continue;

					if (mtokens[0] == "newmtl")
					{
						materialMap[mtokens[1]] = matIndex++;
						materials.push_back(new Material());
						materials.back()->texture = NULL; 
						cout << "Added material #" << matIndex - 1 << ", " << mtokens[1] << endl;
					}
					else if (mtokens[0] == "map_Kd")
					{
						materials.back()->loadTextureFromFile(filepath + mtokens[1]);
						cout << "Added texture from " << mtokens[1] << endl;
					}
					else if (mtokens[0] == "Ka")
					{
						materials.back()->ambientColour = glm::vec3(toFloat(mtokens[1]), toFloat(mtokens[2]), toFloat(mtokens[3]));
					}
					else if (mtokens[0] == "Kd")
					{
						materials.back()->diffuseColour = glm::vec3(toFloat(mtokens[1]), toFloat(mtokens[2]), toFloat(mtokens[3]));
					}
				}
			}
		}
		else if (tokens[0] == "usemtl")
		{
			meshes.back().matIndex = materialMap[tokens[1]];
			cout << "Switching to material #" << currMat << ", " << tokens[1] << endl;
		}
		else if (tokens[0] == "v")
		{
			x = toFloat(tokens[1]);
			y = toFloat(tokens[2]);
			z = toFloat(tokens[3]);

			vertices.push_back(new Vertex(x, y, z, 0, 0));
			meshes.back().vertices.push_back(vertices.back());
		}
		else if (tokens[0] == "vt")
		{
			x = toFloat(tokens[1]);
			y = toFloat(tokens[2]);

			while (x < 0) x++;
			while (x > 1) x--;

			while (y < 0) y++;
			while (y > 1) y--;

			
			if (x < 0 || y < 0 || x > 1 || y > 1)
			{
				cout << "Error: " << x << ", " << y << endl;
			}

			uvcoords.push_back(glm::vec2(x, y));

		}
		else if (tokens[0] == "vn")
		{
			x = toFloat(tokens[1]);
			y = toFloat(tokens[2]);
			z = toFloat(tokens[3]);

			normals.push_back(glm::vec3(x, y, z));
		}
		else if (tokens[0] == "f")
		{
			vector<string> tokens1 = split(tokens[1], '/');
			vector<string> tokens2 = split(tokens[2], '/');
			vector<string> tokens3 = split(tokens[3], '/');
			
			int pos = 0;

			float v1 = toFloat(tokens1[pos]) - 1;
			float v2 = toFloat(tokens2[pos]) - 1;
			float v3 = toFloat(tokens3[pos]) - 1;
			
			glm::vec2 uv1 = glm::vec2(0), uv2 = glm::vec2(0), uv3 = glm::vec2(0);

			if (tokens1.size() >= 2 && tokens2.size() >= 2 && tokens3.size() >= 2)
			{
				pos++;
				uv1 = uvcoords[toFloat(tokens1[pos]) - 1];
				uv2 = uvcoords[toFloat(tokens2[pos]) - 1];
				uv3 = uvcoords[toFloat(tokens3[pos]) - 1];
			}
			vertices[v1]->uv = uv1;
			vertices[v2]->uv = uv2;
			vertices[v3]->uv = uv3;

			// Load vertex normals
			pos++;
			glm::vec3 n1 = normals[toFloat(tokens1[pos]) - 1];
			glm::vec3 n2 = normals[toFloat(tokens2[pos]) - 1];
			glm::vec3 n3 = normals[toFloat(tokens3[pos]) - 1];
			vertices[v1]->normal = glm::vec4(n1, 0);
			vertices[v2]->normal = glm::vec4(n2, 0);
			vertices[v3]->normal = glm::vec4(n3, 0);

			meshes.back().triangles.push_back(Triangle(vertices[v1],
				vertices[v2],
				vertices[v3]));
		}
		else if (tokens[0] == "o")
		{
			// New object, so delete current vertices and uvcoords
			//vertices.clear();
			//uvcoords.clear();
			cout << "New object: " << tokens[1] << endl;
			meshes.push_back(Mesh());
		}
	}

	//texture.loadFromFile(texturename);
}
*/
void Model::loadFromFile(string filename, string filepath)
{
	// Code adapted from http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=23
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath + filename,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		cerr << "Error in Model::loadFromFile() - Could not load " << filename << endl;
		return;
	}

	// For each mesh in the scene
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// Add the mesh to the object
		aiMesh* mesh = scene->mMeshes[i];
		meshes.push_back(Mesh());
		// Add the material index to the mesh
		meshes.back().matIndex = mesh->mMaterialIndex;
		
		// Load vertices first
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{

			/*Vertex* v = new Vertex(mesh->mVertices[j].x,
								mesh->mVertices[j].y,
								mesh->mVertices[j].z,
								mesh->mTextureCoords[0][j].x,
								mesh->mTextureCoords[0][j].y,
								mesh->mNormals[j].x,
								mesh->mNormals[j].y,
								mesh->mNormals[j].z
								);*/

			// Create a new vertex. This will be deleted by the object
			Vertex* v = new Vertex();
			v->position = glm::vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1);

			if (mesh->mTextureCoords[0] == NULL) // The object has no uv coords
			{
				v->uv = glm::vec2(0, 0);
			}
			else // The object has uv coords
			{
				v->uv.x = mesh->mTextureCoords[0][j].x;
				v->uv.y = mesh->mTextureCoords[0][j].y;
			}
			// Cap uv coords from 0-1
			while (v->uv.x > 1) v->uv.x--;
			while (v->uv.y > 1) v->uv.y--;
			
			while (v->uv.x < 0) v->uv.x++;
			while (v->uv.y < 0) v->uv.y++;

			v->normal = glm::vec4(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z, 0);

			meshes.back().vertices.push_back(v);
		}

		// Now load faces
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			meshes.back().triangles.push_back(Triangle());
			// Link the faces with the correct vertices
			for (unsigned int k = 0; k < 3; k++)
			{
				meshes.back().triangles.back().vertices[k] = meshes.back().vertices[face.mIndices[k]];
			}
		}
	}

	// Load materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		int texIndex = 0;
		aiString textureFile;

		materials.push_back(new Material());

		// Set texture
		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &textureFile) == AI_SUCCESS)
		{
			materials.back()->loadTextureFromFile(filepath + textureFile.C_Str());
		}
		else
		{
			materials.back()->texture = NULL;
		}

		// Set color. For now just use white, to simplify things
		materials.back()->ambientColour = glm::vec3(1);
		materials.back()->diffuseColour = glm::vec3(1);

	}
	int numTris = 0, numVertices = 0;
	for (int i = 0; i < meshes.size(); i++)
	{
		numTris += meshes[i].triangles.size();
		numVertices += meshes[i].vertices.size();
	}

	cout << "Successfully loaded " << filename << ": " << numVertices << " vertices, " << numTris << " triangles." << endl;
}

void Model::setTranslation(glm::vec3 translation)
{
	static glm::mat4 identity = glm::mat4();
	translationMatrix = glm::translate(identity, translation);
	modelMatrixUpdated = false;
}
void Model::translate(glm::vec3 translation)
{
	translationMatrix = glm::translate(translationMatrix, translation);
	modelMatrixUpdated = false;
}
void Model::setRotation(float angle, glm::vec3 axis)
{
	static glm::mat4 identity = glm::mat4();
	rotationMatrix = glm::rotate(identity, angle, axis);
	modelMatrixUpdated = false;
}
void Model::rotate(float angle, glm::vec3 axis)
{
	rotationMatrix = glm::rotate(rotationMatrix, angle, axis);
	modelMatrixUpdated = false;
}
void Model::setScale(glm::vec3 factor)
{
	static glm::mat4 identity = glm::mat4();
	scaleMatrix = glm::scale(identity, factor);
	modelMatrixUpdated = false;
}
void Model::scale(glm::vec3 factor)
{
	scaleMatrix = glm::scale(scaleMatrix, factor);
	modelMatrixUpdated = false;
}
glm::mat4 Model::getModelMatrix()
{
	if (!modelMatrixUpdated)
	{
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		modelMatrixUpdated = true;
	}
	return modelMatrix;
}