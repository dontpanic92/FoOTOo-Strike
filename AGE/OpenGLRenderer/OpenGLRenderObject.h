#ifndef __AGE_OPENGLRENDEROBJECTDATA_HEADER__
#define __AGE_OPENGLRENDEROBJECTDATA_HEADER__

#include "../Renderable.h"

namespace AGE{

	class OpenGLRenderObject : public RenderObject
	{
	public:
		GLuint VertexArrayBufferObject;
		GLuint BufferObjects[3];

		void Update() { 
			Mesh->UpdateVertex();
			glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0,
				sizeof(GLfloat) * Mesh->GetNumberOfVertex() * 3, Mesh->GetVertexData());
		}

		void Destroy()
		{
			if (BufferObjects[0]) {
				glDeleteBuffers(3, BufferObjects);
				BufferObjects[0] = 0;
			}
			if (VertexArrayBufferObject) {
				glBindVertexArray(0);
				glDeleteVertexArrays(1, &VertexArrayBufferObject);
				VertexArrayBufferObject = 0;
			}
		}
	};

}

#endif