#include "glpch.h"
#include "Shader.h"
#include "Texture.h"
#include "stb_image.h"

namespace GLCore::RendererAPI 
{
	Texture::Texture(const std::string& path)
	{
		int w, h, bits;
		stbi_set_flip_vertically_on_load(1);

		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, 4);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		stbi_image_free(pixels);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}
}
