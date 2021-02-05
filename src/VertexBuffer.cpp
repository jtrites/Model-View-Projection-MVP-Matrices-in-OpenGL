/* Vid13: contains definitions declared in Renderer.h */
#include <GL/glew.h>  /* must be the 1st #include BEFORE ANY other #includes */
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "Renderer.h"



/* VertexBuffer constructor */
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/* VertexBuffer destructor */
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

/* VertexBuffer Bind method */
void VertexBuffer::Bind() const 
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

/* VertexBuffer Unbind method  */
void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
