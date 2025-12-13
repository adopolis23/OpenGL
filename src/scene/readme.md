# Box Class

Extends from object

Has VAO and VBO

## VAO

Vertex Array Object

A VAO stores the state of your vertex attribute configuration, such as:
* Which VBOs are bound
* How your vertex data is laid out (stride, offset, formats)

## VBO

Vertex Buffer Object

A VBO is a chunk of GPU memory that stores your actual vertex data such as:
* positions
* texture UVs
* normals
* colors

```C
  // binds vbo and fills it will data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // static draw tells gpu the data wont change much. 
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
```