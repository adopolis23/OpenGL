# Application Architecture - SDL

Application
* Window (handles SDL window + GL context)
* Renderer (sets GL state, draws objects)
* Scene (stores objects)
  - Box
  - Mesh
  - etc.
* Camera
  - Bounds
  - Projection for the renderer to use.
* Main loop (poll events → update → render)


## Application

Will contain main loop
* Poll events
* Update
* Render

## Window

The window should be responsible for:

* SDL initialization
* creating the SDL_Window
* creating the GL context
* swapping buffers
* window size / title
* providing raw input events (just forwarding them)

## Renderer

Objects in the scene will store data like VBO and VAO data.
The renderer will render all of them.
Something like `renderer.render(scene);`

Example:

```
void Renderer::render(const Scene& scene) {
    for (auto& object : scene.objects) {
        shader.bind();
        shader.setMat4("model", object.transform);
        glBindVertexArray(object.vao);
        glDrawElements(GL_TRIANGLES, object.indexCount, GL_UNSIGNED_INT, 0);
    }
}
```

The renderer will also keep track of shaders maybe?

## Scene

Will hold all information about objects in the scene such as the particles.
Will have something like `std::vector<Object*> objects;`

## Camera

Camera will store the worlds bounds that are a product of the projection
The Renderer will use the projection when drawing objects.