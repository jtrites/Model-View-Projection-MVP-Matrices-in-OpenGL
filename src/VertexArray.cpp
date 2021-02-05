#include "VertexArray.h"

#include "Renderer.h"

/* Vid#16: (9:45) #include VertexBufferLayout.h" - removes the infinite include looping error */
#include "VertexBufferLayout.h"

/* VertexArray constructor */
VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

/* VertexArray ~destructor */
VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

/* Bind the Vertex Buffer and Setup the Vertex Layout */
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    /* Bind the VertexArray */
    Bind();

    /* Bind the VertexBuffer */
	vb.Bind();

    /* Setup a Vertex Layout */
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];


        /* Enable or disable a generic vertex attribute array for index = 0 */
        GLCall(glEnableVertexAttribArray(i));

        /* When we specify the GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0)); ,
            index 0 (1st param) of this Vertex Array is going to be bound to the currently bound
            glBindBuffer(GL_ARRAY_BUFFER, buffer).

        define an array of generic vertex attribute data
            1st param index = i,
            2nd param size = element.count for a (3) component vector that represents each Vertex position,
            3rd param symbolic constant = element.type,
            4th param normalized = converted directly as fixed-point values (GL_FALSE) 4th param,
            5th param stride = the amount of bytes between each Vertex based on
                2nd param vec2 (x, y, z) component position vector of 3 floats = 12 bytes,
            6th param pointer = position has an offset pointer = 0 */

        GLCall(glVertexAttribPointer(i, element.count, element.type,
            element.normalized, layout.GetStride(), (const void*)offset));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
