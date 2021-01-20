#pragma once

#include "glpch.h"

namespace GLCore::RendererAPI {

	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();
	public:
		GLuint m_TextureID;
	};
}