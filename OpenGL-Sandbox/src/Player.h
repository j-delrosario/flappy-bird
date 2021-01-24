#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <GLCoreRendererAPI.h>

#include "Defines.h"

using namespace GLCore;
using namespace GLCore::Utils;
using namespace GLCore::RendererAPI;

class Player
{
public:
	Player();

	void LoadAssets();
	void OnUpdate(Timestep ts);
	void OnEvent(Event& e);
	void OnRender();

	void Reset();
	void Kill();

	const glm::vec2& GetPosition() const { return m_Position; }
	const glm::vec2& GetSize() const { return m_Size;  }
	uint32_t GetScore(); 
	uint32_t GetHighScore() const { return m_HighScore; }
private:
	float m_Time = 0.0f;
	float m_Gravity = 1.2f;
	float m_Lift = 20.0f;
	float m_Rotation = 0.0f;
	glm::vec2 m_Velocity = { 5.0f, 0.0f };
	glm::vec2 m_Position = { 0.0f, 0.0f };
	glm::vec2 m_Size = { 1.7f, 1.3f };
	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Texture> m_NeutralFlapTexture;
	std::shared_ptr<Texture> m_DownFlapTexture;
	std::shared_ptr<Texture> m_UpFlapTexture;
	uint32_t m_HighScore = 0;
};