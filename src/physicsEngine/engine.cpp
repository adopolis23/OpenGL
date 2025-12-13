#include "engine.h"


Engine::Engine(const Camera* cam)
{

}

void Engine::Update(Scene& scene, float dt)
{

    for (auto obj : scene.objects)
    {
        obj->position.x += 0.0001 * dt;
    }


}