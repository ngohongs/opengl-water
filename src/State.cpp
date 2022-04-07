#include "State.h"
#include "..\inc\State.h"
#include <iostream>

State state;

State::State(const int& width, const int& height)
{
	m_Window = Window(width, height);
	m_LastX = width / 2.0;
	m_LastY = height / 2.0;
	m_FirstMouse = true;
	m_FOV = 54.0f;
	m_DropPos = glm::vec2(0.5f);
	m_NowTime = glfwGetTime();
	m_LastTime = 0;
	m_DeltaTime = 0;
	m_DropTest = false;
	m_Abort = false;
	for (auto& i : m_KeyMap)
		i = false;
	m_Res = 64;
	m_Light = Light(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		{ 0.1f, 0.1f, 0.1f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f });
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)width / (float)height, 0.1f, 1000.0f);
	m_OrthogonalMatrix = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 100.0f);
}

int State::Init()
{
    return m_Window.Init();
}
void State::Update()
{
	m_NowTime = glfwGetTime();
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
	m_Geometry.emplace("debugQuad", Geometry(dquadVertices, dquadNVertices, dquadTriangles, dquadNTriangles));
	m_Geometry.emplace("debuglQuad", Geometry(dlquadVertices, dlquadNVertices, dlquadTriangles, dlquadNTriangles));
	m_Geometry.emplace("debugdlQuad", Geometry(ddlquadVertices, ddlquadNVertices, ddlquadTriangles, ddlquadNTriangles));
	m_Geometry.emplace("debugddQuad", Geometry(ddquadVertices, ddquadNVertices, ddquadTriangles, ddquadNTriangles));


	glm::vec3 pinkColor({ 1.0f, 0.0f, 1.0f });
	Material pinkMaterial{ 0.4f * pinkColor, 0.7f * pinkColor, 0.31f * pinkColor, 10.0f };

	m_Models.emplace("duck", Model("resources/duck/duck.obj"));
	m_Models.emplace("terrain", Model("resources/terrain/terrain.obj"));
	m_Models.emplace("cube", Model(Geometry(cubeVertices, cubeNVertices, cubeTriangles, cubeNTriangles), pinkMaterial));

	m_Models["duck"].SetScale(glm::vec3(0.2f));
	m_Models["duck"].SetPosition(glm::vec3(0.0f, -0.01f, -.5f));
	m_Models["terrain"].SetScale(glm::vec3(0.125f, 0.25f, 0.125f));
	m_Models["cube"].SetScale(glm::vec3(0.30f));

}

