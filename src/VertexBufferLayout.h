#pragma once

#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	/* return size of type */
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}

};


class VertexBufferLayout
{

private:
	/* VertexBufferElement vector - m_Elements */
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	/* VertexBufferLayout constructor */
	VertexBufferLayout()
		: m_Stride(0) {}


	/* VertexBufferLayout ~destructor */


	/* create template<typename T> */
	template<typename T>

	void Push(unsigned int count)
	{
		static_assert(false);
	}

	/* create template<>  for float */
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	/* create template<> for unsigned int */
	template<>
	 void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	 /* create template<> for unsigned char */
	 template<>
	 void Push<unsigned char>(unsigned int count)
	 {
		 m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		 m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	 }

	 /* add inline method to get elements that returns const& */
	 inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	 /* add inline method to get stride */
	 inline unsigned int GetStride() const { return m_Stride;  }
};

