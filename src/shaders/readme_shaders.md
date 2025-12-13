# Shaders

Has Fragment and Vertex shader.


## Vertex Shader

1. layout(location = 0) in vec3 aPos;

    Reads vertex positions from attribute slot 0 (your VAO’s VBO).

2. uniform mat4 model;

    The object’s model transform (translation/rotation/scale) is passed from C++.

3. gl_Position = model * vec4(aPos, 1.0);

    Transforms the vertex from object space → world space → clip space (here only model matrix).

    Outputs the vertex position to the GPU rasterizer for the next stage.