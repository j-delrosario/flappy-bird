#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <GLCoreRendererAPI.h>

#include "Defines.h"

#include "Player.h"

using namespace GLCore;
using namespace GLCore::Utils;
using namespace GLCore::RendererAPI;

struct Pillar
{
	glm::vec3 TopPosition;
	glm::vec2 TopSize = { 3.0f, 15.0f };

	glm::vec3 BottomPosition;
	glm::vec2 BottomSize = { 3.0f, 15.0f };
};

class Level
{
public:
	void Init();
	void OnUpdate(Timestep ts);
	void OnEvent(Event& e);
	void OnRender();

	bool IsGameOver() const { return m_GameOver; }
	void Reset();

	Player& GetPlayer() { return m_Player; }
private:
	void CreatePillar(int index, float offset);
	bool CollisionTest();
private:
	Player m_Player;

	bool m_GameOver = false;
	float m_Time = 0.0f;
	
	std::unique_ptr<Texture> m_BackgroundTexture;
	std::unique_ptr<Texture> m_FloorTexture;
	std::unique_ptr<Texture> m_TopPillarTexture;
	std::unique_ptr<Texture> m_BottomPillarTexture;

	int64_t m_FloorX1 = -1;
	int64_t m_FloorX2 =  1;

	int  m_PillarIndex = 0;
	float m_PillarTarget = 30.0f;
	std::vector<Pillar> m_Pillars;
};