#pragma once

#include "VertexBuffer.h"

/* Vid#16: (9:35) delete OR comment out #include "VertexBufferLayout.h" and 
	instead declare  class VertexBufferLayout; */
//#include "VertexBufferLayout.h"

class VertexBufferLayout;

class VertexArray
{
	
private:
	unsigned int m_RendererID;

public:
	/* VertexArray constructor */
	VertexArray();

	/* VertexArray ~destructor */
	~VertexArray();

	/* AddBuffer takes in 2 const params:
		1st - VertexBuffer& vb,
		2nd - VertexBufferLayout &, layout */
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	/* VertexArray Bind method */
	void Bind() const;

	/* VertexArray Unbind method */
	void Unbind() const;
};

