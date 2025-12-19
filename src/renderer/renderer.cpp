#include "renderer.h"

void Renderer::Render(const Scene& scene)
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderDensityBackground();

    //for (const auto& [id, obj] : scene.objects) 
    //{

    //   // bind the default shader for now every time
    //   defaultShader->bind();

    //   obj->GenerateObjectModel();
    //   // model comes from the object and has all of the transformation data
    //   defaultShader->setMat4("model", obj->model);
    //   // view and projection come from the renderer and have all of the aspect information
    //   defaultShader->setMat4("view", view);
    //   defaultShader->setMat4("projection", camera->projection);

    //   glBindVertexArray(obj->vao);
    //   glDrawArrays(GL_TRIANGLES, 0, obj->VertexCount);
    //}

}


Renderer::Renderer(const Camera* cam)
    :camera(cam)
{
    view = glm::mat4(1.0f); // identity camera

    densityBackground = new DensityBackground();

    // set the default shader for now. This might be replaced per object in the future
    defaultShader = new Shader("E:/dev/OpenGL/src/shaders/vertex/default_vertex.glsl", "E:/dev/OpenGL/src/shaders/fragment/default_fragment.glsl");

    densityShader = new Shader("E:/dev/OpenGL/src/shaders/vertex/density_vertex.glsl", "E:/dev/OpenGL/src/shaders/fragment/density_fragment.glsl");
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

    glGenBuffers(1, &particleSSBO);
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

void Renderer::UploadParticlePositions(const Scene& scene, float kernelRadius)
{
    gpuKernelRadius = kernelRadius;

    //clear data from previous render
    gpuParticlePositions.clear();

    for (auto& [id, obj] : scene.objects)
    {
        gpuParticlePositions.push_back(glm::vec2(obj->position));
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        gpuParticlePositions.size() * sizeof(glm::vec2),
        gpuParticlePositions.data(),
        GL_DYNAMIC_DRAW
    );
}

void Renderer::RenderDensityBackground()
{
    // need to get the object and bind the correct shader and texture

    densityShader->bind();

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, densityTexture);
    //densityShader->setInt("uDensity", 0);

    //glBindVertexArray(densityBackground->vao);
    //glDrawArrays(GL_TRIANGLES, 0, densityBackground->VertexCount);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);

    densityShader->setInt("particleCount", gpuParticlePositions.size());
    densityShader->setFloat("kernelRadius", gpuKernelRadius);

    densityShader->setVec2("worldMin", glm::vec2(camera->left_world_bound, camera->bottom_world_bound));
    densityShader->setVec2("worldMax", glm::vec2(camera->right_world_bound, camera->top_world_bound));

    glBindVertexArray(densityBackground->vao);
    glDrawArrays(GL_TRIANGLES, 0, densityBackground->VertexCount);
}