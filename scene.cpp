#include "scene.h"

void Scene::AddObjectToScene(PhysicsObject* object)
{
	int index = object->objectId;
	objects[index] = object;
}
