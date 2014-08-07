#include "Scene.h"
#include "ResourceManager.h"
#include "Primitive.h"

using namespace AGE;

SceneNode::SceneNode(){
}

SceneNode::~SceneNode(){
}

void SceneNode::Attach(Attachable* attach){
	mChildrenAttachable.push_back(attach);
}


Scene::Scene(){
}

Scene::~Scene(){
	for(unsigned int i = 0; i < mAllAttachable.size(); i++){
		delete mAllAttachable[i];
	}
}

Mesh* Scene::LoadMesh(){
	Vector3f points[4];

	points[0].Set(0.5, 0.6, 1);
	points[1].Set(0.5, -0.5, 1);
	points[2].Set(-0.5, -0.5, 1);
	points[3].Set(-0.5, 0.5, 1);

	Mesh * attObj = Primitive::GetInstance()->CreatePlaneUnmanage(points);//ResourceManager::GetInstance()->LoadMeshUnmanage();

	mAllAttachable.push_back(attObj);

	return attObj;
}
