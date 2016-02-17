#ifndef __AGE_PRIMITIVE_HEADER__
#define __AGE_PRIMITIVE_HEADER__

#include "Singleton.h"
#include "Renderable.h"
#include <vector>

namespace AGE
{
	class AGE_EXPORT Primitive : public Singleton<Primitive>
	{
	public:
		class MeshBuilder
		{
		public:
			MeshBuilder();
			MeshBuilder(const Mesh::Vertex* base);
			void AddFace(Mesh::Vertex a, Mesh::Vertex b, Mesh::Vertex c);
			void AddFaceByIndex(uint a, uint b, uint c);

			Mesh* BuildMesh();

		private:
			const Mesh::Vertex* mBase;
			std::vector<Mesh::Vertex> mVertex;
		};

		static Mesh* CreateRectangleUnmanage(float width, float height);

		static Mesh* CreateSphereUnmanage(float radius);
	};
}

#endif