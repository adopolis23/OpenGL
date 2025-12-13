#include "renderer.h"

void Renderer::Render(const Scene& scene)
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto obj : scene.objects) {
        obj->shader->bind();

        obj->shader->setMat4("model", obj->model);

        glBindVertexArray(obj->vao);
        glDrawArrays(GL_TRIANGLES, 0, obj->VertexCount);
    }
}