#include "Level.h"

using namespace GLCore;
using namespace GLCore::Utils;
using namespace GLCore::RendererAPI;

void Level::Init()
{
	m_BackgroundTexture = std::make_unique<Texture>("assets/textures/Background.png");
	m_FloorTexture = std::make_unique<Texture>("assets/textures/Floor.png");
	m_TopPillarTexture = std::make_unique<Texture>("assets/textures/TopPipe.png");
	m_BottomPillarTexture = std::make_unique<Texture>("assets/textures/BottomPipe.png");
	m_Player.LoadAssets();

	m_Pillars.resize(5);
	for (int i = 0; i < 5; i++)
		CreatePillar(i, (i+1) * 10.0f);
}

void Level::OnUpdate(Timestep ts)
{
	if (m_GameOver)
	{
		if (m_Player.GetPosition().y > -CAM_HEIGHT + 2 * FLOOR_HEIGHT)
		{
			m_Player.OnUpdate(ts);
		}
		return;
	}

	m_Player.OnUpdate(ts);

	const auto& playerPos = m_Player.GetPosition();
	if (playerPos.x > (std::max(m_FloorX1, m_FloorX2) + 1) * CAM_WIDTH)
		m_FloorX1 > m_FloorX2 ? m_FloorX2 += 4 : m_FloorX1 += 4;

	if (CollisionTest())
	{
		m_GameOver = true;
		m_Time = 1.0f;
		m_Player.Kill();
		return;
	}
	if (m_Player.GetPosition().x > m_PillarTarget)
	{
		CreatePillar(m_PillarIndex, m_PillarTarget + 30.0f);
		m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
		m_PillarTarget += 10.0f;
	}
}

void Level::OnEvent(Event& e)
{
	if (!m_GameOver)
		m_Player.OnEvent(e);
}

void Level::OnRender()
{
	const auto& playerPos = m_Player.GetPosition();

	Renderer::DrawQuad({ -CAM_WIDTH + PLAYER_OFFSET + playerPos.x, -CAM_HEIGHT },    { 2 * CAM_WIDTH, 2 * CAM_HEIGHT   }, (uint32_t)m_BackgroundTexture->m_TextureID);
	Renderer::DrawQuad({ PLAYER_OFFSET + m_FloorX1 * CAM_WIDTH, -CAM_HEIGHT, 0.2f }, { 2 * CAM_WIDTH, 2 * FLOOR_HEIGHT }, (uint32_t)m_FloorTexture->m_TextureID);
	Renderer::DrawQuad({ PLAYER_OFFSET + m_FloorX2 * CAM_WIDTH, -CAM_HEIGHT, 0.2f }, { 2 * CAM_WIDTH, 2 * FLOOR_HEIGHT }, (uint32_t)m_FloorTexture->m_TextureID);

	for (auto& pillar : m_Pillars)
	{
		Renderer::DrawQuad(pillar.TopPosition, pillar.TopSize, (uint32_t)m_TopPillarTexture->m_TextureID);
		Renderer::DrawQuad(pillar.BottomPosition, pillar.BottomSize, (uint32_t)m_BottomPillarTexture->m_TextureID);
	}

	m_Player.OnRender();

	if (m_GameOver && m_Time > 0.0f)
	{
		m_Time -= 0.1f;
		Renderer::DrawQuad({ -CAM_WIDTH + PLAYER_OFFSET + playerPos.x, -CAM_HEIGHT, 1.0f }, { 2 * CAM_WIDTH, 2 * CAM_HEIGHT }, { 0.0f, 0.0f, 0.0f, m_Time });
	}
}

void Level::Reset()
{
	m_GameOver = false;
	m_Player.Reset();

	m_FloorX1 = -1;
	m_FloorX2 = 1;

	m_PillarTarget = 30.0f;
	m_PillarIndex = 0;
	for (int i = 0; i < m_Pillars.size(); i++)
		CreatePillar(i, (i + 1) * 10.0f);
}

void Level::CreatePillar(int index, float offset)
{
	Pillar& pillar = m_Pillars[index];
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;
	pillar.TopPosition.z = 0.1f;
	pillar.BottomPosition.z = 0.1f;

	float center = Random::Float() * 8.0f - 4.0f + FLOOR_HEIGHT;
	float gap = 5.0f;

	pillar.TopPosition.y = center + gap * 0.5f;
	pillar.BottomPosition.y = center - pillar.BottomSize.y - gap * 0.5f;
}

bool Level::CollisionTest()
{
	if (m_Player.GetPosition().y < -CAM_HEIGHT + 2 * FLOOR_HEIGHT)
		return true;

	glm::vec2 pos = { m_Player.GetPosition().x + 0.1f, m_Player.GetPosition().y + 0.1f };
	glm::vec2 size = { m_Player.GetSize().x - 0.2f, m_Player.GetSize().y - 0.2f };


	for (auto& pillar : m_Pillars)
	{
		if (pos.x < pillar.BottomPosition.x + pillar.BottomSize.x &&
			pos.x + size.x > pillar.BottomPosition.x &&
			pos.y + size.y > pillar.BottomPosition.y &&
			pos.y <  pillar.BottomPosition.y + pillar.BottomSize.y)
			return true;
		if (pos.x < pillar.TopPosition.x + pillar.TopSize.x &&
			pos.x + size.x > pillar.TopPosition.x &&
			pos.y + size.y > pillar.TopPosition.y &&
			pos.y < pillar.TopPosition.y + pillar.TopSize.y)
			return true;
	}

	return false;
}