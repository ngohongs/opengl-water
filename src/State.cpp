#include "State.h"

State state;

State::State(const int& width, const int& height)
{
	m_Stop = false;
	m_Window = Window(width, height);
	m_LastX = width / 2.0;
	m_LastY = height / 2.0;
	m_FirstMouse = true;
	m_FOV = 54.0f;
	m_DropPos = glm::vec2(0.5f);
	m_NowTime = (float) glfwGetTime();
	m_LastTime = 0;
	m_DeltaTime = 0;
	m_DropTest = false;
	for (auto& i : m_KeyMap)
		i = false;
	m_M1Prev = false;
	m_EPrev = false;
	m_CursorMode = false;
	m_Res = 64;
	m_Light = Light(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		{ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)width / (float)height, 0.1f, 1000.0f);
	m_OrthogonalMatrix = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 100.0f);

	m_Camera = Camera({ 1.61f, 1.46f, 1.61f }, { -0.56f, -0.64f, -0.56f }, { 0.0f , 1.0f, 0.0f }, -40.0f, -137.201f);
	
	Reset();
}

int State::Init()
{
    return m_Window.Init();
}
void State::Update()
{
	m_NowTime = (float) glfwGetTime();
	m_DeltaTime = m_NowTime - m_LastTime;
	m_LastTime = m_NowTime;
}

void State::GenerateGeometriesModels()
{
	std::vector<Vertex> planeVert;
	std::vector<unsigned int> planeInd;

	std::vector<Vertex> gridVert;

	PlaneGenerator().Generate(m_Res, planeVert, planeInd);
	PlaneGenerator().GenerateGrid(m_Window.GetWidth(), m_Window.GetHeight(), gridVert);

	m_Geometry.emplace("plane", Geometry(planeVert, planeInd));
	m_Geometry.emplace("grid", Geometry(gridVert));
	m_Geometry.emplace("quad", Geometry(quadVertices, quadNVertices, quadTriangles, quadNTriangles));

	m_Models.emplace("duck", Model("resources/duck/duck.obj"));
	m_Models.emplace("terrain", Model("resources/terrain/terrain.obj"));
	m_Models.emplace("cube", Model("resources/cube/cube.obj"));

	m_Models["duck"].SetScale(glm::vec3(0.2f));
	m_Models["duck"].SetPosition(glm::vec3(0.0f, -0.01f, -.5f));
	m_Models["terrain"].SetScale(glm::vec3(0.125f, 0.25f, 0.125f));
	m_Models["terrain"].SetPosition(glm::vec3(0.0f, state.m_BedHeight, 0.0f));
	m_Models["cube"].SetScale(glm::vec3(0.30f));
	m_Models["cube"].SetPosition(glm::vec3(0.0f, state.m_CubeHeight, 0.0f));
	m_Models["cube"].SetMaterial(Material(state.m_CubeColor, state.m_CubeColor, state.m_CubeColor, 30.0f));
}

void State::Reset()
{
	m_Abort = true;

	m_Attenuation = 1.0f;
	m_FirstStage = 0.2f;
	m_SecondStage = 0.6f;
	m_FirstStageColor = glm::vec3(2.0f, 204.0f, 147.0f) / 255.0f;
	m_SecondStageColor = glm::vec3(2.0f, 127.0f, 199.0f) / 255.0f;
	m_FinalStageColor = glm::vec3(1.0f, 9.0f, 100.0f) / 255.0f;

	m_BedHeight = -0.3f;
	m_CubeHeight = -0.6f;
	m_CubeColor = glm::vec3(1.0f, 1.0f, 1.0f);

	m_Radius = 1.5f;
	m_Amplitude = 0.05f;

	m_WaveSpeed = 0.2f;
	m_WaveDamping = 0.0f;
	m_WaveSlope = 0.3f;

	m_CausticsAbsorbtion = 1.0f;
	m_CausticsPower = 95.f;

	m_FirstGuess = 0.1f;

	m_Models["terrain"].SetPosition(glm::vec3(0.0f, state.m_BedHeight, 0.0f));
	m_Models["cube"].SetPosition(glm::vec3(0.0f, state.m_CubeHeight, 0.0f));
	m_Models["cube"].SetMaterial(Material(state.m_CubeColor, state.m_CubeColor, state.m_CubeColor, 30.0f));
}
