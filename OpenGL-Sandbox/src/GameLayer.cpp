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

    m_RestartTexture = std::make_unique<Texture>("assets/textures/RestartBackdrop.png");
    m_ScoreTexture = std::make_unique<Texture>("assets/textures/ScoreBackdrop.png");
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
    m_Level.Init();

    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);
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
    switch (m_GameState)
    {
        case GameState::Play:
        {
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, 
                { ImGui::GetWindowPos().x + 160.0f, ImGui::GetWindowPos().y }, 0xffffffff, scoreStr.c_str());
            break;
        }
        case GameState::GameOver:
        {
            Renderer::BeginBatch();
            Renderer::DrawQuad({ m_Level.GetPlayer().GetPosition().x - 4.4f, 5.7f, 1.0f }, { 13.0f, 2.4f }, m_RestartTexture->m_TextureID);
            Renderer::DrawQuad({ m_Level.GetPlayer().GetPosition().x + 0.1f, 1.5f, 1.0f }, { 2.8f, 3.6f }, m_ScoreTexture->m_TextureID);
            Renderer::EndBatch();
            Renderer::Flush();

            std::string restartStr = std::string("Space to Restart");
            std::string scoreStr = std::string("Score");
            std::string score = std::to_string(m_Level.GetPlayer().GetScore());
            std::string highScoreStr = std::string("Best");
            std::string highScore = std::to_string(m_Level.GetPlayer().GetHighScore());

            ImGui::GetForegroundDrawList()->AddText(m_Font, 64.0f, 
                { ImGui::GetWindowPos().x + 5.0f, ImGui::GetWindowPos().y }, 0xffffffff, restartStr.c_str());
            ImGui::GetForegroundDrawList()->AddText(m_Font, 32.0f,
                { ImGui::GetWindowPos().x + 135.0f, ImGui::GetWindowPos().y + 100.0f }, 0xe81001ff, scoreStr.c_str());
            ImGui::GetForegroundDrawList()->AddText(m_Font, 32.0f,
                { ImGui::GetWindowPos().x + 160.0f, ImGui::GetWindowPos().y + 125.0f }, 0xe81001ff, score.c_str());
            ImGui::GetForegroundDrawList()->AddText(m_Font, 32.0f, 
                { ImGui::GetWindowPos().x + 142.0f, ImGui::GetWindowPos().y + 150.0f }, 0xe81001ff, highScoreStr.c_str());
            ImGui::GetForegroundDrawList()->AddText(m_Font, 32.0f,
                { ImGui::GetWindowPos().x + 160.0f, ImGui::GetWindowPos().y + 175.0f }, 0xe81001ff, highScore.c_str());
            break;
        }
    }
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
