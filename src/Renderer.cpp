/* Vid13: contains definitions declared in Renderer.h */

#include "Renderer.h"
#include <iostream>

void GLClearErrors()
{
    /* loop while there are errors and until GL_NO_ERROR is returned */
    while (glGetError() != GL_NO_ERROR);
}

/* Vid10:  GLLogCall() static function that returns a bool and accepts parameters that allow the console
    to printout the C++ source file, the line of code, and the function name that errored */

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") " << function
            << " " << file << ": " << line << std::endl;
        return false;
    }

    return true;
}

/* Vid#16: (10:45) Move glClear from Application.cpp to Renderer.h class Renderer public method */
void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

/* Vid#16 (5:30) add new Draw method definition that takes in a VertexArray, 
    IndexArray, and Shadow references */

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    /* Vid#16: (5:50) moved these 4 calls from Application.cpp while loop */

    /* Vid#15: (16:20) changed GLCall(glUseProgram(shader) to shader.Bind() */
    shader.Bind();      // bind our shader

    /* Vid#14: (24:00) Delete OR Comment out GLCall(glBindVertexArray(vao)); */
    //GLCall(glBindVertexArray(vao));     // bind our Vertex Array Object
    /*  and Replace with va.Bind() Call */

    va.Bind();

    /* Vid#13: (20:00) delete OR comment out this glBindBuffer and replace with: */

    ib.Bind();      // new bind call to Index Buffer Class method

    /* Vid#16: (6:20) change 2nd param from 6 to ib.GetCount() */

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));  // Draw Elements call

}

