#ifndef __AGE_MESH_HEADER__
#define __AGE_MESH_HEADER__

//#include "../3rd/glew-1.10.0/include/GL/glew.h"
//#include "../3rd/glew-1.10.0/include/GL/wglew.h"
#include "Math.h"

#include <GL\glew.h>

namespace AGE{

	class Mesh
	{
	public:
		Mesh(GLfloat* vertexArray, GLfloat* normalArray, int vertexNum, GLushort* indexArray, int indexNum);

		~Mesh();

		const GLfloat* GetVertexData() const{ return mVertexData;}

		const GLushort* GetIndexData() const{ return mIndexData; }

		const GLfloat* GetNormalData() const{ return mNormalData; }

		int GetVertexNum() const{ return mVertexNum; }

		int GetIndexNum() const{ return mIndexNum; }

	private:
		int mVertexNum;
		int mIndexNum;

		GLfloat* mVertexData;
		GLfloat* mNormalData;
		GLushort* mIndexData;
	};

}
#endif
