#include <GL/glew.h>  /* must be the 1st #include BEFORE ANY other #includes */
#include <GLFW/glfw3.h>

#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

/* Vid#17 Texture Class */
Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	/* Load the image: ChernoLogo.png and flip it vertically on load,
		b/c OpenGL expects us to start at the bottom-left as 0,0 (not top-left) */
	stbi_set_flip_vertically_on_load(1);

	/* initialize m_LocalBuffer passing in: 
		filename, 
		(3) pointers to the width, height, and Bits Per Pixel of the file (ChernoLogo.png is 460x460 pixels), 
		and (RGBA requires) 4 channels of variables */
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	/* Load textures using ID and Bind 2D Texture */
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	/* Set (4) Required Texture Integer Parameters: 
		(target, param_name: GL_TEXTURE_MIN_FILTER (resampled down per pixel), linear resampling)
		(target, param_name: GL_TEXTURE_MAG_FILTER (scale up render texture when area is larger in pixels than the texture size ), linear sampling)
		(target, param_name: GL_TEXTURE_WRAP_S (horizontal, x), clamp/not extend the area/no tiling)
		(target, param_name: GL_TEXTURE_WRAP_T (vertical, y), clamp/not extend the area/no tiling)
	*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	/* Vid#17 (14:00) Send OpenGL the Data in m_LocalBuffer using glTexImage2D(
		1st param: GL_TEXTURE_2D - target,
		2nd param: 0 - single level = 0, 
		3rd param: GL_RGBA8 - internal format is how OpenGL stores texture data, 
		4th param: texture width, 
		5th param: texture height,
		6th param: 0 pixel border,
		7th param: format of RGBA data, 
		8th param: GL_UNSIGNED_BYTE - data type, 
		9th param: pointer to pixels with data OR nullptr if just allocating space )
	*/
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);


	/* Unbind the Texture */
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	//glBindTexture(GL_TEXTURE_2D, 0);

	/* if m_LocalBuffer contains data, then free local buffer data */
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	/* destructor deletes texture(s) (passing in Renderer ID) from the GPU */
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	/* specify a texture 1st slot (GL_TEXTURE0) of 32 slots to activate 
		BEFORE binding that texture to slot0 */
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	//glActiveTexture(GL_TEXTURE0 + slot);
	//glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	/* Unbind the Texture */
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
