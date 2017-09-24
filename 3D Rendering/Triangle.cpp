#include "Triangle.h"


Triangle::Triangle(Vertex* v1, Vertex* v2, Vertex* v3)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;

	//calcNormal(); // Normal calculation will come from vertices
}
Triangle::Triangle(Vertex* v[3])
{
	vertices[0] = v[0];
	vertices[1] = v[1];
	vertices[2] = v[2];

	//calcNormal();
}