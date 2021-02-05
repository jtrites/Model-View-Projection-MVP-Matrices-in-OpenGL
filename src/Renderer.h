#pragma once

/* Vid#13: (6:30) Cut & Paste from Application.cpp to new Renderer.h file */
#include <GL/glew.h>  /* must be the 1st #include BEFORE ANY other #includes */

/* Vid#16: (5:20) add include libraries for the new Renderer class */
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


/* Vid#10: (14:30) add ASSERT(x) macro to validate a condition and call a breakpoint if true
    using the MSVC function __debugbreak() */

#define ASSERT(x) if (!(x)) __debugbreak();

    /* Vid#10: (16:20) GLCall(x) macro where (x) is the call function to Clear OpenGL Error(s)
        that calls the GLClearErrors() function */
        /* Vid#10 (18:45) use macros to find out which line of code this errored function occurred.
            In GLLogCall(x) - changed to a string (#x) for printing the file name (__FILE__),
            and printing the line number (__LINE__) */

#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* Vid13: remove static from GLClearErrors() and GLLogCall functions to make them declarations 
   and move their definitions to Renderer.cpp */

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int line);

/* Vid#16: (3:30) Create New Renderer Class - static or singleton? */
class Renderer
{

public:

    /* Vid#16: (10:40) add new void Clear() declaration */

    void Clear() const;

    /* Vid#16 (5:00) add new Draw method declaration that takes in a VertexArray, 
        IndexArray, and Shadow references */

    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
