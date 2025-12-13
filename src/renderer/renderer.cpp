#include "renderer.h"

void Renderer::Render(const Scene& scene)
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto obj : scene.objects) {

        // bind the default shader for now every time
        defaultShader->bind();
        // model comes from the object and has all of the transformation data
        defaultShader->setMat4("model", obj->model);
        // view and projection come from the renderer and have all of the aspect information
        defaultShader->setMat4("view", view);
        defaultShader->setMat4("projection", projection);

        glBindVertexArray(obj->vao);
        glDrawArrays(GL_TRIANGLES, 0, obj->VertexCount);
    }
}


Renderer::Renderer(int w, int h)
: window_width(w), window_height(h)
{
    view = glm::mat4(1.0f); // identity camera

    // set the projection matrix that accounts for the aspect ratio of the window
    float aspect_ratio = (float)window_width / (float)window_height;
    projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f);

    // set the default shader for now. This might be replaced per object in the future
    defaultShader = new Shader("/home/brandon/dev/OpenGL/src/shaders/vertex/default_vertex.glsl", "/home/brandon/dev/OpenGL/src/shaders/fragment/default_fragment.glsl");
}