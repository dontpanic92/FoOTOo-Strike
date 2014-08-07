#ifndef __AGE_MESH_HEADER__
#define __AGE_MESH_HEADER__

#include "../3rd/glew-1.10.0/include/GL/glew.h"
#include "../3rd/glew-1.10.0/include/GL/wglew.h"
#include "Math.h"

namespace AGE{
	
	class Attachable
	{
	public:
		virtual ~Attachable(){};
	private:
		Transform mTransform;
	};

	class Mesh : public Attachable
	{
	public:
		Mesh(GLfloat* vertexArray, int vertexNum, GLushort* indexArray, int indexNum);

		~Mesh();

		const GLfloat* GetVertexData() const{ return mVertexData;}

		const GLushort* GetIndexData() const{ return mIndexData; }

		int GetVertexNum() const{ return mVertexNum; }

		int GetIndexNum() const{ return mIndexNum; }

	private:
		int mVertexNum;
		int mIndexNum;

		GLfloat* mVertexData;
		GLushort* mIndexData;
	};

}
#endif
