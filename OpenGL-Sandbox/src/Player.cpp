#include "Player.h"

using namespace GLCore;
using namespace GLCore::Utils;
using namespace GLCore::RendererAPI;

Player::Player()
{
}

void Player::LoadAssets()
{
	m_NeutralFlapTexture = std::make_shared<Texture>("assets/textures/BirdNeutralFlap.png");
	m_DownFlapTexture = std::make_shared<Texture>("assets/textures/BirdDownFlap.png");
	m_UpFlapTexture = std::make_shared<Texture>("assets/textures/BirdUpFlap.png");

	m_Texture = m_NeutralFlapTexture;
}

void Player::OnUpdate(Timestep ts)
{
	m_Time += ts;
	if (m_Time > 0.1f)
	{
		if (m_Texture->m_TextureID == m_NeutralFlapTexture->m_TextureID)
			m_Texture = m_DownFlapTexture;
		else if (m_Texture->m_TextureID == m_DownFlapTexture->m_TextureID)
			m_Texture = m_UpFlapTexture;
		else if (m_Texture->m_TextureID == m_UpFlapTexture->m_TextureID)
			m_Texture = m_NeutralFlapTexture;
		m_Time = 0.0f;
	}

	if (m_Velocity.y > -m_Lift)
		m_Velocity.y -= m_Gravity;

	if (m_Velocity.y > 5.0f)
		m_Rotation = -0.3f;
	else if (m_Rotation < glm::pi<float>() / 2)
		m_Rotation += 0.035f;

	m_Position += m_Velocity * (float)ts;
}

void Player::OnEvent(Event& e)
{
	m_Velocity.y = m_Lift;
}

void Player::OnRender()
{
	Renderer::DrawQuad({ m_Position.x, m_Position.y}, m_Size, m_Rotation, (uint32_t)m_Texture->m_TextureID);
}

void Player::Reset()
{
	m_Position = { 0.0f, 0.0f };
	m_Velocity = { 5.0f, 0.0f };
	m_Rotation = 0.0f;
}

void Player::Kill()
{
	m_Velocity.y = -m_Lift;
}

uint32_t Player::GetScore()
{
	uint32_t score = (uint32_t)(m_Position.x + 10.0f) / 10.0f;
	if (score > m_HighScore)
		m_HighScore = score;
	return score;
}
