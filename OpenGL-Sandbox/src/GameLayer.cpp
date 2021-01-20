#include "GameLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;
using namespace GLCore::RendererAPI;

GameLayer::GameLayer()
    : Layer("GameLayer")
{
    Window& window = Application::Get().GetWindow();
    CreateCamera();

    m_Camera->SetPosition({ PLAYER_OFFSET, 0.0f, 0.0f });

    Random::Init();
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
    m_Level.Init();
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Timestep ts)
{
    if (m_Level.IsGameOver())
        m_GameState = GameState::GameOver;

    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    switch (m_GameState)
    {
    case GameState::Play:
        m_Level.OnUpdate(ts);
        m_Camera->SetPosition({ playerPos.x + PLAYER_OFFSET, 0.0f, 0.0f });
        break;
    case GameState::GameOver:
        m_Level.OnUpdate(ts);
        m_Camera->SetPosition({ playerPos.x + PLAYER_OFFSET, 0.0f, 0.0f });
        break;
    default:
        break;
    }

    Renderer::Clear();

    Renderer::BeginScene(*m_Camera.get());
    Renderer::BeginBatch();
    m_Level.OnRender();
    Renderer::EndBatch();
    Renderer::Flush();
    Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
}


void GameLayer::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseButtonPressedEvent>(GLCORE_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<KeyPressedEvent>(GLCORE_BIND_EVENT_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    switch (m_GameState)
    {
    case GameState::Play:
        m_Level.OnEvent(e);
        break;
    case GameState::MainMenu:
        m_GameState = GameState::Play;
        break;
    case GameState::GameOver:
        break;
    default:
        break;
    }
    return false;
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
    if (e.GetKeyCode() == HZ_KEY_SPACE) 
    {
        const auto& playerPos = m_Level.GetPlayer().GetPosition();
        switch (m_GameState)
        {
        case GameState::Play:
            m_Level.OnEvent(e);
            break;
        case GameState::MainMenu:
            m_GameState = GameState::Play;
            break;
        case GameState::GameOver:
            m_GameState = GameState::MainMenu;
            m_Level.Reset();
            m_Camera->SetPosition({ playerPos.x + PLAYER_OFFSET, 0.0f, 0.0f });
            break;
        default:
            break;
        }
    }
    return false;
}

bool GameLayer::OnWindowResize(WindowResizeEvent& e)
{
    m_CamWidth = e.GetWidth();
    m_CamHeight = e.GetHeight();
    CreateCamera();
    return false;
}

void GameLayer::CreateCamera()
{
    m_Camera = std::make_unique<OrthographicCamera>(-m_CamWidth, m_CamWidth, -m_CamHeight, m_CamHeight);
}
