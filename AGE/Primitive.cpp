#include "Primitive.h"

using namespace AGE;

Mesh* Primitive::CreatePlaneUnmanage(Vector3f points[4]){
	GLfloat vertex[12];
	
	for(uint i = 0; i < 4; i++){
		for(uint j = 0; j < 3; j++){
			vertex[i * 3 + j] = points[i][j];
		}
	}

	GLushort index[6] = {0, 1, 2, 0, 2, 3};

	return new Mesh(vertex, 12, index, 6);
}