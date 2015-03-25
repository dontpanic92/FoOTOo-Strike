#include "Attachable.h"
#include "Scene.h"
using namespace AGE;

void Attachable::SetParent(SceneNode* parent)
{
	mParent = parent;
}