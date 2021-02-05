#pragma once

class IndexBuffer
{

private:
	/* m_RendererID is an OpenGL unique unsigned int Renderer ID for every type of object 
		(vertex buffer, index buffer, shader, texture, vertex array, frame buffer, etc)
		we allocate that identifies a specific object created */

	unsigned int m_RendererID;
	unsigned int m_Count;		// count of indices

public:
	/* constructor supports 32-bit indice elements (unsigned int* size in bytes) only 
		of (unsigned int element count) */
	IndexBuffer(const unsigned int* data, unsigned int count);

	/* destructor */
	~IndexBuffer();

	/* class methods */
	void Bind() const;
	void Unbind() const;

	/* Add inline GETTER GetCount() const that returns an unsigned int */
	inline unsigned int GetCount() const {
		return m_Count;
	};

};