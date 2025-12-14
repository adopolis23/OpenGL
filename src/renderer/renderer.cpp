#include "renderer.h"

void Renderer::Render(const Scene& scene)
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderDensityBackground();

    for (auto obj : scene.objects) 
    {

        // bind the default shader for now every time
        defaultShader->bind();

        obj->GenerateObjectModel();
        // model comes from the object and has all of the transformation data
        defaultShader->setMat4("model", obj->model);
        // view and projection come from the renderer and have all of the aspect information
        defaultShader->setMat4("view", view);
        defaultShader->setMat4("projection", camera->projection);

        glBindVertexArray(obj->vao);
        glDrawArrays(GL_TRIANGLES, 0, obj->VertexCount);
    }

}


Renderer::Renderer(const Camera* cam)
    :camera(cam)
{
    view = glm::mat4(1.0f); // identity camera

    densityBackground = new DensityBackground();

    // set the default shader for now. This might be replaced per object in the future
    defaultShader = new Shader("/home/bran406/dev/OpenGL/src/shaders/vertex/default_vertex.glsl", "/home/bran406/dev/OpenGL/src/shaders/fragment/default_fragment.glsl");

    densityShader = new Shader("/home/bran406/dev/OpenGL/src/shaders/vertex/density_vertex.glsl", "/home/bran406/dev/OpenGL/src/shaders/fragment/density_fragment.glsl");
}


void Renderer::InitDensityResources(const DensityField& densityField)
{
    // generate the density texture with no data in it yet
    glGenTextures(1, &densityTexture);
    glBindTexture(GL_TEXTURE_2D, densityTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // allocate GPU memory once
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R32F,
        densityField.width,
        densityField.height,
        0,
        GL_RED,
        GL_FLOAT,
        nullptr   // IMPORTANT: no data yet
    );
}

void Renderer::UploadDensity(const DensityField& densityField)
{
    // bind the texture and update the data
    glBindTexture(GL_TEXTURE_2D, densityTexture);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0, 0,
        densityField.width,
        densityField.height,
        GL_RED,
        GL_FLOAT,
        densityField.density.data()
    );
}

void Renderer::RenderDensityBackground()
{
    // need to get the object and bind the correct shader and texture

    densityShader->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, densityTexture);
    densityShader->setInt("uDensity", 0);

    glBindVertexArray(densityBackground->vao);
    glDrawArrays(GL_TRIANGLES, 0, densityBackground->VertexCount);
}