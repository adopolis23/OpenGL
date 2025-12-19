#pragma once

#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:

    Camera(int width, int height);

    // these are the actual world bounds after correction for aspect ratio. 
    // ex. if world height is 2 then the top and bottom are at +1 and -1
    // and left and right are at +aspect and -aspect
    float left_world_bound;
    float right_world_bound;
    float top_world_bound;
    float bottom_world_bound;

    // width / height
    float aspect_ratio;

    // example heihgt=2.0 width=height*aspect
    float world_height, world_width;

    // pixel height and width of the window
    int window_width;
    int window_height;

    // the 4d projection that takes things from the local space to the world space with this aspect ratio
    glm::mat4 projection;

private:


};