#pragma once

#include "Renderer.h"

class Texture
{

private:

	/* Renderer ID, filepath, and local buffer */
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;

	/* Pixel Attributes width, height, and bits per pixel (BPP) */
	int m_Width, m_Height, m_BPP;

public:
	/* Texture constructor takes in a path */
	Texture(const std::string& path);

	/* Texture ~destructor */
	~Texture();

	/* Vid#17: (8:10) Bind Texture FCN assigning Texture to Bind to slot 0
		provides ability to assign multiple Textures to different slots
		Desktops typically can have 32 slots, Mobile typically can support 8 slots
		Can ask OpenGL how many open slots available on current platform */

	void Bind(unsigned int slot = 0) const;

	/* Unbind Texture FCN */
	void Unbind() const;

	/* Vid#17: (7:45) Texture Helper FCN declarations - NOT USED IN THIS VIDEO! */
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};
