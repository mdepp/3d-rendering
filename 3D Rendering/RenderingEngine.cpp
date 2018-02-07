#include "RenderingEngine.h"

RenderingEngine::RenderingEngine(GameEngine* e): upVector(0, 1, 0)
{
	engine = e;
	projectionMatrix = glm::perspective(45.f, (float)engine->SCREEN_WIDTH / (float)engine->SCREEN_HEIGHT, 0.1f, 1000.f);

	depthBuffer.resize(engine->SCREEN_WIDTH);
	for (auto& array : depthBuffer)
	{
		array.resize(engine->SCREEN_HEIGHT);
	}
}

// source channel, target channel, width, height, radius
void gaussBlur_1(vector<vector<DepthPixel>>* scl, vector<vector<DepthPixel>>* tcl, float r)
{
	int w = scl->size();
	int h = scl->at(0).size();
	int rs = ceil(r * 2.57);     // significant radius
	float PI = 3.14159f;

	int i, j, iy, ix, x, y, dsq;
	float wght, wsum, valr, valg, valb;



	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			wsum = 0;
			valr = valg = valb = 0;

			for (iy = i - rs; iy < i + rs + 1; iy++)
			{
				for (ix = j - rs; ix < j + rs + 1; ix++)
				{
					x = min(w - 1, max(0, ix));
					y = min(h - 1, max(0, iy));
					dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
					wght = exp(-dsq / (2 * r*r)) / (PI * 2.f * r*r);
					valr += scl->at(x)[y].r * wght;
					valg += scl->at(x)[y].g * wght;
					valb += scl->at(x)[y].b * wght;
					wsum += wght;
				}
			}
			tcl->at(j)[i].r = valr / wsum;
			tcl->at(j)[i].g = valg / wsum;
			tcl->at(j)[i].b = valb / wsum;

		}
	}
}

RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::lookAt(glm::vec3& cameraPosition, glm::vec3& cameraTarget)
{
	viewMatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);
}
void RenderingEngine::setViewMatrix(glm::mat4 vMat)
{
	viewMatrix = vMat;
}


vector<vector<DepthPixel>>* RenderingEngine::renderModels(vector<Model>& models, const vector<Light>& lights)
{
	for (int i = 0; i < models.size(); i++)
	{
		renderModel(models[i], lights);
	}

	//gaussBlur_1(&depthBuffer, &depthBuffer, 1);

	return &depthBuffer;
}
// Note - This function can actually change the model, permanently
void RenderingEngine::renderModel(Model& model, const vector<Light>& lights)
{
	MVPmatrix = projectionMatrix * viewMatrix * model.getModelMatrix();

	int i, j, k;
	glm::vec3 ambientColour, diffuseColour; // Diffuse colour is outputted as a percentage (0-1), ambient as an offset (0-255)
	float intensity;

	// Clear the depth buffer
	for (i = 0; i < engine->SCREEN_WIDTH; i++)
	{
		for (j = 0; j < engine->SCREEN_HEIGHT; j++)
		{
			depthBuffer[i][j].clear();
		}
	}
	// Do per-vertex calculations
	Vertex* vert;
	for (int i = 0; i < model.getMeshes()->size(); i++)
	{
		for (int j = 0; j < model.getMeshes()->at(i).vertices.size(); j++)
		{
			vert = model.getMeshes()->at(i).vertices[j];

			vert->transformedNormal = glm::vec3(glm::normalize(model.getModelMatrix() * vert->normal));

			// Calculate lighting
			vert->ambientColour = glm::vec3(0, 0, 0);
			vert->diffuseColour = glm::vec3(0, 0, 0);
			for (k = 0; k < lights.size(); k++)
			{
				if (lights[k].type == Light::TYPE_AMBIENT)
				{
					vert->ambientColour += lights[k].colour * lights[k].intensity;
				}
				else if (lights[k].type == Light::TYPE_DIFFUSE)
				{
					if (lights[k].position.w == 0) // Directional lighting - easy
					{
						intensity = lights[k].intensity*glm::dot(vert->transformedNormal, glm::vec3(lights[k].position));
						if (intensity > 0)
						{
							vert->diffuseColour += lights[k].colour * intensity;
						}
					}
					else if (lights[k].position.w == 1) // Positional lighting - hard
					{
						// Actually, skip this for now
					}
				}
			}
			// Turn diffuse colour into a percentage
			vert->diffuseColour /= 255.f;
		}
	}

	// Now render each triangle
	for (i = 0; i < model.getMeshes()->size(); i++)
	{
		for (j = 0; j < model.getMeshes()->at(i).triangles.size(); j++)
		{
			renderTriangle(model.getMeshes()->at(i).triangles[j], model.getMaterial(model.getMeshes()->at(i).matIndex));
		}
	}
	/*
	for (i = 0; i < model.getMeshes()->size(); i++)
	{
		for (j = 0; j < model.getMeshes()->at(i).triangles.size(); j++)
		{
			// Calculate normal in world space
			model.getMeshes()->at(i).triangles[j].transformedNormal = glm::vec3(glm::normalize(model.getModelMatrix() * model.getMeshes()->at(i).triangles[j].normal));

			// Calculate lighting
			diffuseColour = glm::vec3(0, 0, 0);
			ambientColour = glm::vec3(0, 0, 0);
			for (k = 0; k < lights.size(); k++)
			{
				if (lights[k].type == Light::TYPE_AMBIENT)
				{
					ambientColour += lights[k].colour * lights[k].intensity;
				}
				else if (lights[k].type == Light::TYPE_DIFFUSE)
				{
					if (lights[k].position.w == 0) // Directional lighting - easy
					{
						intensity = lights[k].intensity*glm::dot(model.getMeshes()->at(i).triangles[j].transformedNormal, glm::vec3(lights[k].position));
						if (intensity > 0)
						{
							diffuseColour += lights[k].colour * intensity;
						}
					}
					else if (lights[k].position.w == 1) // Positional lighting - hard
					{
						// Actually, skip this for now
					}
				}
			}
			// Turn diffuse colour into a percentage
			diffuseColour /= 255.f;

			renderTriangle(model.getMeshes()->at(i).triangles[j], model.getMaterial(model.getMeshes()->at(i).matIndex), ambientColour, diffuseColour);
		}
	}*/
}
static float cross(glm::vec2 v1, glm::vec2 v2)
{
	return (v1.x*v2.y) - (v1.y*v2.x);
}
static float clamp(float val)
{
	if (val > 255) return 255;
	else if (val < 0) return 0;
	else return val;
}

inline static glm::vec3 calcNormal(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3)
{
	return glm::normalize(glm::cross(glm::vec3(v1 - v2), glm::vec3(v1 - v3)));
}

// Precondition: MVP matrix updated
void RenderingEngine::renderTriangle(const Triangle& tri, Material* material)
{
	Triangle newTri;
	// Transform the triangle to screen space
	bool inFront = true;
	for (int i = 0; i < 3; i++)
	{
		newTri.vertices[i] = tri.vertices[i];

		newTri.vertices[i]->transformedPosition = MVPmatrix * tri.vertices[i]->position;

		if (newTri.vertices[i]->transformedPosition.w < 0) inFront = false;

		newTri.vertices[i]->transformedPosition /= newTri.vertices[i]->transformedPosition.w;

		newTri.vertices[i]->transformedPosition.x = newTri.vertices[i]->transformedPosition.x*(float)engine->HALF_SCREEN_WIDTH + engine->HALF_SCREEN_WIDTH;
		newTri.vertices[i]->transformedPosition.y = newTri.vertices[i]->transformedPosition.y*(float)engine->HALF_SCREEN_HEIGHT + engine->HALF_SCREEN_HEIGHT;

	}
	if (!inFront) return; // Make sure no w value is negative. Removes things behind camera, I think

	// Calculate the normal, in camera space
	glm::vec3 normal = calcNormal(newTri.vertices[0]->transformedPosition, newTri.vertices[1]->transformedPosition, newTri.vertices[2]->transformedPosition); // TODO - Maybe change this to something better
	
	glm::vec3 diffuseColour;
	glm::vec3 ambientColour;

	// Filter front facing triangles
	static glm::vec3 viewDir = glm::vec3(0, 0, -1);
	if (glm::dot(glm::vec3(newTri.vertices[0]->transformedPosition) - viewDir, normal) <= 0.f)
	{
		// Facing the wrong way
		return;
	}

	// Get the value for and draw every pixel in the triangle into the depth buffer. Code adapted from http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
	int maxX = max(max(newTri.vertices[0]->transformedPosition.x, newTri.vertices[1]->transformedPosition.x), newTri.vertices[2]->transformedPosition.x);
	int minX = min(min(newTri.vertices[0]->transformedPosition.x, newTri.vertices[1]->transformedPosition.x), newTri.vertices[2]->transformedPosition.x);
	int maxY = max(max(newTri.vertices[0]->transformedPosition.y, newTri.vertices[1]->transformedPosition.y), newTri.vertices[2]->transformedPosition.y);
	int minY = min(min(newTri.vertices[0]->transformedPosition.y, newTri.vertices[1]->transformedPosition.y), newTri.vertices[2]->transformedPosition.y);
	
	if (maxX < 0 || maxY < 0 || minX > engine->SCREEN_WIDTH || minY > engine->SCREEN_HEIGHT) return;

	maxX = min(maxX, engine->SCREEN_WIDTH-1);
	maxY = min(maxY, engine->SCREEN_HEIGHT-1);
	minX = max(minX, 0);
	minY = max(minY, 0);


	glm::vec2 vs1 = glm::vec2(newTri.vertices[1]->transformedPosition.x - newTri.vertices[0]->transformedPosition.x, newTri.vertices[1]->transformedPosition.y - newTri.vertices[0]->transformedPosition.y);
	glm::vec2 vs2 = glm::vec2(newTri.vertices[2]->transformedPosition.x - newTri.vertices[0]->transformedPosition.x, newTri.vertices[2]->transformedPosition.y - newTri.vertices[0]->transformedPosition.y);
	float s, t;
	int x, y;
	float z;

	glm::vec2 diff, q;

	glm::vec3 p1, p2, p3, f, f1, f2, f3;
	float a, a1, a2, a3;
	glm::vec3 texColour;

	for (x = minX; x <= maxX; x++)
	{
		for (y = minY; y <= maxY; y++)
		{
			q = glm::vec2(x - newTri.vertices[0]->transformedPosition.x, y - newTri.vertices[0]->transformedPosition.y);

			s = cross(q, vs2) / cross(vs1, vs2);
			t = cross(vs1, q) / cross(vs1, vs2);

			if ((s >= 0) && (t >= 0) && (s + t <= 1))
			{
				// The point x, y is inside the triangle. For now, just draw it.

				
				// Find z-depth
				diff = glm::vec2(newTri.vertices[0]->transformedPosition.x - x, newTri.vertices[0]->transformedPosition.y - y);
				z = newTri.vertices[0]->transformedPosition.z + (diff.x*normal.x + diff.y*normal.y) / normal.z;
				
				if (depthBuffer[x][y].willChange(z))
				{

					// calculate vectors from point f to vertices p1, p2 and p3:

					p1 = glm::vec3(newTri.vertices[0]->transformedPosition);
					p2 = glm::vec3(newTri.vertices[1]->transformedPosition);
					p3 = glm::vec3(newTri.vertices[2]->transformedPosition);

					f = glm::vec3(x, y, z);

					f1 = p1 - f;
					f2 = p2 - f;
					f3 = p3 - f;

					// calculate the areas and factors (order of parameters doesn't matter):
					a = glm::length(glm::cross(p1 - p2, p1 - p3)); // main triangle area a
					a1 = glm::length(glm::cross(f2, f3)) / a; // p1's triangle area / a
					a2 = glm::length(glm::cross(f3, f1)) / a; // p2's triangle area / a 
					a3 = 1.f - a1 - a2;
					//a3 = glm::length(glm::cross(f1, f2)) / a; // p3's triangle area / a
					// find the uv corresponding to point f (uv1/uv2/uv3 are associated to p1/p2/p3):

					// Calculate pixel colour from uv coords
					static glm::vec3 white(255, 255, 255);
					texColour = material->texture == NULL ? white : material->texture->getColour(tri.vertices[0]->uv * a1 + tri.vertices[1]->uv * a2 + tri.vertices[2]->uv * a3);

					diffuseColour = a1 * tri.vertices[0]->diffuseColour + a2 * tri.vertices[1]->diffuseColour + a3 * tri.vertices[2]->diffuseColour;
					ambientColour = a1 * tri.vertices[0]->ambientColour + a2 * tri.vertices[1]->ambientColour + a3 * tri.vertices[2]->ambientColour;


					depthBuffer[x][y].changePixel(z, clamp(texColour.r * diffuseColour.r * material->diffuseColour.r + ambientColour.r * material->ambientColour.r),
								clamp(texColour.g * diffuseColour.g * material->diffuseColour.g + ambientColour.g * material->ambientColour.g),
								clamp(texColour.b * diffuseColour.b * material->diffuseColour.b + ambientColour.b * material->ambientColour.b));
				}
			}
		}
	}
	
	//SDL_SetRenderDrawColor(engine->windowRenderer, 0, 0, 0, 1);
}
