#include "Transform.h"

Vertex ViewTransform::transformVertex(Vertex& v)
{
	return v;
}

void 

Vertex ProjectionTransform::transformVertex(Vertex& v)
{
	return Vertex(v.position.x / v.position.z*scaleFactor, v.position.y / v.position.z*scaleFactor, v.position.z, v.u, v.v);
}