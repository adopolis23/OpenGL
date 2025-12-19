#include "scene.h"

Scene::Scene(const Camera& cam)
{
	camera = &cam;
}

void Scene::AddObjectToScene(PhysicsObject* object)
{
	int index = object->objectId;
	objects[index] = object;
}
