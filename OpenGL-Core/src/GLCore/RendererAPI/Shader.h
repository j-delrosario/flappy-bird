#pragma once

#include "glpch.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
namespace GLCore::RendererAPI {

	class Shader
	{
	public:
		~Shader();

		GLuint GetRendererID() { return m_RendererID; }
		void SetUniformMat4(const std::string& name, const glm::mat4& value);

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		
	private:
		Shader() = default;

		void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		GLuint CompileShader(GLenum type, const std::string& source);
	private:
		GLuint m_RendererID;
	};

}