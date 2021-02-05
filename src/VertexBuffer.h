#pragma once

class VertexBuffer
{

private:
	/* m_RendererID is an OpenGL unique unsigned int Renderer ID for every type of object 
		(vertex buffer, index buffer, shader, texture, vertex array, frame buffer, etc)
		we allocate that identifies a specific object created */

	unsigned int m_RendererID;

public:
	/* constructor */
	VertexBuffer(const void* data, unsigned int size);

	/* destructor */
	~VertexBuffer();

	/* class methods */
	void Bind() const;
	void Unbind() const;

};