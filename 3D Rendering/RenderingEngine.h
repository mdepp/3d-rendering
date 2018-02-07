#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
#include "GameEngine.h"
#include "DepthPixel.h"
#include <vector>
#include <algorithm>
#include <map>
#include "Model.h"
#include "Light.h"

using namespace std;

class GameEngine;

class RenderingEngine
{
public:
	RenderingEngine(GameEngine* e);
	~RenderingEngine();

	// Functions to set matrices
	void lookAt(glm::vec3& cameraPosition, glm::vec3& cameraTarget);
	void setViewMatrix(glm::mat4 vMat);

	vector<vector<DepthPixel>>* renderModels(vector<Model>& models, const vector<Light>& lights);

private:
	void renderTriangle(const Triangle& tri, Material* material);
	void renderModel(Model& model, const vector<Light>& lights);

	glm::mat4 viewMatrix, projectionMatrix, MVPmatrix;
	glm::vec3 upVector;

	vector<vector<DepthPixel>> depthBuffer;

	GameEngine* engine;
};