#pragma once

#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:

    Camera(int width, int height);

    float left_world_bound;
    float right_world_bound;
    float top_world_bound;
    float bottom_world_bound;

    float aspect_ratio;

    float world_height, world_width;

    int window_width;
    int window_height;

    glm::mat4 projection;

private:


};