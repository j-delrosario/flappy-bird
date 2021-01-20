#pragma once
#
#include "GLCore/Core/Timestep.h"
#include "../Events/Event.h"
#include "OrthographicCamera.h"
#include <glm/glm.hpp>

namespace GLCore::RendererAPI {
	// 2D Batch Renderer
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		// New camera update functions
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void BeginBatch();
		static void EndBatch();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t textureID);

		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void EnableBlend();

		static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix);
	};
}