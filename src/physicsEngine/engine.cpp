#include "engine.h"


Engine::Engine(const Camera* cam, const Scene& scene)
    :densitySystem(cam, scene), spatialGrid(scene.objectCount, densitySystem.kernelRadius*2, cam)
{
    camera = cam;

    densitySystem.spatialGrid = &spatialGrid;

}


void Engine::HandleCollisions(PhysicsObject* obj)
{
    bool collision = false;

    // left side of object contacts left wall
    if (obj->position.x - obj->radius < camera->left_world_bound && obj->velocity.x < 0)
    {
        obj->position.x = camera->left_world_bound + obj->radius;
        obj->velocity.x *= -1;
        collision = true;
    }

    // right side of object contacts right wall
    if (obj->position.x + obj->radius > camera->right_world_bound && obj->velocity.x > 0)
    {
        obj->position.x = camera->right_world_bound - obj->radius;
        obj->velocity.x *= -1;
        collision = true;
    }

    // top of object contacts top wall
    if (obj->position.y + obj->radius > camera->top_world_bound && obj->velocity.y > 0)
    {
        obj->position.y = camera->top_world_bound - obj->radius;
        obj->velocity.y *= -1;
        collision = true;
    }

    // bottom of object contacs bottom wall
    if (obj->position.y - obj->radius < camera->bottom_world_bound && obj->velocity.y < 0)
    {
        obj->position.y = camera->bottom_world_bound + obj->radius;
        obj->velocity.y *= -1;
        collision = true;
    }

    // handle physics only if there was a collision
    if (collision)
    {
        obj->velocity *= collisionDampingFactor;
    }

}


void Engine::ApplyGravity(Scene& scene)
{
    float accel_gravity = 0.000003f;

    for (auto& [id, obj] : scene.objects)
    {
        obj->velocity.y += (-1) * accel_gravity;
    }
}



void Engine::Update(Scene& scene, float dt)
{

    spatialGrid.UpdateQuadLocations(scene);


    //update the values in the particleDensityGradient object to store the density gradient at each particles position
    //densitySystem.CalculateDensityGradientAtParticles(scene);
    densitySystem.CalculatePressureForParticles(scene);
    ApplyPressureForceToParticles(scene, dt);

    //ApplyGravity(scene);

    for (auto& [id, obj] : scene.objects)
    {
        // add velocities to each particles position
        obj->position.x += obj->velocity.x * dt;
        obj->position.y += obj->velocity.y * dt;

        // handle collisions for that object
        HandleCollisions(obj);

    }

    // update the values in the DensityField object to display the density in the background
	// not done anymore because this is handled by the shaders now
    //CalculateDensityField(scene);

}


void Engine::ApplyRadialForceToParticlesAtPosition(Scene& scene, const glm::vec2& position, float forceMagnitude, float radius, float dt)
{
    float max_velocity = 0.003f;
    
	// first we need to find which quads are in range of the radial force
	// skip for now
    float quadSize = spatialGrid.quadSize;
    int quads_to_check = static_cast<int>(std::ceil(radius / quadSize));

	float accelerationX, accelerationY;

    for (auto& [id, obj] : scene.objects)
    {
		// get direction from force position to particle
		glm::vec2 dir = glm::vec2(obj->position.x, obj->position.y) - position;

		//get distance from force position to particle
		float dist = glm::length(dir);

        if (dist > radius) continue;

		// normalize direction
		dir = dir / dist;

		//constant force for all particles in radius
        glm::vec2 force = glm::vec2(dir.x * forceMagnitude, dir.y * forceMagnitude);

        // acceleration = F / m
        accelerationX = force.x / obj->mass;
        accelerationY = force.y / obj->mass;

        // integrate velocity using dt
        obj->velocity.x += accelerationX * dt;
        obj->velocity.y += accelerationY * dt;

        // clamp velocity to avoid explosion (tune max_velocity)
        if (obj->velocity.x > max_velocity) obj->velocity.x = max_velocity;
        if (obj->velocity.x < -max_velocity) obj->velocity.x = -max_velocity;
        if (obj->velocity.y > max_velocity) obj->velocity.y = max_velocity;
        if (obj->velocity.y < -max_velocity) obj->velocity.y = -max_velocity;

    }

}


void Engine::ApplyPressureForceToParticles(Scene& scene, float dt)
{
    float accelerationX, accelerationY;
    glm::vec2 force;
    float max_velocity = 0.003f;

    float constant = 0.0001f;

    //f = m * a
    // therefor the acceleration to apply to the particle a = f / m

    for (auto& [id, obj] : scene.objects)
    {
        // check if there is a density gradient entry for this particle
        if (densitySystem.particlePressureForce.find(obj->objectId) == densitySystem.particlePressureForce.end())
        {
            continue;
        }

        force = densitySystem.particlePressureForce.at(obj->objectId);

        // acceleration = F / m
        accelerationX = force.x / obj->mass;
        accelerationY = force.y / obj->mass;

        // integrate velocity using dt
        obj->velocity.x += accelerationX * dt;
        obj->velocity.y += accelerationY * dt;

        // clamp velocity to avoid explosion (tune max_velocity)
        if (obj->velocity.x > max_velocity) obj->velocity.x = max_velocity;
        if (obj->velocity.x < -max_velocity) obj->velocity.x = -max_velocity;
        if (obj->velocity.y > max_velocity) obj->velocity.y = max_velocity;
        if (obj->velocity.y < -max_velocity) obj->velocity.y = -max_velocity;
    }
}



void Engine::SetCollisionDampingFactor(float new_value)
{
    collisionDampingFactor = new_value;
}
